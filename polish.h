#ifndef POLISH_H
#define POLISH_H
#define FUNCTIONS_SIZE 6
#include "stack.h"

char *get_number_string(char *input);
char *get_alpha_string(char *input);
int add_number(char *input, char **expression);
int add_alpha(char *input, char **expression, struct node **stack);
int add_from_stack(char **expression, struct node **stack);
int is_number(char input);
int is_alpha(char input);
int is_operator(char input);
int parse(char *input, char **numbers, struct node **stack);
int calculate(char *expression, double x, double *result);

#endif