#include "functions.h"
#include "structures.h"

#define LENGTH  40

int main(int argc, char *argv[]) {

    if(argc == 3) {

    FILE *ini; //handler to ini file

    int first_key = 0; // this flag is needed because we have to distinguish beetwen adding first key to the section and adding other.
    argv++; //moving pointer to the name of the file
    ini = fopen(*argv, "r"); // opening the file
    argv++; //moving pointer to the argument to find
    //char to_find[256] = "slippery-balance.lonely-nature\0";
    //printf("To do %s\n", to_find);



    if(!ini){
        perror("Cant open ini file");
        return EXIT_FAILURE;
    }

    size_t length = LENGTH; //default max length of the string

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
                *(buf + strlen(buf) - 1) = '\0'; // cutting the ]

                if(check_alphanumeric(buf+1)) {
                    printf("Invalid value of the section, string %s contains none alphanumeric characters!", buf+1);
                    return 1;
                }

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
            //If we somehow failed to add the entry return -1
            if(add_new_entry(buf, curr, first_key) == -1) return 1;
            first_key = 0;
        }
    }
    //*****************************************
    //STRUCTURE OF INI FILE IS CREATED HERE
    //PERFORMING SEARCH OR ARITHMETIC OPERATION
    //*****************************************

    char operator = get_operator(*argv); // checking if we got expression or not
    //int length2 = 260;

    //if we didn't get expression operator variable contains 'x'
    if (operator != 'x'){
        //allocating memory for the left and right part of the expression
        char * left = (char *)malloc((strlen(*argv)+ 2) * sizeof(char));
        char * right = (char *)malloc((strlen(*argv)+ 2) * sizeof(char));

        //Splitting the expression using correct delimiter remember that we changed - to ^
        if(operator == '^') split(*argv, left, right, "^");
        else if (operator == '*') split(*argv, left, right, "*");
        else if (operator == '/') split(*argv, left, right, "/");
        else if (operator == '+') split(*argv, left, right, "+");
        else{
            printf("Invalid operator!");
            return 1;
        }
        //printf("%s %s\n", left, right );
        //Allocate memory for sections, keys and values
        char *left_sec = (char *)malloc((strlen(left)+ 2) * sizeof(char));
        char *left_key = (char *)malloc((strlen(left)+ 2) * sizeof(char));
        char *left_val = (char *)malloc(length * sizeof(char));
        char *right_sec = (char *)malloc((strlen(right) +2) * sizeof(char));
        char *right_key = (char *)malloc((strlen(right) +2) * sizeof(char));
        char *right_val = (char *)malloc(length * sizeof(char));


        //splitting the whole section.key pairs
        split(left, left_sec, left_key, ".");
        split(right, right_sec, right_key, ".");

        //printf("%s %s %s %s\n", left_sec, left_key, right_sec, right_key);
        //trying to find given section.key pairs
        if (!find_key(first_section, left_sec, left_key, left_val))  return 1;

        if (!find_key(first_section, right_sec, right_key, right_val)) return 1;


        //DOING ARITHMETICS

       //printf("%s %s\n", left_key, right_key);
       //If both are digits
       if (check_digit(left_val) && check_digit(right_val)){
           int left_operand = atoi(left_val);
           int right_operand = atoi(right_val);
           //printf("%d %d %d\n", left_operand, right_operand, left_operand *right_operand);
           switch (operator){
               case '^':
                   printf("%d", left_operand-right_operand);
                   break;
               case '+':
                   printf("%d", left_operand + right_operand);
                   break;
               case '*':
                   printf("%d", left_operand * right_operand);
                   break;
               case '/': {
                   if (right_operand == 0) {
                       printf("DIVISION BY ZERO!");
                       return 1;
                   } else printf("%d", left_operand / right_operand);
                   break;
               }
               default:
                   printf("Wrong operand!!!");
                   break;
           }
       }
        //if both operands are strings
       else if (!check_digit(left_val) && !check_digit(right_val)){
           if (operator == '+') {
               char * concatenated = (char *)malloc((strlen(left_val) + strlen(right_val) + 2) * sizeof(char));
               *concatenated = '\0'; // not to have any garbage characters inside concatenated
               strcat(concatenated, left_val);
               strcat(concatenated,right_val);
               printf("%s\n", concatenated);
               free(concatenated);
           }
           //if one try to do unsuported operation on strings
           else {
               printf("Invalid operation on string you can only use '+' operator!");
               return 1;
           }
       }
       //if both have different data type
       else {
           printf("Cannot do operation on string and int!");
           return 1;
       }

        //freeing
        free(left_val);
        free(right_val);
        free(left);
        free(right);
        free(right_sec);
        free(left_key);
        free(right_key);
        free(left_sec);
    }
    //if we dont have expression but just one key.value
    else{
        //at first check wheather we don't have any unsupported operators that may crash our program
        //then do the search
        if(search_for_invalid_operator(*argv)) {
            char *left_sec = (char *) malloc((strlen(*argv) + 2) * sizeof(char));
            char *left_key = (char *) malloc((strlen(*argv) + 2) * sizeof(char));
            char *left_val = (char *) malloc(length * sizeof(char));
            split(*argv, left_sec, left_key, ".");
            find_key(first_section, left_sec, left_key, left_val);
            free(left_val);
            free(left_key);
            free(left_sec);
        }
        else return 1;
    }


    //free bufor, file and whole section and entry structures.
    free(buf);
    fclose(ini);
    free_ini();

    //free(key_value);

    }
    //In case user passed too few arguments
    else {
        printf("Program accepts 2 additional arguments, name of the ini file and section.key string");
    }
    return 0;

}
