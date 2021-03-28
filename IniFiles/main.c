#include "functions.h"
#include "structures.h"


int main(int argc, char *argv[]) {

//    if(argc > 2) {
//
//        char *key_value = (char *) malloc((strlen(argv[2])+1) * sizeof(char));
//        key_value = strcpy(key_value, argv[2]);
//        char * key = strtok(key_value, ".");
//        char * value = strtok(NULL, ".");
//        printf("%s\n%s\n", key, value);

    FILE *ini; //handler to ini file

    int first_key = 0; // this flag is needed because we have to distinguish beetwen adding first key to the section and adding other.
    ini = fopen("file.ini", "r"); // opening the file

    if(!ini){
        perror("Cant open ini file");
        return EXIT_FAILURE;
    }

    size_t length = 40; //default max length of the string

    char * buf = (char *)malloc(length * sizeof(char)); // alocating memory for our string

    if(!buf){
        printf("Failed to allocate memory");
        return -2;
    }


    int number_of_sections = 0; //when we add first section the bahaviour is quite different

    //we begin to read file content and do this until it have some
    while(fgets(buf, length, ini) != NULL) {
        //If length of the line is greater than expected we need to expand our buffor for string
        while( *(buf+strlen(buf)-1) != '\n' && *(buf+strlen(buf)-1) != '\r' ){
            buf = realloc(buf, 2 * length * sizeof(char));
            if(!buf){
                printf("Error with reallocating memory!\n");
                fclose(ini);
                return -3;
            }
            char * chartmp = malloc(length * sizeof(char)); //we create temporary char not to overwrite our buffor
            //as we expanded bufor 2 times we need to get only length more characters to fill it fully

            if (chartmp) {
                fgets(chartmp, length, ini); // we again read from file
                strcat(buf, chartmp); //we concatenate our buffor with temp char and now it is fully filled
                length *= 2; // we increase capacity

            }
            else{
                printf("Error with reallocating memory!\n");
                fclose(ini);
                return -3;
            }
            free(chartmp); // we need to free this temporary string
        }
        buf = trim(buf); // to get rid of any whitespace characters
        //functions is defined in functions.c
        //printf("%s\n", buf);

        //if first character after trimming is '[' then is should be section
        if(*buf == '['){
            //if last character is not ] it means that we have invalid ini file
            if(*(buf + strlen(buf)-1) ==']') {
                number_of_sections = add_new_section(buf, number_of_sections); //add new section
                if (number_of_sections > 1) curr = curr->next; // this is needed because we pass that curr pointer to add_new_entry function

                first_key = 1; //now next non empty line should be the key_value pair
                if(number_of_sections == -1) return -1; // if somehow add_new_section function failed it return -1


            }
            else{
                printf("Invalid INI file, check if all sections have ']' at the end");
                return -1;
            }

        }
        //if first line is ; it means this is a comment and if it is empty char there is nothing to process
        else if(*buf == ';' || *buf == '\0') continue;
        else {
            //we add new entry and set flag to 0, as from now we should expect more keys or new section
            add_new_entry(buf, curr, first_key);
            first_key = 0;
        }



    }
    //free bufor, file and whole section and entry structures.
    free(buf);
    fclose(ini);
   free_ini();

    //free(key_value);

    //}
    return 0;

}
