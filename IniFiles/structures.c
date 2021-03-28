#include "structures.h"
#include "functions.h"

int add_new_section(char * buf, int number_of_sections){
    //if we havent added any section yet we do it differently
    if (number_of_sections == 0){
        first_section = (struct section *)malloc(sizeof(struct section)); //allocate memory for the section
        if (first_section == NULL){
            return -1;
        }
        first_section->name = malloc((strlen(buf)) * sizeof(char)); //allocate memory for the name of the section

        strcpy(first_section->name, buf+1); // start copying from buf+1 not to copy [
        *(first_section->name + strlen(first_section->name) - 1) = '\0'; //terminate string one character earlier not co contain ]
        first_section->next = NULL; //set next section to the NULL pointer since we don't know if there will be more sections
        tmp = first_section; // set tmp pointer to the first section
        //printf("%s",first_section->name);
        number_of_sections++; //increase the counter of sections
        curr = first_section; //set curr pointer to the first section
    }
    else{
        next_section = (struct section *)malloc(sizeof(struct section)); //now we allocate memory for the new section
        if (next_section == NULL){
            return -1;
        }
        next_section->name = malloc((strlen(buf))* sizeof(char));
        strcpy(next_section->name, buf+1);
        *(next_section->name + strlen(next_section->name) - 1) = '\0';
        next_section-> next = NULL; //there is no more section after the one created
        number_of_sections++;
        tmp->next = next_section; //set the next of the predecessor to created section

        tmp = tmp->next; // set tmp to point to the last section created
    }

    return number_of_sections;
    //printf("%s\n",buf);
}

int add_new_entry(char *buf, struct section* s, int first){
    //create new entry
    struct key_value* entry = (struct key_value*)malloc(sizeof(struct key_value));
    if(!entry) {
        printf("%s", "Failed to allocate memory!");
        return -1;
    }
    //if this is the first entry we just add it to the section
    if (first == 1){
        s->entry = entry;
        entry->next = NULL;
    }
    //if this is not the first entry we add this at the end of the linked list
    else{
        keytmp = s->entry;
        while (keytmp -> next != NULL) {
            keytmp = keytmp ->next;
        }
        keytmp->next = entry;
        entry->next = NULL;
    }
    //now we have to split key value pair delimiter is = sign
    char * token = strtok(buf, "=");
    trim(token); //trim token in case someone put some whitespaces
    entry ->key = malloc((strlen(token) + 1) * sizeof(char)); //allocate memory for the key
    if(!entry->key) {
        printf("%s", "Failed to allocate memory!");
        return -1;
    }
    strcpy(entry->key, token); //copy content of the key
    //do exactly the same for the value
    token = strtok(NULL, "=");
    trim(token);
    entry -> value = malloc((strlen(token) + 1) * sizeof(char));
    if(!entry->value) {
        printf("%s", "Failed to allocate memory!");
        return -1;
    }
    strcpy(entry->value, token);
   // printf("%s  ", s->entry->key);
    //printf("%s\n", s->entry->value);

    return 0;
}

int free_ini(){
    //while there are some section to free
    while (first_section != NULL)
    {
        tmp = first_section; //tmp points to the first section

        printf("%s\n", first_section->name); //debugging print
        //while there are some entries in current section to free
        while(first_section->entry != NULL){
            keytmp = first_section->entry; //tmp point to the entry we want to free
            printf("%s  %s\n", first_section->entry->key ,first_section->entry->value); //debugging print
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