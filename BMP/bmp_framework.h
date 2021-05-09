#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef BMP_BMP_FRAMEWORK_H
#define BMP_BMP_FRAMEWORK_H

//aliasing types
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

typedef struct tagBITMAPFILEHEADER {
    WORD  bfType; // 2 bytes Indicates the file type. Has to be equal "BM"
    DWORD bfSize; // 4 bytes Size in bytes
    WORD  bfReserved1; // 2 bytes Reserved field not used, has to be equal 0
    WORD  bfReserved2; // 2 bytes Reserved field not used, has to be equal 0
    DWORD bfOffBits; // 4 bytes Addres where actuall bitmap starts
}*LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize; // 4 bytes of actual size of this header
    LONG  biWidth; //4 bytes width od the bitmap
    LONG  biHeight; // 4 bytes height of bitmap
    WORD  biPlanes; // 2 bytes not used (but is set to 1)
    WORD  biBitCount; // 2 bytes describing how many bits we use to represent 1 pixel
    DWORD biCompression; // 4 bytes in our case must be set to 0
    DWORD biSizeImage; // 4 bytes image size
    LONG  biXPelsPerMeter; // 4-byte horizontal DPI
    LONG  biYPelsPerMeter; // 4-byte vertical DPI
    DWORD biClrUsed; // 4-byte number of colors in the palette
    DWORD biClrImportant; // 4-byte number of important colors (not used)
}  *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;


void to_bin(int a, char * s); //auxiliary function that converts int a into string and writes it in char s
int to_num(char * s); //auxiliary functions that converts char s into integer and return it
struct tagBITMAPFILEHEADER read_f_header(FILE * bmp); // function that reads file header from bmp file and return it
void print_f_header(struct tagBITMAPFILEHEADER BITMAPFILEHEADER); //function that prints file header
struct tagBITMAPINFOHEADER read_i_header(FILE * bmp); // function that reads info header from bmp file
int print_i_header(struct tagBITMAPINFOHEADER BITMAPINFOHEADER); //function that prints info header from bmp file
//it additionally return flag wheather file is supported or not
uint8_t *** read_image_create_hist(struct tagBITMAPFILEHEADER BITMAPFILEHEADER, struct tagBITMAPINFOHEADER BITMAPINFOHEADER, FILE * bmp, int print_hist);
//function that takes both headers, pointer to the file, and one additional flag and returns image mapped into 3D array
void create_greyscale(char * filename, struct tagBITMAPFILEHEADER BITMAPFILEHEADER, struct tagBITMAPINFOHEADER BITMAPINFOHEADER, FILE * bmp, uint8_t ***image);
//function that creates greyscale of given bmp image and saves it into file
void decode_Steganography(uint8_t ***image, struct tagBITMAPINFOHEADER BITMAPINFOHEADER);
//function that decodes steganography given 3d array representation of an image
void encode_Steganography(char * filename, char * encode_message, struct tagBITMAPFILEHEADER BITMAPFILEHEADER, struct tagBITMAPINFOHEADER BITMAPINFOHEADER, FILE * bmp, uint8_t ***image);
//function that encode given message in copy of given bmp file.
void free_image(uint8_t ***image, struct tagBITMAPINFOHEADER BITMAPINFOHEADER);
//function that free memory of 3d array

#endif //BMP_BMP_FRAMEWORK_H
