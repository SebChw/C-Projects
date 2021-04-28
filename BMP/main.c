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

void to_bin(int a, char * s){
    int it = 0;
    while(a > 0){
        s[it] = a%2 + 48;
        ++it;
        a /= 2;
    }
    while(it < 8){
        s[it] = '0';
        ++it;
    }
}

int to_num(char * s){
    int k = 1;
    int ans = 0;
    for (int i = 0; i < strlen(s); ++i) {
        int tmp = s[i] - 48;
        ans += tmp * k;
        k *= 2;
    }

    return ans;
}

int main(int argc, char *argv[]) {
    if(argc < 2 || argc > 4){
        printf("Wrong number of parameters\n");
        return 1;
    }
    FILE * bmp = fopen(argv[1], "rb") ; // file has to be open in binary mode in order to read anything from it
    if (!bmp) {
        printf("Failed to open the file!\n");
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
    printf("%lu\n", sizeof(BITMAPINFOHEADER));
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
    if(BITMAPINFOHEADER.biCompression != 0 || BITMAPINFOHEADER.biBitCount != 24){
        printf("Unsupported type of .bmp file\n");
        return 1;
    }
    fseek(bmp, BITMAPFILEHEADER.bfOffBits, SEEK_SET); //file position indicator will be moved at bffOffBist byte

    //printf("Each row has : %u Pixels\n", ((BITMAPINFOHEADER.biBitCount * BITMAPINFOHEADER.biWidth +31)/32) * 4);
    //printf("Height: %u\n", BITMAPINFOHEADER.biHeight);
    int width = ((BITMAPINFOHEADER.biBitCount * BITMAPINFOHEADER.biWidth +31)/32) * 4 ; //amount of pixel + padding in a row
    int height = BITMAPINFOHEADER.biHeight;
    int true_width = BITMAPINFOHEADER.biWidth * 3; // real amount of pixel in a row
    uint8_t pixel;

    int Histogram[3][16]; // 0 - red, 1- green 2-blue

    uint8_t ***image = malloc(sizeof(uint8_t **) * 3 );
    for (int i = 0; i < 3; ++i) {
        image[i] = malloc(sizeof(uint8_t *) * height);
        for (int j = 0; j < height; ++j) {
            image[i][j] = calloc(true_width, sizeof(uint8_t));
        }
    }

    //int image[3][height][width];

    for (int i = 0; i < 3; ++i) {
        memset(Histogram[i], 0, 16*sizeof(int));
    }

    //printf("Liczymy");

    for (int i = 0; i < height; ++i) {
        int cnt = 0;
        for (int j = 0; j < width; ++j) {
            fread(&pixel, sizeof(pixel), 1, bmp);
            if(j >= true_width) continue;

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

    if(argc == 2){
        printf("Decode steganography? [Y/n]\n");
        char decision;
        decision = getchar();

        int to_decode;
        int n_bit = 0, n_sgn = 0;
        char * s = malloc(sizeof(char) * 9);
        char * decoded;
        for (int i = 0; i < 8; ++i) {
            s[i] = '0';
        }
        s[8] = '\0';
        //printf("%s\n", s);
        if(decision == 'Y' || decision == 'y'){
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < (int)(true_width/3); ++j) {
                    for (int k = 0; k < 3; ++k) {
                        uint8_t pixel2 = image[k][i][j];
                        if(n_sgn == 0){
                            s[n_bit] = pixel2 & 1 + 48;
                            ++n_bit;
                            if(n_bit == 8){
                                ++n_sgn;
                                to_decode = to_num(s);
                                  //printf("%s %d\n", s, to_decode);
                                n_bit = 0;
                                decoded = malloc(sizeof(char) * (to_decode +1)); // we need to decode one more character
                            }
                        }
                        else if(n_sgn <= to_decode){
                            s[n_bit] = pixel2 & 1 + 48;
                            ++n_bit;
                            if(n_bit == 8){
                                int dec_char = to_num(s);
                                // printf("%s %d\n", s, to_decode);
                                decoded[n_sgn-1] = (char)(dec_char);
                                ++n_sgn;
                                n_bit = 0;
                            }
                        }
                    }
                }
            }
            decoded[to_decode] = '\0'; // need to terminate this char
            printf("Decoded message:\n%s\n", decoded);
            free(decoded);
            free(s);
        }
    }

    if(argc == 3){
        FILE *greyscale = fopen(argv[2], "wb");

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

        fseek(bmp, BITMAPFILEHEADER.bfOffBits, SEEK_SET); // moving pointer at the beggining of pixels
        //Making greyscale
        uint8_t additional_pixels[true_width-width];
        memset(pad,0,sizeof(uint8_t) * (true_width-width));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < (int)(true_width/3); ++j) {
                uint8_t greypixel = (unsigned int)(image[0][i][j] + image[1][i][j] + image[2][i][j])/3;
                fwrite(&greypixel, sizeof(uint8_t), 1, greyscale);
                fwrite(&greypixel, sizeof(uint8_t), 1, greyscale);
                fwrite(&greypixel, sizeof(uint8_t), 1, greyscale);
            }
            fwrite(additional_pixels, sizeof(uint8_t), (true_width-width), greyscale);
        }
        fclose(greyscale);
    }

    if(argc == 4){
        FILE *encoded = fopen(argv[2], "wb");

        fwrite(&BITMAPFILEHEADER.bfType, sizeof BITMAPFILEHEADER.bfType, 1 , encoded);
        fwrite(&BITMAPFILEHEADER.bfSize, sizeof BITMAPFILEHEADER.bfSize, 1 , encoded);
        fwrite(&BITMAPFILEHEADER.bfReserved1, sizeof BITMAPFILEHEADER.bfReserved1, 1 , encoded);
        fwrite(&BITMAPFILEHEADER.bfReserved2, sizeof BITMAPFILEHEADER.bfReserved2, 1 , encoded);
        fwrite(&BITMAPFILEHEADER.bfOffBits, sizeof BITMAPFILEHEADER.bfOffBits, 1 , encoded);
        fwrite(&BITMAPINFOHEADER, sizeof BITMAPINFOHEADER,1 , encoded);

        int padding = BITMAPINFOHEADER.biSize - 40; // to know how many -0 bits we have to add
        uint8_t pad[padding];
        memset(pad,0,sizeof(uint8_t) * padding);
        fwrite(pad, sizeof(uint8_t) ,padding , encoded);

        fseek(bmp, BITMAPFILEHEADER.bfOffBits, SEEK_SET); // moving pointer at the beggining of pixels
        uint8_t additional_pixels[true_width-width];
        memset(pad,0,sizeof(uint8_t) * (true_width-width));


        int length = strlen(argv[3]);
        char * s = malloc(sizeof(char) * 9);
        for (int i = 0; i < 8; ++i) {
            s[i] = '0';
        }
        to_bin(length, s);
        int to_encode = length + 1;
        int n_bit = 0, n_sgn = 0; //current bit to be encoded, current char form string to be encoded

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < (int)(true_width/3); ++j) {
                for (int k = 0; k < 3; ++k) {
                    uint8_t pixel1 = image[k][i][j];
                    if(n_sgn < to_encode){
                        // printf("%d %d %s \t%d\n", n_sgn, n_bit, s, pixel1);
                        if(s[n_bit] == '0')
                            pixel1 &= 254;
                        else
                            pixel1 |= 1;
                        // printf("Pixel = %d\n", pixel1);
                        ++n_bit;
                        if(n_bit == 8){
                            n_bit = 0;
                            ++n_sgn;
                            to_bin(argv[3][n_sgn-1], s);
                        }
                    }
                    fwrite(&pixel1, sizeof(uint8_t), 1, encoded);
                }
            }
            fwrite(additional_pixels, sizeof(uint8_t), (true_width-width), encoded);
        }

        free(s);
        fclose(encoded);
    }


    //freeing the image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < 3; ++j) {
            free(image[j][i]);
        }
    }
    for (int i = 0; i < 3; ++i) {
        free(image[i]);
    }
    free(image);
    fclose(bmp);
    return 0;
}
