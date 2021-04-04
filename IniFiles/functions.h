#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef INI_FUNCTIONS_H
#define INI_FUNCTIONS_H


char *trim(char *str); //function to get rid of heading and trailing whitespaces
int check_alphanumeric(char *str); //function that checks if string contains some none alphanumeric characters
char get_operator(char *str); // function that returns operator, it additionally change - sign into ^ since key or values may containt hyphens
void split(char *buf, char *a, char*b, char *delim); //function tak takes string separated with delimiter and copy content of the left to a and of the right to b
int check_digit(char *str); // function that chekcs wheather text contains only digits
int search_for_invalid_operator(char * str); // function that check if maybe we have some unsupported operators
#endif