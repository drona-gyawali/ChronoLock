> Note: This project was created as part of my first semester coursework for the subject "Programming with C." Therefore, external contributions are not being accepted.
##  **Secure Journal System**  

**A command-line based journal system where users can register, log in, and create encrypted journal entries using AES-256 encryption.**  
The goal is to ensure privacy and security while maintaining an easy-to-use interface.

---

##  **Features**  
**User Registration & Login** – Secure authentication system.  
**Create Encrypted Journal Entries** – Uses **AES-256-CBC** encryption for privacy.  
**Read Journal Entries** – Automatically decrypts when accessed by the owner.  
**Secure File Storage** – Journal entries are stored in a structured format.  

---

## **CLI - Registration**
>![Screenshot from 2025-05-26 17-44-16](https://github.com/user-attachments/assets/15796384-c8f2-486d-a7b6-95a5bb338f5c)

## **After Login - Journal Writing**
>![Screenshot from 2025-05-26 17-44-04](https://github.com/user-attachments/assets/f850d579-0e89-43f0-a7a4-c5db9f08f57d)



## 🛠 **Setup & Installation**  

### **1️⃣ Prerequisites**  
Before you begin, ensure you have the following installed:  
- **GCC (GNU Compiler Collection)** – To compile C code.  
- **OpenSSL** – Required for AES encryption. Install it using:  
  ```bash
  sudo apt update && sudo apt install libssl-dev
  ```

---

### **2️⃣ Clone the Repository**
First, navigate to your desired directory and clone the project:  
```bash
git clone https://github.com/drona-gyawali/ChronoLock.git
cd ChronoLock
```

---

### **3️⃣ Compile the Project**  
Use the **Makefile** to compile the program:  
```bash
make
```

---

### **4️⃣ Run the Program**  
After compiling, execute the journal system:  
```bash
make run
```

---

### **5️⃣ Clean Compiled Files**  
To remove generated files and free up space, use:  
```bash
make clean
```

---

## **How It Works**
### **1️⃣ User Registration**  
- The system asks for a **username and password**.
- If the user is **new**, they get **registered**.

### **2️⃣ Creating a Journal Entry**  
- Users can write an entry, and it gets **encrypted** before being saved.

### **3️⃣ Reading Journal Entries**  
- Users retrieve their entries, and the system **decrypts them** in real-time.

---
