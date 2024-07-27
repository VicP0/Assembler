#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

node* create_node(char *name, char *content) {
    node *new_node = (node*)malloc(sizeof(node));
    new_node->name = (char*)malloc(strlen(name) + 1);
    strcpy(new_node->name, name);
    new_node->content = (char*)malloc(strlen(content) + 1);
    strcpy(new_node->content, content);
    new_node->next = NULL;
    return new_node;
}

node *search_node(node *head, char *name){
    node *runner = head;
    while (runner != NULL){
        if(strcmp(runner->name,name)== 0)
            return runner;
        runner = runner->next;
    }
    return NULL;

}
int add_node(node **head, node *new) {
    node *runner = *head;
    if (search_node(*head, new->name) != NULL) {
        return 0;  //node already exist
    }

    if (*head == NULL) {
        *head = new;
    }
    while (runner->next != NULL) {
        runner = runner->next;
    }
    runner->next = new;
    return 1;
}
void append_content(node *n, char *new_content) {
    int old_content_length = (int)strlen(n->content);
    int new_content_length = (int)strlen(new_content);

    char *temp = (char*)realloc(n->content, old_content_length + new_content_length -1);
    n->content = temp;
    strcpy(n->content + old_content_length, new_content);// moving the pointer to the plaxce were the string previus string end and then appened
}

void free_node(node *node1){
    /* Free memory allocated for node */
    free(node1->name);
    free(node1->content);
    free(node1);
}

void free_list(node *head){
    /* Free memory for the list node*/
    while(head != NULL) {
        node *temp = head;
        head = head->next;
        free_node(temp);
    }
}

