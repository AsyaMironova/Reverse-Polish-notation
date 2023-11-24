#include "io_module.h"

#include <stdio.h>
#include <stdlib.h>

int realloc_str(char **str, int new_size) {
    char *tmp = (char *)realloc((*str), new_size);

    if (NULL == tmp) {
        free(tmp);
        return -1;
    }
    *str = tmp;
    return 0;
}

int append_string(char **str, int *size, char new_char) {
    if (realloc_str(str, ++(*size) * sizeof(char))) {
        printf("something wrong\n");
        return -1;
    }
    (*str)[(*size) - 1] = new_char;
    return 0;
}

int input(char **str, int *size) {
    int ch = 0;
    while ((ch = getc(stdin)) != '\n') {
        if (append_string(str, size, ch) == -1) return -1;
    }
    if (append_string(str, size, '\0') == -1) return -1;
    return 0;
}

void draw(char *array, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c", array[i * width + j]);
            if (j + 1 < width) printf(" ");
        }
        if (i + 1 < height) printf("\n");
    }
}