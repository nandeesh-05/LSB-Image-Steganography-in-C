# LSB Image Steganography in C

## 📌 Description

A C-based steganography application that hides and extracts secret text messages inside BMP images using the Least Significant Bit (LSB) technique. The project demonstrates secure data embedding without noticeable visual changes, using file handling and bitwise operations.

## 🎯 Key Highlights

* Hide secret text messages inside BMP images
* Extract hidden messages accurately
* No visible distortion in the original image
* Educational and practical implementation of steganography

## 🛠️ Technologies Used

* Programming Language: C
* Concepts: Bitwise Operations, File Handling
* Domain: Image Processing, Data Security

## ▶️ How to Compile and Run

```bash
gcc *.c -o stego
./stego --help
```

## 📌 Command-Line Usage

### Encode (Hide Message)

```bash
./stego -e inputimage.bmp secret.txt output.bmp
```

*Note: output.bmp is optional*

### Decode (Extract Message)

```bash
./stego -d output.bmp output.txt
```

*Note: output.txt is optional*

## ⚙️ Working Principle

The program embeds secret message bits into the least significant bits of image pixel data. During decoding, these bits are extracted and reconstructed to retrieve the original message.

## 📚 Skills Demonstrated

* C Programming
* File Handling
* Bitwise Operations
* Encoding and Decoding Algorithms
* Image Processing
* Algorithm Development
