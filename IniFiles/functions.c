#include "functions.h"


char *trim(char *str)
{
    size_t len = 0; //length of the string
    char *frontp = str; //pointer at the beginning of the string
    char *endp = NULL; // pointer at the end of the string

    if( str == NULL ) { return NULL; } //If the string is Null we return Null
    if( str[0] == '\0' ) { return str; } //If the string is empty we also return it

    len = strlen(str); //getting length of str
    endp = str + len; //setting the endp pointer at the end of the string, now it is set before \0 sign

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp) ) { ++frontp; } //while current char is a space we moving forward our pointer
    if( endp != frontp ) // this condition will be met if our string contains chars different than white-spaces
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}  //we move our pointer until we have spaces or we met front pointer
    }

    if( frontp != str && endp == frontp ) // if we moved front pointer and both pointers've met string contains only white-spaces
        *str = '\0';
    else if( str + len - 1 != endp ) //if we moved end pointer we set null character right after it
        *(endp + 1) = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */

    endp = str;
    if( frontp != str )
    {
        while( *frontp ) { *endp++ = *frontp++; }
        *endp = '\0';
    }

    return str;
}

int check_alphanumeric(char *str){
    size_t len = strlen(str); //get length of the string
    //iterate over string
    for (size_t i = 0; i < len ; ++i) {
        if(!isalnum(*(str+i))) {
            if (*(str+i) != '-')return 1;
        }
    }
    return 0;
}
char get_operator(char *str){

    size_t len = strlen(str);
    for (size_t i = 0; i < len ; ++i) {
        // if we find one of supported operators we return it
        if(*(str+i) == '*' || *(str+i) == '/' || (*(str+i) == '-' && *(str+i+1) == ' ') || *(str+i) == '+') {
            if (*(str+i) == '-') *(str+i) = '^';
            return *(str+i);
        }
    }
    return 'x'; // if no operator was found
}
int search_for_invalid_operator(char * str){
    size_t len = strlen(str);
    for (size_t i = 0; i < len ; ++i) {
        if(!isalnum(*(str+i))) {
            //if we have none alphanumerical character different from those supported we raise an error
            if (*(str+i) == '-' || *(str+i) == '.' || *(str+i) == ' ') continue;
            else {
                printf("Query contains some forbidden alphanumeric characters! %c", *(str +i));

                return 0;
            }
        }
    }
    return 1 ; // if everything is fine
}

void split(char *buf, char *a, char*b, char *delim){
    char * token = strtok(buf, delim);
    trim(token); //trim token in case someone put some whitespaces
    strcpy(a, token); //copy left part to a
    token = strtok(NULL, delim);
    strcpy(b,token); //copy right part to b

}
int check_digit(char *str){
    size_t len = strlen(str);
    //scan whole string and check if it contains only digits if it finds one none digit characters returns 0
    for (size_t i = 0; i < len ; ++i)
        if(!isdigit(*(str+i))) return 0;

    return 1;
}