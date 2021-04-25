#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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


int main() {
    FILE * bmp = fopen("c-programming-97x96.bmp", "rb") ; // file has to be open in binary mode in order to read anything from it
    if (!bmp) {
        printf("Failed to open the file!");
        return 1;
    }
    //printf("%llu\n", offsetof(struct tagBITMAPFILEHEADER, bfOffBits));
    //printf("%llu\n", sizeof(DWORD));
    //printf("%llu\n", sizeof(struct tagBITMAPFILEHEADER)); //size is 16 because size has to be divisible by sizeof largest member so we basically cannot just
    //read the whole  structure in one try because it may contains padding we need to read components one by one
    //fread(&BITMAPFILEHEADER, sizeof(struct tagBITMAPFILEHEADER), 1, bmp); // this will read exactly 16 bytes and won't work

    //*******************
    //READING FIRST HEADER
    //*******************
    struct tagBITMAPFILEHEADER BITMAPFILEHEADER;
    fread(&BITMAPFILEHEADER.bfType, sizeof BITMAPFILEHEADER.bfType, 1 , bmp);
    fread(&BITMAPFILEHEADER.bfSize, sizeof BITMAPFILEHEADER.bfSize, 1 , bmp);
    fread(&BITMAPFILEHEADER.bfReserved1, sizeof BITMAPFILEHEADER.bfReserved1, 1 , bmp);
    fread(&BITMAPFILEHEADER.bfReserved2, sizeof BITMAPFILEHEADER.bfReserved2, 1 , bmp);
    fread(&BITMAPFILEHEADER.bfOffBits, sizeof BITMAPFILEHEADER.bfOffBits, 1 , bmp);
    unsigned char type[2];
    type[0] = BITMAPFILEHEADER.bfType;
    type[1] = (BITMAPFILEHEADER.bfType >> 8u);

    printf("BITMAPFILEHEADER: \n");
    printf(" bfType:\t0x%X (%c%c)\n", BITMAPFILEHEADER.bfType, type[0], type[1]);
    printf(" bfSize:\t%u\n", BITMAPFILEHEADER.bfSize);
    printf(" bfReserved1:\t0x%X\n", BITMAPFILEHEADER.bfReserved1);
    printf(" bfReserved2:\t0x%X\n", BITMAPFILEHEADER.bfReserved2);
    printf(" bfOffBits:\t%u\n", BITMAPFILEHEADER.bfOffBits);

    //*******************
    //READING SECOND HEADER
    //*******************
    struct tagBITMAPINFOHEADER BITMAPINFOHEADER;
    printf("%llu\n", sizeof(BITMAPINFOHEADER));
    fread(&BITMAPINFOHEADER, sizeof(struct tagBITMAPINFOHEADER), 1, bmp);
    printf("BITMAPINFOHEADER:\n");
    printf("biSize: %u\n", BITMAPINFOHEADER.biSize);
    printf("biWidth: %u\n", BITMAPINFOHEADER.biWidth);
    printf("biHeight: %u\n", BITMAPINFOHEADER.biHeight);
    printf("biPlanes: %u\n", BITMAPINFOHEADER.biPlanes);
    printf("biBitCount: %u\n", BITMAPINFOHEADER.biBitCount);
    printf("biCompression: %u\n", BITMAPINFOHEADER.biCompression);
    printf("biSizeImage: %u\n", BITMAPINFOHEADER.biSizeImage);
    printf("biXPelsPerMeter: %u\n", BITMAPINFOHEADER.biXPelsPerMeter);
    printf("biYPelsPerMeter: %u\n", BITMAPINFOHEADER.biYPelsPerMeter);
    printf("biClrUsed: %u\n", BITMAPINFOHEADER.biClrUsed);
    printf("biClrImportant: %u\n", BITMAPINFOHEADER.biClrImportant);

    fseek(bmp, BITMAPFILEHEADER.bfOffBits, SEEK_SET); //file position indicator will be moved at bffOffBist byte

    //printf("Each row has : %u Pixels\n", ((BITMAPINFOHEADER.biBitCount * BITMAPINFOHEADER.biWidth +31)/32) * 4);
    //printf("Height: %u\n", BITMAPINFOHEADER.biHeight);
    int width = ((BITMAPINFOHEADER.biBitCount * BITMAPINFOHEADER.biWidth +31)/32) * 4 ;
    int height = BITMAPINFOHEADER.biHeight;
    uint8_t pixel;

    int Histogram[3][16]; // 0 - red, 1- green 2-blue
    int image[3][height][width];

    for (int i = 0; i < 3; ++i) {
        memset(Histogram[i], 0, 16*sizeof(int));
    }



    for (int i = 0; i < height; ++i) {
        int cnt = 0;
        for (int j = 0; j < width; ++j) {
            fread(&pixel, sizeof pixel, 1, bmp);
            if(j == width - 1) break;

            Histogram[j%3][pixel/16] += 1;
            image[j%3][i][cnt] = pixel;

            if (j%3 == 2) cnt++;
            // printf("%u\n", pixel);
        }

    }
    for (int i = 0; i < 3; ++i) {
        if (i== 0) printf("RED: \n");
        else if (i == 1) printf("GREEN: \n");
        else printf("BLUE: \n");
        for (int j = 0; j < 16; ++j) {
            printf("%d-%d: %f%%\n", j*16, (j+1)*16-1, (double)(Histogram[i][j])/(width*height/3)*100);
        }
    }

    FILE *greyscale = fopen("greyscale.bmp", "wb");
    //uint8_t * fileHeader = malloc(sizeof(uint8_t) * 14);

    fwrite(&BITMAPFILEHEADER.bfType, sizeof BITMAPFILEHEADER.bfType, 1 , greyscale);
    fwrite(&BITMAPFILEHEADER.bfSize, sizeof BITMAPFILEHEADER.bfSize, 1 , greyscale);
    fwrite(&BITMAPFILEHEADER.bfReserved1, sizeof BITMAPFILEHEADER.bfReserved1, 1 , greyscale);
    fwrite(&BITMAPFILEHEADER.bfReserved2, sizeof BITMAPFILEHEADER.bfReserved2, 1 , greyscale);
    fwrite(&BITMAPFILEHEADER.bfOffBits, sizeof BITMAPFILEHEADER.bfOffBits, 1 , greyscale);
    fwrite(&BITMAPINFOHEADER, sizeof BITMAPINFOHEADER,1 , greyscale);

    int padding = BITMAPINFOHEADER.biSize - 40; // to know how many -0 bits we have to add
    uint8_t pad[padding];
    memset(pad,0,sizeof(uint8_t) * padding);
    fwrite(pad, sizeof(uint8_t) ,padding , greyscale);

    fclose(greyscale);
    fclose(bmp);
    return 0;
}
