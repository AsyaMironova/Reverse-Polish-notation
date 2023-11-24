#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_module.h"
#include "polish.h"
#include "stack.h"
#define WIDTH 80
#define HEIGHT 25

void distrib(char *expression) {
    double max_x = 4 * M_PI;
    double delta_x = max_x / 80;
    double delta_y = 2.0 / 25;
    double y = 0;
    char *arr = (char *)malloc(WIDTH * HEIGHT * sizeof(char));
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        arr[i] = '.';
    }
    for (double x = 0; x < max_x; x += delta_x) {
        char *temp = (char *)malloc(sizeof(char) * strlen(expression));
        strcpy(temp, expression);
        calculate(temp, x, &y);
        int i_x = (int)(x / delta_x);
        int i_y = (int)(y / delta_y) + 12;
        printf("x = %f, y = %f\ni_x = %d, i_y = %d\n", x, y, i_x, i_y);
        arr[i_y * WIDTH + i_x] = '*';
        free(temp);
    }
    draw(arr, WIDTH, HEIGHT);
    free(arr);
}

int main() {
    char *str = NULL;
    int str_len = 0;
    if (input(&str, &str_len) == -1) {
        printf("Error reading input data");
        return -1;
    }
    char *numbers = NULL;
    struct node *operators = NULL;
    if (!parse(str, &numbers, &operators)) {
        printf("Cannot parse the string\n");
        return -1;
    }
    distrib(numbers);
    destroy(&operators);
    if (str != NULL) {
        free(str);
    }
    if (str != NULL) {
        free(numbers);
    }
    return 0;
}