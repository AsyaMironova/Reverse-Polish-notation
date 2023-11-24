#ifndef STACK_H
#define STACK_H

struct node {
    union {
        char val_c;
        double val_d;
    } data;

    struct node *next;
};

void push_c(struct node **top, char data);
char pop_c(struct node **top);
char peek_c(struct node *top);
void push_d(struct node **top, double data);
double pop_d(struct node **top);
void destroy(struct node **top);

#endif