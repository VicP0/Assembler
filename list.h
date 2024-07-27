/* Represents a node in a linked list */
typedef struct node {
    char *name;         /* Name associated with the node */
    char *content;      /* Content stored in the node */
    struct node *next;  /* Pointer to the next node in the linked list */
} node;

node *create_node(char *name, char *content);


node *search_node(node *head, char *name);


int add_node(node **head, node *new);

void append_content(node *n,char *new_content);


void free_node(node *node1);


void free_list(node *head);
