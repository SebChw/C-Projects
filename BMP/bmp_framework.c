#include "bmp_framework.h"

void to_bin(int a, char * s){
    int it = 0;
    //standard algorithm for coverting decimal representation to binary. Divide number by 2 until it becames 0 and every time save remainder
    while(a > 0){
        s[it] = a%2 + 48;
        ++it;
        a /= 2;
    }
    //additional padding becaouse each number should be represented in 1 byte
    while(it < 8){
        s[it] = '0';
        ++it;
    }
}

int to_num(char * s){
    int k = 1; //weight
    int ans = 0; //decimal number
    //iterate over string and sum up product of each bit and its weight
    for (int i = 0; i < strlen(s); ++i) {
        int tmp = s[i] - 48;
        ans += tmp * k;
        k *= 2;
    }

    return ans;
}

struct tagBITMAPFILEHEADER read_f_header(FILE * bmp){

    struct tagBITMAPFILEHEADER header; //create structure

    //printf("%llu\n", offsetof(struct tagBITMAPFILEHEADER, bfOffBits));
    //printf("%llu\n", sizeof(DWORD));
    //printf("%llu\n", sizeof(struct tagBITMAPFILEHEADER)); //size is 16 because size has to be divisible by sizeof largest member so we basically cannot just
    //read the whole  structure in one try because it may contains padding we need to read components one by one
    //fread(&BITMAPFILEHEADER, sizeof(struct tagBITMAPFILEHEADER), 1, bmp); // this will read exactly 16 bytes and won't work

    //read each header one by one, we cannot read whole header, as length of structure does not match length of whole header
    fread(&header.bfType, sizeof header.bfType, 1 , bmp);
    fread(&header.bfSize, sizeof header.bfSize, 1 , bmp);
    fread(&header.bfReserved1, sizeof header.bfReserved1, 1 , bmp);
    fread(&header.bfReserved2, sizeof header.bfReserved2, 1 , bmp);
    fread(&header.bfOffBits, sizeof header.bfOffBits, 1 , bmp);

    return header;

}
void print_f_header(struct tagBITMAPFILEHEADER BITMAPFILEHEADER) {
    unsigned char type[2]; //this array will contain two letters
    //get characters from bf Type
    type[0] = BITMAPFILEHEADER.bfType;
    type[1] = (BITMAPFILEHEADER.bfType >> 8u);

    //just print each structure member
    printf("BITMAPFILEHEADER: \n");
    printf("   bfType:\t0x%X (%c%c)\n", BITMAPFILEHEADER.bfType, type[0], type[1]);
    printf("   bfSize:\t%u\n", BITMAPFILEHEADER.bfSize);
    printf("   bfReserved1:\t0x%X\n", BITMAPFILEHEADER.bfReserved1);
    printf("   bfReserved2:\t0x%X\n", BITMAPFILEHEADER.bfReserved2);
    printf("   bfOffBits:\t%u\n", BITMAPFILEHEADER.bfOffBits);
}

struct tagBITMAPINFOHEADER read_i_header(FILE * bmp){
    struct tagBITMAPINFOHEADER BITMAPINFOHEADER;
    //reading bytes from bmp file now we can read whole structure at once as sizes match
    fread(&BITMAPINFOHEADER, sizeof(struct tagBITMAPINFOHEADER), 1, bmp);
    //printf("%lu\n", sizeof(BITMAPINFOHEADER));

    return BITMAPINFOHEADER;
}

int print_i_header(struct tagBITMAPINFOHEADER BITMAPINFOHEADER){

    //just printing every attribute of the structer and at the end checking whether file is supported
    printf("BITMAPINFOHEADER: \n");
    printf("   biSize: %u\n", BITMAPINFOHEADER.biSize);
    printf("   biWidth: %u\n", BITMAPINFOHEADER.biWidth);
    printf("   biHeight: %u\n", BITMAPINFOHEADER.biHeight);
    printf("   biPlanes: %u\n", BITMAPINFOHEADER.biPlanes);
    printf("   biBitCount: %u\n", BITMAPINFOHEADER.biBitCount);
    printf("   biCompression: %u\n", BITMAPINFOHEADER.biCompression);
    printf("   biSizeImage: %u\n", BITMAPINFOHEADER.biSizeImage);
    printf("   biXPelsPerMeter: %u\n", BITMAPINFOHEADER.biXPelsPerMeter);
    printf("   biYPelsPerMeter: %u\n", BITMAPINFOHEADER.biYPelsPerMeter);
    printf("   biClrUsed: %u\n", BITMAPINFOHEADER.biClrUsed);
    printf("   biClrImportant: %u\n", BITMAPINFOHEADER.biClrImportant);

    //checking if given bmp file is supported by our functions
    if(BITMAPINFOHEADER.biCompression != 0 || BITMAPINFOHEADER.biBitCount != 24){
        printf("Unsupported type of .bmp file\n");
        return 1; //file is not supported
    }
    return 0; //file is supported

}

