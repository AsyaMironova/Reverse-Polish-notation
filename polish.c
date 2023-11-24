#include "polish.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_module.h"

double ctg(double x) { return 1 / tan(x); }

static const char *function_names[FUNCTIONS_SIZE] = {"sin", "cos", "tan", "ctg", "sqrt", "ln"};
double (*functions[FUNCTIONS_SIZE])(double x) = {sin, cos, tan, ctg, sqrt, log};

double execute_function(int func_number, double value) { return functions[func_number](value); }

double execute_operation(char *cur_elem, double first, double second) {
    int add = !strcmp("+", cur_elem);
    int sub = !strcmp("-", cur_elem);
    int mul = !strcmp("*", cur_elem);
    int div = !strcmp("/", cur_elem);
    if (add) {
        return first + second;
    } else if (sub) {
        return first - second;
    } else if (mul) {
        return first * second;
    } else if (div) {
        return first / second;
    }
    return INFINITY;
}

double str_to_double(char *str) {
    double res = 0;
    while (str && *str) {
        res = res * 10 + (*str) - '0';
        str++;
    }
    return res;
}

char *get_number_string(char *input) {
    char *number_string = NULL;
    int size = 0;
    while (is_number(*input)) {
        append_string(&number_string, &size, *input);
        input++;
    }
    return number_string;
}

char *get_alpha_string(char *input) {
    char *alpha_string = NULL;
    int size = 0;
    while (is_alpha(*input)) {
        append_string(&alpha_string, &size, *input);
        input++;
    }
    return alpha_string;
}

