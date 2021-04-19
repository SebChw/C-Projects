#include "structures.h"
#include "functions.h"

struct section * add_new_section(char * buf, int * number_of_sections, struct  section * first_section){
    //if we havent added any section yet we do it differently
    if (*number_of_sections == 0){
        first_section = (struct section *)malloc(sizeof(struct section)); //allocate memory for the section
        if (first_section == NULL){
            return NULL;
        }
        first_section->name = malloc((strlen(buf)) * sizeof(char)); //allocate memory for the name of the section

        strcpy(first_section->name, buf+1); // start copying from buf+1 not to copy [
        //Already doing this with buffor
        //*(first_section->name + strlen(first_section->name) - 1) = '\0'; //terminate string one character earlier not co contain ]
        first_section->next = NULL; //set next section to the NULL pointer since we don't know if there will be more sections
        //tmp = first_section; // set tmp pointer to the first section
        //printf("%s",first_section->name);
        (*number_of_sections)++; //increase the counter of sections

    }
    else{
        struct section * next_section = (struct section *)malloc(sizeof(struct section)); //now we allocate memory for the new section
        if (next_section == NULL){
            return NULL;
        }
        next_section->name = malloc((strlen(buf))* sizeof(char));
        strcpy(next_section->name, buf+1);
        //Already doing this in main
        //*(next_section->name + strlen(next_section->name) - 1) = '\0';
        next_section-> next = NULL; //there is no more section after the one created
        (*number_of_sections)++;
        struct section * tmp = first_section;
        while(tmp->next != NULL) tmp = tmp->next;
        tmp->next = next_section; //set the next of the predecessor to created section

        //tmp = tmp->next; // set tmp to point to the last section created
    }

    return first_section;
    //printf("%s\n",buf);
}

int add_new_entry(char *buf, struct section* s, int first){
    //At first check if string are valid
    char * token = strtok(buf, "=");
    trim(token); //trim token in case someone put some whitespaces
    if(check_alphanumeric(token)) {
        printf("Invalid value of the key, string %s contains none alphanumeric characters!", token);
        return -1;
    }

    char * token2 = strtok(NULL, "=");
    trim(token2);

    if(check_alphanumeric(token2)) {
        printf("Invalid value of the key, string %s contains none alphanumeric characters!", token2);
        return -1;
    }


    //create new entry
    struct key_value* entry = (struct key_value*)malloc(sizeof(struct key_value));
    if(!entry) {
        printf("%s", "Failed to allocate memory!");
        return -1;
    }
    //if this is the first entry we just add it to the section as a head
    if (first == 1){
        s->entry = entry;
        entry->next = NULL;
    }
    //if this is not the first entry we add this at the end of the linked list
    else{
        struct key_value * keytmp;
        keytmp = s->entry;
        while (keytmp -> next != NULL) {
            keytmp = keytmp ->next;
        }
        keytmp->next = entry;
        entry->next = NULL;
    }



    entry ->key = malloc((strlen(token) + 1) * sizeof(char)); //allocate memory for the key

    if(!entry->key) {
        printf("%s", "Failed to allocate memory!");
        return -1;
    }
    strcpy(entry->key, token); //copy content of the key
    //do exactly the same for the value

    entry -> value = malloc((strlen(token2) + 1) * sizeof(char));
    if(!entry->value) {
        printf("%s", "Failed to allocate memory!");
        return -1;
    }
    strcpy(entry->value, token2);
   // printf("%s  ", s->entry->key);
    //printf("%s\n", s->entry->value);

    return 0;
}
int find_key(struct section * head, char * section, char * key, char * target){
    struct section * temporary = head; //pointer to first section
    //just do linear search in section linked list
    while(temporary != NULL){
        if (strcmp(temporary->name, section) == 0){
            struct key_value * temporary2 = temporary->entry;
            //another linear search but now in linked list of entries
            while(temporary2 != NULL){
                if (strcmp(temporary2->key, key) == 0){
                    printf("%s\n",temporary2->value);
                    strcpy(target, temporary2->value);
                    return 1;
                }
                temporary2 =temporary2->next;
            }
            //if we got at the end of entries list print error
            if (temporary2 == NULL) {
                printf("Failed to find key \"%s\"  in section [%s]", key, section);
                return 0;
            }
        }
        temporary = temporary->next;
    }
    //if we got at the end of section list print error
    printf("Failed to find section [%s]", section);
    return 0;
}


int free_ini(struct  section * first_section){
    //while there are some section to free
    while (first_section != NULL)
    {
        struct section *tmp;
        tmp = first_section; //tmp points to the first section

        //printf("[%s]\n", first_section->name); //debugging print
        //while there are some entries in current section to free
        while(first_section->entry != NULL){
            struct key_value * keytmp;
            keytmp = first_section->entry; //tmp point to the entry we want to free
            //printf("%s = %s\n", first_section->entry->key ,first_section->entry->value); //debugging print
            first_section->entry = first_section->entry->next; //we move to the next entry
            free(keytmp ->value); //free value
            free(keytmp->key); //free key
            free(keytmp); //fre whole entry
        }

        first_section = first_section->next; //we move to the next section
        free(tmp->name); //free name
        free(tmp); //free whole section
    }


    return 1;
}