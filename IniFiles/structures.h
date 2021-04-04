#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef STRUCTURES_H
#define STRUCTURES_H


//we store all keys in every section as a linked list
struct key_value{
    char * key;
    char * value;
    struct key_value *next;
};
//we store sections in linked list
//we create first_section that is the head of the linked list
struct section{
    char * name;
    struct key_value * entry;
    struct section * next;
}*first_section;

struct section *next_section, *tmp; // we need those two pointers to add another section
struct section *curr; // we need this to know to which sections we have to add keys
struct key_value *keytmp; //we need this to free the keys

int add_new_section(char * buf, int number_of_sections); //function that adds new section
int add_new_entry(char *buf, struct section* s, int first); //function that adds another entry to section s
int free_ini(); //function that free memory of all sections and corresponding entry list
int find_key(struct section * head, char * section, char * key, char * target);
#endif