int is_alpha(char input) { return (input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'); }

int is_number(char input) { return input >= '0' && input <= '9'; }

int get_priority(char operator) {
    if (operator== '+' || operator== '-')
        return 1;
    else if (operator== '*' || operator== '/')
        return 2;
    else if (operator== '~')
        return 3;
    else if (operator>= 'A' && operator<= 'A' + FUNCTIONS_SIZE)
        return 4;
    else if (operator== '(' || operator== ')')
        return 0;
    return -1;
}

int is_operator(char input) {
    return input == '*' || input == '+' || input == '-' || input == '/' || input == '(';
}

int add_number(char *input, char **expression) {
    printf("addnumber curr expr = %s\n", *expression);
    char *new_number = get_number_string(input);
    printf("new number = %s\n", new_number);
    int new_size = sizeof(char);
    if ((*expression) != NULL) {
        new_size *= strlen(*expression);
    }
    int new_number_size = strlen(new_number);
    new_size += sizeof(char) * (new_number_size + 1);
    if (realloc_str(expression, new_size) == -1) {
        printf("Bad allocation\n");
        return -1;
    }
    printf("new_szie = %d\n", new_size);
    strcat((*expression), new_number);
    strcat((*expression), " ");
    free(new_number);
    return new_number_size;
}

int add_alpha(char *input, char **expression, struct node **stack) {
    char *new_alpha = get_alpha_string(input);
    int new_alpha_len = strlen(new_alpha);
    int check_push = 0;
    int check_x = 0;
    for (int i = 0; i < 6; i++) {
        if (strcmp(new_alpha, function_names[i]) == 0) {
            push_c(stack, 'A' + i);
            check_push = 1;
            break;
        }
    }
    if (strcmp(new_alpha, "x") == 0) {
        int expr_len = 0;
        if (*expression != NULL) {
            expr_len = strlen(*expression);
        }
        if (realloc_str(expression, sizeof(char) * (expr_len + new_alpha_len + 1))) {
            printf("Bad allocation\n");
            return -1;
        }
        strcat((*expression), new_alpha);
        strcat((*expression), " ");
        check_x = 1;
    }
    free(new_alpha);
    if (!check_push && !check_x) return -1;
    return new_alpha_len;
}

int add_from_stack(char **expression, struct node **stack) {
    int expr_len;
    int func_len;
    printf("peek c = %c\n", peek_c(*stack));
    while (stack && (*stack) && peek_c(*stack) != '(') {
        char new_char = pop_c(stack);
        expr_len = strlen(*expression);

        if (new_char >= 'A' && new_char < ('A' + FUNCTIONS_SIZE)) {
            func_len = strlen(function_names[(int)(new_char - 'A')]);
            if (realloc_str(expression, sizeof(char) * (expr_len + func_len + 1)) == -1) {
                printf("Bad allocation\n");
                return -1;
            }
            strcat((*expression), function_names[(int)(new_char - 'A')]);
            strcat((*expression), " ");
        } else {
            append_string(expression, &expr_len, new_char);
            append_string(expression, &expr_len, ' ');
        }
    }
    pop_c(stack);
    return 0;
}

int add_operator(char **input, char **input_start, char **expression, struct node **stack) {
    char op = **input;
    if (op == '-' && (*input == *input_start || (*input - *input_start > 1 && is_operator(*(*input - 1))))) {
        op = '~';
    }
    int op_priority = get_priority(op);
    int expr_len = 0;
    if (*expression != 0) {
        expr_len = strlen(*expression);
    }

    while (stack && (*stack) && get_priority(peek_c(*stack)) >= op_priority) {
        append_string(expression, &expr_len, pop_c(stack));
        append_string(expression, &expr_len, ' ');
    }
    push_c(stack, op);

    return 0;
}

int parse(char *input, char **expression, struct node **stack) {
    int offset;
    int parse_ok = 1;
    char *input_start = input;
    while (input && *input) {
        if (is_number(*input)) {
            if ((offset = add_number(input, expression)) == -1) {
                parse_ok = 0;
                break;
            }
            input += offset - 1;
        } else if (is_alpha(*input)) {
            if ((offset = add_alpha(input, expression, stack)) == -1) {
                parse_ok = 0;
                break;
            }
            input += offset - 1;
        } else if (*input == '(') {
            push_c(stack, *input);
        } else if (*input == ')') {
            if (add_from_stack(expression, stack) == -1) {
                parse_ok = 0;
                break;
            }
        } else if (is_operator(*input)) {
            if (add_operator(&input, &input_start, expression, stack) == -1) {
                parse_ok = 0;
                break;
            }
        } else if (*input == ' ') {
        } else {
            parse_ok = 0;
            break;
        }
        struct node *temp = *stack;
        printf("\nstack ");
        while (temp) {
            printf("%c ", temp->data.val_c);
            temp = temp->next;
        }
        printf("\n");
        input++;
    }
    int expr_len = strlen(*expression);
    while (*stack) {
        append_string(expression, &expr_len, pop_c(stack));
        append_string(expression, &expr_len, ' ');
    }
    return parse_ok;
}

int calculate(char *expression, double x, double *result) {
    char *cur_elem = strtok(expression, " ");
    struct node *stack = NULL;
    while (cur_elem != NULL) {
        if (is_number(cur_elem[0])) {
            push_d(&stack, str_to_double(cur_elem));
        } else if (is_operator(cur_elem[0]) || is_alpha(cur_elem[0])) {
            if (strcmp("~", cur_elem) == 0) {
                double last = stack ? pop_d(&stack) : 0;
                push_d(&stack, (0 - last));
            } else if (strcmp("x", cur_elem) == 0) {
                push_d(&stack, x);
            } else if (cur_elem[0] >= 'A' && cur_elem[0] < ('A' + FUNCTIONS_SIZE)) {
                double first = stack ? pop_d(&stack) : 0;
                push_d(&stack, execute_function(cur_elem[0] - 'A', first));
            } else {
                double second = stack ? pop_d(&stack) : 0;
                double first = stack ? pop_d(&stack) : 0;
                push_d(&stack, execute_operation(cur_elem, first, second));
            }
        }
        cur_elem = strtok(NULL, " ");
    }
    *result = pop_d(&stack);
    if (stack) {
        destroy(&stack);
    }
    return 0;
}