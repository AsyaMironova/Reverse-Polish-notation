#ifndef IO_MODULE_H
#define IO_MODULE_H

int realloc_str(char **str, int new_size);
int append_string(char **str, int *size, char new_char);
int input(char **str, int *size);
void draw(char *array, int width, int height);

#endif