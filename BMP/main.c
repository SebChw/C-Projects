#include <stdio.h>
#include <stdint.h>


#include "bmp_framework.h"

int main(int argc, char *argv[]) {
    // Check whether we have enough amount of arguments
    if(argc < 2 || argc > 4){
        printf("Wrong number of parameters\n");
        return 1;
    }
    //opening the file
    FILE * bmp = fopen(argv[1], "rb") ; // file has to be open in binary mode in order to read anything from it
    //checking if opening was succesfull
    if (!bmp) {
        printf("Failed to open the file!\n");
        return 1;
    }

    //*******************
    //READING FIRST HEADER
    //*******************
    //just using functions
    struct tagBITMAPFILEHEADER BITMAPFILEHEADER = read_f_header(bmp);
    print_f_header(BITMAPFILEHEADER);

    //*******************
    //READING SECOND HEADER
    //*******************
    //just using functions
    struct tagBITMAPINFOHEADER BITMAPINFOHEADER = read_i_header(bmp);
    if(print_i_header(BITMAPINFOHEADER) == 1) return 1; //this functions return int saying whether a bmp files is supported or not

    // GETTING MATRIX REPRESENTING THE IMAGE with function and also printing histogram
    uint8_t ***image = read_image_create_hist(BITMAPFILEHEADER ,BITMAPINFOHEADER, bmp , 1);

    //If user passed only one argument we ask if they want to decode steganography
    if(argc == 2){
        printf("Decode steganography? [Y/n]\n");
        char decision;
        decision = (char)getchar(); //getting character from the user
        if(decision == 'Y' || decision == 'y'){
            decode_Steganography(image, BITMAPINFOHEADER);
        }
    }

    //if user passed 3 arguments we use function that creates greyscale copy of it
    if(argc == 3)
        create_greyscale(argv[2], BITMAPFILEHEADER, BITMAPINFOHEADER,bmp, image);

    //if user passed 4 arguments we use function that encode steganography
    if(argc == 4) encode_Steganography(argv[2], argv[3], BITMAPFILEHEADER, BITMAPINFOHEADER,bmp, image);

    //freeing the image
    free_image(image, BITMAPINFOHEADER);
    //closing the file
    fclose(bmp);
    return 0;
}
