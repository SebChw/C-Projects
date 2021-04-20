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
struct section{
    char * name;
    struct key_value * entry;
    struct section * next;
};



struct section * add_new_section(char * buf, int * number_of_sections, struct  section * first_section); //function that adds new section
int add_new_entry(char *buf, struct section* s); //function that adds another entry to section s
int free_ini(struct  section * first_section); //function that free memory of all sections and corresponding entry list
int find_key(struct section * head, char * section, char * key, char * target); // function that search for a given section.key pair and copies the outcomes to target;
#endif
