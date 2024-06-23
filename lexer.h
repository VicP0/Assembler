#ifndef UNTITLED6_LEXER_H
#define UNTITLED6_LEXER_H

typedef struct op_code{
    char* op_name;
    int num_of_args;
}op_code;


int find_opcode (char* input);

int find_register(char* input);

int find_instraction(char* input);

int label_fine(char* input);
#endif //UNTITLED6_LEXER_H
