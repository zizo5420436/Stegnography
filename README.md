Steganography - Hiding Messages in Images
📌 Overview
Steganography is the technique of hiding secret information within digital media, making the presence of communication undetectable. This project implements image steganography using the Least Significant Bit (LSB) technique, specifically in BMP images, to embed and extract hidden messages.

🛠 Features
🔹 Hide Secret Messages – Embed text into BMP images without noticeable distortion.
🔹 Extract Hidden Data – Retrieve the embedded message from the image.
🔹 Least Significant Bit (LSB) Encoding – Efficient bit-level manipulation for secure data hiding.
🔹 Lossless Image Preservation – Ensures minimal visual changes to the original image.
🔹 Simple and Lightweight – Written in C, with a focus on efficiency and performance.
🚀 How It Works
Encoding: The secret message is converted into binary and embedded into the least significant bits of the image pixels.
Decoding: The hidden data is extracted by reading the modified bits and reconstructing the original message.

📌 Requirements
C Compiler (GCC)
BMP image files (24-bit)
Linux/macOS/Windows (with MinGW)
🔒 Use Cases
✔ Secure Communication
✔ Watermarking Digital Content
✔ Protecting Sensitive Data