uint8_t *** read_image_create_hist(struct tagBITMAPFILEHEADER BITMAPFILEHEADER, struct tagBITMAPINFOHEADER BITMAPINFOHEADER, FILE * bmp, int print_hist){

    //printf("Each row has : %u Pixels\n", ((BITMAPINFOHEADER.biBitCount * BITMAPINFOHEADER.biWidth +31)/32) * 4);
    //printf("Height: %u\n", BITMAPINFOHEADER.biHeight);

    //In almost every function we need those 3 values.
    int width = ((BITMAPINFOHEADER.biBitCount * BITMAPINFOHEADER.biWidth +31)/32) * 4 ; //amount of pixel + padding in a row
    int height = BITMAPINFOHEADER.biHeight;
    int true_width = BITMAPINFOHEADER.biWidth * 3; // real amount of pixel in a row
    //printf("%d, %d ,%d, %d\n", width, height, true_width, BITMAPINFOHEADER.biWidth);
    int Histogram[3][16]; // 0 - blue, 1- green 2-red as order is reversed while reading

    //alocating the image
    uint8_t ***image = malloc(sizeof(uint8_t **) * 3 ); //alocating 3 **pointers for each colour
    //for each colour alocating memory for each row and for each row allocate memory for each column
    for (int i = 0; i < 3; ++i) {
        image[i] = malloc(sizeof(uint8_t *) * height);
        for (int j = 0; j < height; ++j) {
            image[i][j] = calloc(true_width, sizeof(uint8_t));
        }
    }

    uint8_t pixel; //each pixel is stored as 1byte number

    //in histogram we will count amount of pixels in some interval
    for (int i = 0; i < 3; ++i) {
        memset(Histogram[i], 0, 16*sizeof(int));
    }

    //we need to get to the correct place to start reading pixels
    fseek(bmp, BITMAPFILEHEADER.bfOffBits, SEEK_SET); //file position indicator will be moved at bffOffBist byte

    //iterating over height and width of the image
    for (int i = 0; i < height; ++i) {
        int cnt = 0; //this will store amount of pixels we read in each row
        for (int j = 0; j < width; ++j) {
            //read pixel by pixel in each row
            fread(&pixel, sizeof(pixel), 1, bmp);

            Histogram[j%3][pixel/16] += 1; //increment correct Histogram member
            image[j%3][i][cnt] = pixel; //save pixel in the correct place, j%3 preser ensure that we will read blue then green then red and so on

            if (j%3 == 2) cnt++; //if we read 1 pixel which is build of three values we increment counter
            if(j >= true_width) break; //if we read all pixels and there is additional padding still don't do anything

            // printf("%u\n", pixel);
        }

    }
    //if user wants to print histogram we do it
    if (print_hist) {
        for (int i = 2; i > -1; --i) {
            if (i== 0) printf("BLUE: \n");
            else if (i == 1) printf("GREEN: \n");
            else printf("RED: \n");
            for (int j = 0; j < 16; ++j) {
                printf("   %d-%d: %.2f%%\n", j*16, (j+1)*16-1, (double)(Histogram[i][j])/(true_width*height/3)*100);
            }
        }
    }

    return image;

}

