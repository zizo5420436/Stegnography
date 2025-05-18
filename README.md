# 🕵️‍♂️ Steganography - Hiding Messages in Images

### 📌 Overview
**Steganography** is the art of hiding secret information within digital media in a way that makes the existence of the communication invisible.  
This project implements **image steganography** using the **Least Significant Bit (LSB)** technique, specifically for **24-bit BMP images**, allowing you to **embed** and **extract** hidden messages.

---

### 🛠 Features

- 🔹 **Hide Secret Messages** – Embed textual data into BMP images with imperceptible changes  
- 🔹 **Extract Hidden Data** – Retrieve embedded messages accurately  
- 🔹 **LSB Encoding** – Uses least significant bit manipulation for secure embedding  
- 🔹 **Lossless Image Preservation** – Original image appearance is virtually unchanged  
- 🔹 **Simple and Lightweight** – Written in **C**, focused on performance and portability  

---

### 🚀 How It Works

#### ✅ Encoding:
1. The secret message is first converted into binary format.  
2. The binary bits are embedded one by one into the **least significant bits** of the image's pixel values (RGB components).  
3. The result is a visually identical image with the message hidden inside.

#### ✅ Decoding:
1. The program reads the **least significant bits** of pixels sequentially.  
2. It reconstructs the binary stream and converts it back to readable text to reveal the hidden message.

---

### 📋 Requirements

- ✅ C Compiler (e.g., **GCC** or **Clang**)  
- ✅ **24-bit BMP** image files  
- ✅ OS: Linux, macOS, or Windows (with **MinGW** or equivalent)

---

### 🔒 Use Cases

- ✔ Secure Communication  
- ✔ Watermarking Digital Media  
- ✔ Confidential File Tagging  
- ✔ Data Hiding in Surveillance Systems  

---

### 📥 How to Compile

```bash
gcc encode.c decode.c steganography_utils.c -o steganography
```

---

### ▶️ How to Run

```bash
# To encode
./steganography encode input.bmp output.bmp "Your secret message"

# To decode
./steganography decode output.bmp
```

---

### 🧪 Sample Output

**Encoding:**
```
Embedding message: "The password is 1234"
Message successfully embedded in output.bmp
```

**Decoding:**
```
Extracted message: "The password is 1234"
```

---
