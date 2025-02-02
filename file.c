/*  
   This program contains the logic for:  
       - User Registration  
       - User Login  
       - Creating a journal (Journal entries are encrypted using AES-256-CBC)  
       - Reading the journal  

   The project is currently under development, meaning contributions and new features from contributors are welcome!  
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <ctype.h>
#define TOKEN_LENGTH 16 
#define BUFFER  10000
#define KEY_SIZE 32  // AES-256 Key Size
#define IV_SIZE 16   // AES IV Size

// Generate a random AES key (this should be stored securely)
unsigned char key[KEY_SIZE] = "mystrongsecretkeyforjournal!!!";  // Ideally, store this securely.
unsigned char iv[IV_SIZE];


unsigned char* Generate_secure_token();
void validate_user_credentials(char *username, char *password);
void Register_user_credentials(char *username, char *password);
void create_journal(char *post, char *username);
int is_validate(char *username);
void Read_Journal(char *username);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext);

// Encrypt a string using AES-256-CBC
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) return -1;

    // Initialize encryption operation
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) return -1;

    // Encrypt the data
    if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1) return -1;
    ciphertext_len = len;

    // Finalize encryption
    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) return -1;
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

// Decrypt an AES-256-CBC encrypted string
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if (!(ctx = EVP_CIPHER_CTX_new())) return -1;

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) return -1;

    if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1) return -1;
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) return -1;
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}



unsigned char* Generate_secure_token() {

    int open_file = open("/dev/urandom",O_RDONLY); // linux pre-install randomness file 

    if(open_file < 0) {
        perror("Failed to Generate Key");
        return NULL;
    }

    unsigned char* key_store = malloc(TOKEN_LENGTH);
    if(!key_store) {
        perror("Memmory allocation failed.");
        close(open_file);
        return NULL;
    }
    read(open_file,key_store,TOKEN_LENGTH);
    close(open_file);

    unsigned char* token = Generate_secure_token();
    for(int i =0; i<TOKEN_LENGTH; i++) {
        printf("%02x", token[i]);
    }
    printf("\n");
    free(token);

    return key_store;

}

// validators
void validate_user_credentials(char *username, char* password) {

    FILE *open_file;
    char buffer[BUFFER];
    

    open_file= fopen("user_data.txt","r");
    if(open_file == NULL) {
        perror("Failed to read file");
        return ;
    }
    char file_username[BUFFER], file_password[BUFFER];

    while(fgets(buffer,sizeof(buffer),open_file)!= NULL){

        buffer[strcspn(buffer, "\n")] = '\0';

        if(sscanf(buffer,"%s %s", file_username, file_password)==2) {
            if(strcmp(username , file_username)== 0 && strcmp(password, file_password)== 0) {
                printf("Welcome, %s\n" , username);
                fclose(open_file);
                return;
            }
        }
    }

    printf("invalid Credentials.\n");
    fclose(open_file);

}

// validators
int is_validate(char *username) {

    FILE *openfile;
    openfile  = fopen("user_data.txt", "r");

    if(openfile==NULL) {
        printf("Something went wrong.");
        return -1;
    }

    char buffer[BUFFER];
    char get_username[BUFFER], get_password[BUFFER];

    while(fgets(buffer, sizeof(buffer), openfile) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if(sscanf(buffer, "%s %s", get_username, get_password) == 2) {
            if(strcmp(username, get_username) == 0) {
                fclose(openfile);
                return 2;
            }
        }
    }
    fclose(openfile);
    return 1;

}

// append only file
void Register_user_credentials(char *username, char *password) {

    FILE *open_file;
    open_file = fopen("user_data.txt", "a");

    if(open_file == NULL) {
        printf("File has been Generated.");
    }

    int result = is_validate(username);

    if(result != 2) {
        printf("\nRegistration successful 🎉 ! Please log in\n");
        fprintf(open_file,"\n%s %s", username, password);
    }else {
        printf("\n oops ! Username already exists.\n");
    }

    
    fclose(open_file);
}
 
void create_journal(char *post, char *username) {
    FILE *openfile = fopen("journal.txt", "a");
    if (!openfile) {
        perror("Error opening file");
        return;
    }

    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);

    unsigned char encrypted_post[1024];
    int encrypted_len = encrypt((unsigned char *)post, strlen(post), encrypted_post);

    if (encrypted_len == -1) {
        printf("Encryption failed!\n");
        fclose(openfile);
        return;
    }

    int result = is_validate(username);
    if (result == 2) {
        fprintf(openfile, "\nAuthored by: %s\nCreated at: %sEntry: ", username, asctime(ptr));
        fwrite(encrypted_post, 1, encrypted_len, openfile);
        fprintf(openfile, "\n");
        printf("\nJournal has been saved securely 🎉.\n");
    } else {
        printf("Oops! Credentials have not been registered yet.\n");
    }

    fclose(openfile);
}


// Creating the efficient algo for latest post : A Dynamic Array.
typedef struct {
    char created_at[BUFFER];
    char post[BUFFER];
}LatestPost;

void trim_whitespace(char *str) {
    // Trim leading spaces
    char *start = str;
    while (isspace(*start)) {
        start++;
    }
    memmove(str, start, strlen(start) + 1);

    // Trim trailing spaces
    char *end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';
}

void Read_Journal(char *username) {
    FILE *openfile = fopen("journal.txt", "r");
    if (!openfile) {
        perror("Error opening file");
        return;
    }

    char buffer[1024];
    char get_username[100], get_created_at[100], encrypted_post[1024];
    unsigned char decrypted_post[1024];
    int found = 0, count = 0;

    LatestPost *lffl = NULL;

    int result = is_validate(username);
    if (result != 2) {
        printf("\n======> User %s not found. <=======\n\n", username);
        fclose(openfile);
        return;
    } else {
        printf("\n=====> Authored by: %s  <=======\n\n", username);
    }

    // Trim input username to handle any accidental whitespace
    trim_whitespace(username);

    while (fgets(buffer, sizeof(buffer), openfile) != NULL) {
        if (strncmp(buffer, "Authored by:", 12) == 0) {
            sscanf(buffer, "Authored by: %99[^\n]", get_username);
            trim_whitespace(get_username); // Trim whitespace from file's username

            fgets(buffer, sizeof(buffer), openfile);
            sscanf(buffer, "Created at: %99[^\n]", get_created_at);

            fgets(buffer, sizeof(buffer), openfile);
            sscanf(buffer, "Entry: %1023[^\n]", encrypted_post);

            if (strcmp(username, get_username) != 0) {
                continue; // Skip if username doesn't match after trimming
            }

            int decrypted_len = decrypt((unsigned char *)encrypted_post, strlen(encrypted_post), decrypted_post);
            if (decrypted_len == -1) {
                continue;
            }
            decrypted_post[decrypted_len] = '\0';

            LatestPost *temp = realloc(lffl, (count + 1) * sizeof(LatestPost));
            if (!temp) {
                perror("Memory allocation failed");
                free(lffl);
                fclose(openfile);
                return;
            }
            lffl = temp;

            strcpy(lffl[count].created_at, get_created_at);
            strcpy(lffl[count].post, (char *)decrypted_post);
            count++;
            found = 1;
        }
    }
    fclose(openfile);

    if (!found) {
        printf("No journal entries found for user %s.\n", username);
    } else {
        for (int i = count - 1; i >= 0; i--) {
            printf("Time: %s\nEntry: %s\n\n", lffl[i].created_at, lffl[i].post);
        }
    }

    free(lffl);
}

int main() {
    char username[100], password[12], post[1024];
    char option[10];  
    int loggedIn = 0;

    printf("Welcome to the Secure Journal System!\n");

    while (1) {
        printf("\nChoose an option:\n");
        printf("1. Register Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        
        fgets(option, sizeof(option), stdin);
        option[strcspn(option, "\n")] = '\0'; // Remove newline

        if (strcmp(option, "1") == 0) {
            // Get username
            printf("Enter username: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';

            // Get password
            printf("Enter password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';

            Register_user_credentials(username, password);
        } 
        else if (strcmp(option, "2") == 0) {
            // Get username
            printf("Enter username: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';

            int result = is_validate(username);
            if(result == 2) {
                printf("\nWelcome to Journaling System [Beta]\n");
                loggedIn = 1;
                break;    
            }else {
                printf("🚫 Invalid Credentials. Make sure you exist.\n");
            }
            
        } 
        else if (strcmp(option, "3") == 0) {
            printf("Exiting the system. Goodbye!\n\n");
            return 0;
        } 
        else {
            printf("🚫 Invalid choice. Please try again.\n\n");
        }
    }

    // If login is successful, allow journaling
    if (loggedIn) {
        while (1) {
            printf("\nJournal Options:\n");
            printf("1. Write a Journal Entry\n");
            printf("2. Read Journal\n");
            printf("3. Logout\n");
            printf("Enter your choice: ");

            fgets(option, sizeof(option), stdin);
            option[strcspn(option, "\n")] = '\0';

            if (strcmp(option, "1") == 0) {
                printf("Write your journal entry below:\n");
                fgets(post, sizeof(post), stdin);
                create_journal(post, username);
            } 
            else if (strcmp(option, "2") == 0) {
                Read_Journal(username);
            } 
            else if (strcmp(option, "3") == 0) {
                printf("Logging out...\n\n");
                break;
            } 
            else {
                printf("Invalid choice. Try again.\n\n");
            }
        }
    }

    return 0;
}