void create_greyscale(char * filename, struct tagBITMAPFILEHEADER BITMAPFILEHEADER,
                      struct tagBITMAPINFOHEADER BITMAPINFOHEADER, FILE * bmp, uint8_t ***image){
    FILE *greyscale = fopen(filename, "wb");


    //again we need those tree parameters
    int width = ((BITMAPINFOHEADER.biBitCount * BITMAPINFOHEADER.biWidth +31)/32) * 4 ; //amount of pixel + padding in a row
    int height = BITMAPINFOHEADER.biHeight;
    int true_width = BITMAPINFOHEADER.biWidth * 3; // real amount of pixel in a row

    //read everything as it is from
    fwrite(&BITMAPFILEHEADER.bfType, sizeof BITMAPFILEHEADER.bfType, 1 , greyscale);
    fwrite(&BITMAPFILEHEADER.bfSize, sizeof BITMAPFILEHEADER.bfSize, 1 , greyscale);
    fwrite(&BITMAPFILEHEADER.bfReserved1, sizeof BITMAPFILEHEADER.bfReserved1, 1 , greyscale);
    fwrite(&BITMAPFILEHEADER.bfReserved2, sizeof BITMAPFILEHEADER.bfReserved2, 1 , greyscale);
    fwrite(&BITMAPFILEHEADER.bfOffBits, sizeof BITMAPFILEHEADER.bfOffBits, 1 , greyscale);
    fwrite(&BITMAPINFOHEADER, sizeof BITMAPINFOHEADER,1 , greyscale);

    int padding = BITMAPINFOHEADER.biSize - 40; // to know how many -0 bits we have to add, as header has size of 40, and real pixels may start later we have to
    //add additional zeroes

    uint8_t pad[padding];
    memset(pad,0,sizeof(uint8_t) * padding); //just set all needed bytes to zero
    fwrite(pad, sizeof(uint8_t) ,padding , greyscale);
    fseek(bmp, BITMAPFILEHEADER.bfOffBits, SEEK_SET); // moving pointer at the beggining of pixels
    //Making greyscale
    uint8_t additional_pixels[abs(width-true_width)]; // additional padding
    memset(pad,0,sizeof(uint8_t) * abs(width-true_width)); //setting it to zero
    for (int i = 0; i < height; ++i) {
        //we need to divide by 3 as we write 3 bytes at once
        for (int j = 0; j < (int)(true_width/3); ++j) {
            uint8_t greypixel = (unsigned int)(image[0][i][j] + image[1][i][j] + image[2][i][j])/3; //calculating average of pixels
            fwrite(&greypixel, sizeof(uint8_t), 1, greyscale); //writing red green blue to be the same value
            fwrite(&greypixel, sizeof(uint8_t), 1, greyscale);
            fwrite(&greypixel, sizeof(uint8_t), 1, greyscale);
        }
        fwrite(additional_pixels, sizeof(uint8_t), abs(width-true_width), greyscale); //write additional padding
    }
    fclose(greyscale); //close the file

}

void decode_Steganography(uint8_t ***image, struct tagBITMAPINFOHEADER BITMAPINFOHEADER){

    int width = ((BITMAPINFOHEADER.biBitCount * BITMAPINFOHEADER.biWidth +31)/32) * 4 ; //amount of pixel + padding in a row
    int height = BITMAPINFOHEADER.biHeight;
    int true_width = BITMAPINFOHEADER.biWidth * 3; // real amount of pixel in a row

    int to_decode; // here we will store how many characters we need to read
    int n_bit = 0, n_sgn = 0; // n_bit - how many bits we read already, after reading 8 bits we will reset it to zero
    // n_sgn -  how many characters we already decoded
    char * s = malloc(sizeof(char) * 9); //we will store each char in this array build of ones and zeroes
    char * decoded;
    for (int i = 0; i < 8; ++i) {
        s[i] = '0'; //at first set all characters to be zero
    }
    s[8] = '\0'; // terminate the string
    //printf("%s\n", s);
    //iterate over image to read from it
    for (int i = 0; i < height; ++i) {
        //here we divide true_width by 3 because we read 3 pixels at once
        for (int j = 0; j < (int) (true_width / 3); ++j) {
            //read red green and blue pixel
            for (int k = 0; k < 3; ++k) {
                uint8_t pixel2 = image[k][i][j]; //reading current pixel
                //here if we haven't read any signs yet we try to read amount
                if (n_sgn == 0) {
                    s[n_bit] = pixel2 & 1 + 48; // we using binary and operator to get only the least significant bit and add 48 to have digit either 0 or 1 in ASCII
                    ++n_bit;
                    //if we've read 8 bits it means we should decode whole byte and start reading next
                    if (n_bit == 8) {
                        ++n_sgn; //we increment counter of read signs
                        to_decode = to_num(s); // to_num changes string binary representation to decimal
                        //printf("%s %d\n", s, to_decode);
                        n_bit = 0; //reset our read bit counter
                        decoded = malloc(sizeof(char) * (to_decode + 1)); //allocating memory needed to store the whole string
                        // we need to decode one more character for terminating char
                    }
                } else if (n_sgn <= to_decode) {
                    //now we read untill there still is something more to decode
                    s[n_bit] = pixel2 & 1 + 48; //again we get just the least significant bit
                    ++n_bit;
                    if (n_bit == 8) {
                        //here we change binary to decimal and then insert character at the correct position
                        int dec_char = to_num(s);
                        // printf("%s %d\n", s, to_decode);
                        decoded[n_sgn - 1] = (char) (dec_char);
                        ++n_sgn;
                        n_bit = 0;
                    }
                }
            }
        }
        if (n_sgn > to_decode) break; //if we read all needed characters we break from the loop
    }
    //after reading whole message we terminate it and print
    decoded[to_decode] = '\0'; // need to terminate this char
    printf("Decoded message:\n%s", decoded);
    free(decoded);
    free(s);

}

