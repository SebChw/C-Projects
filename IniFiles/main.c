#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
struct key_value{
    char * key;
    char * value;
};
struct section{
    struct key_value * entry;
};

int main(int argc, char *argv[]) {

//    if(argc > 2) {
//
//        char *key_value = (char *) malloc((strlen(argv[2])+1) * sizeof(char));
//        key_value = strcpy(key_value, argv[2]);
//        char * key = strtok(key_value, ".");
//        char * value = strtok(NULL, ".");
//        printf("%s\n%s\n", key, value);

        FILE *ini;
        ini = fopen("file.ini", "r");

        if(!ini){
            perror("Cant open ini file");
            return EXIT_FAILURE;
        }

        size_t length = 10; //default max length of the string
        char * buf = (char *)malloc(length * sizeof(char));

        if(!buf){
            printf("Failed to allocate memory");
            return -2;
        }

       while(fgets(buf, length, ini) != NULL) {

           while( *(buf+strlen(buf)-1) != '\n' && *(buf+strlen(buf)-1) != '\r' ){
               buf = realloc(buf, 2 * length * sizeof(char));
               if(!buf){
                   printf("Error with reallocating memory!\n");
                   fclose(ini);
                   return -3;
               }
               char * tmp = malloc(length * sizeof(char));

                if (tmp) {
                    fgets(tmp, length, ini); // we try to
                    strcat(buf, tmp);
                    length *= 2; // we increase capacity

                }
                else{
                    printf("Error with reallocating memory!\n");
                    fclose(ini);
                    return -3;
                }
                free(tmp);
           }
           printf("%s",buf);
       }
       free(buf);
       fclose(ini);


        //free(key_value);

    //}
return 0;

}
