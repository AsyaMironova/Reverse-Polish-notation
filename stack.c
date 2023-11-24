#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

void push_c(struct node **top, char data) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node == NULL) {
        printf("Memory is not allocated\n");
        return;
    }
    new_node->data.val_c = data;
    if ((*top) == NULL) {
        new_node->next = NULL;
        *top = new_node;
    } else {
        new_node->next = (*top);
        (*top) = new_node;
    }
}

char pop_c(struct node **top) {
    if (!top || !(*top)) return 0;
    char result = (*top)->data.val_c;
    struct node *to_delete = (*top);
    (*top) = (*top)->next;
    free(to_delete);
    return result;
}

void push_d(struct node **top, double data) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node == NULL) {
        printf("Memory is not allocated\n");
        return;
    }
    new_node->data.val_d = data;
    if ((*top) == NULL) {
        new_node->next = NULL;
        *top = new_node;
    } else {
        new_node->next = (*top);
        (*top) = new_node;
    }
}

double pop_d(struct node **top) {
    if (!top || !(*top)) return 0;
    double result = (*top)->data.val_d;
    struct node *to_delete = (*top);
    (*top) = (*top)->next;
    free(to_delete);
    return result;
}

void destroy(struct node **top) {
    while ((*top) != NULL) {
        struct node *to_delete = (*top);
        (*top) = (*top)->next;
        free(to_delete);
    }
}

char peek_c(struct node *top) { return top->data.val_c; }