void encode_Steganography(char * filename, char * encode_message, struct tagBITMAPFILEHEADER BITMAPFILEHEADER,
                          struct tagBITMAPINFOHEADER BITMAPINFOHEADER, FILE * bmp, uint8_t ***image) {
    FILE *encoded = fopen(filename, "wb"); //we create file in which we will encode message

    //again we need these three parameters
    int width = ((BITMAPINFOHEADER.biBitCount * BITMAPINFOHEADER.biWidth +31)/32) * 4 ; //amount of pixel + padding in a row
    int height = BITMAPINFOHEADER.biHeight;
    int true_width = BITMAPINFOHEADER.biWidth * 3; // real amount of pixel in a row

    //we rewrite all headers
    fwrite(&BITMAPFILEHEADER.bfType, sizeof BITMAPFILEHEADER.bfType, 1 , encoded);
    fwrite(&BITMAPFILEHEADER.bfSize, sizeof BITMAPFILEHEADER.bfSize, 1 , encoded);
    fwrite(&BITMAPFILEHEADER.bfReserved1, sizeof BITMAPFILEHEADER.bfReserved1, 1 , encoded);
    fwrite(&BITMAPFILEHEADER.bfReserved2, sizeof BITMAPFILEHEADER.bfReserved2, 1 , encoded);
    fwrite(&BITMAPFILEHEADER.bfOffBits, sizeof BITMAPFILEHEADER.bfOffBits, 1 , encoded);
    fwrite(&BITMAPINFOHEADER, sizeof BITMAPINFOHEADER,1 , encoded);

    int padding = BITMAPINFOHEADER.biSize - 40; // to know how many -0 bits we have to add, as header has size of 40, and real pixels may start later we have to
    //add additional zeroes

    uint8_t pad[padding]; //we write as many zeroes as needed to the file
    memset(pad,0,sizeof(uint8_t) * padding);
    fwrite(pad, sizeof(uint8_t) ,padding , encoded);

    fseek(bmp, BITMAPFILEHEADER.bfOffBits, SEEK_SET); // moving pointer at the beggining of pixels

    uint8_t additional_pixels[abs(width-true_width)]; //create needed padding
    memset(pad,0,sizeof(uint8_t) * abs(width-true_width));


    int length = strlen(encode_message); //to know how many characters we need to encode
    char * s = malloc(sizeof(char) * 9); //we will store each char in this array build of ones and zeroes
    for (int i = 0; i < 8; ++i) {
        s[i] = '0'; //at first set everything to zero
    }
    to_bin(length, s); // now we have length of the string encoded in variable s
    int to_encode = length + 1; //we have to encode one additional character, length of the string that is encoded
    int n_bit = 0, n_sgn = 0; //current bit to be encoded, current char form string to be encoded

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < (int)(true_width/3); ++j) {
            for (int k = 0; k < 3; ++k) {
                uint8_t pixel1 = image[k][i][j];
                if(n_sgn < to_encode){
                    // printf("%d %d %s \t%d\n", n_sgn, n_bit, s, pixel1);
                    if(s[n_bit] == '0')
                        pixel1 &= 254; //if last bit need to be set to zero we do binary AND operation, all bits from 1-7 will be as they are but last will be zeroed
                    else
                        pixel1 |= 1; //if last bit need to be set to one we do binary OR operation, all bits from 1-7 will be as they are but last will become one for sure
                    // printf("Pixel = %d\n", pixel1);
                    ++n_bit;
                    if(n_bit == 8){
                        //if we written 8 bits it means that we have encoded one character and we can proceed to the next one
                        n_bit = 0;
                        ++n_sgn;
                        to_bin(encode_message[n_sgn-1], s);
                    }
                }
                fwrite(&pixel1, sizeof(uint8_t), 1, encoded); //we write each pixel
            }
        }
        fwrite(additional_pixels, sizeof(uint8_t), (abs(width-true_width)), encoded); //at the end write padding
    }

    free(s);
    fclose(encoded);
}

void free_image(uint8_t ***image, struct tagBITMAPINFOHEADER BITMAPINFOHEADER) {
    int height = BITMAPINFOHEADER.biHeight;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < 3; ++j) {
            free(image[j][i]); //fre each row
        }
    }
    for (int i = 0; i < 3; ++i) {
        free(image[i]); //fre each column
    }
    free(image); //free colours
}