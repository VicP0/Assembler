#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "defines.h"
#include "lexer.h"

op_code kind_of_opcode [] = {
        {
            "mov" , 2
        },
        {
            "cmp" , 2
        },
        {
            "add" , 2
        } ,
        {
            "sub" , 2
        },
        {
            "lea" , 2
        },
        {
            "clr" , 1
        },
        {
            "not" , 1
        },
        {
            "inc" , 1
        },
        {
            "dec" , 1
        },
        {
            "jmp" , 1
        }, {
            "bne" , 1
        },
        {
            "red" , 1
        },
        {
            "prn" , 1
        },
        {
            "jsr" , 1
        },
        {
            "rts" , 0
        },
        {
            "stop" , 0
        }
};

char* registers [] = {"r0" , "r1" , "r2" , "r3" , "r4" , "r5" , "r6" , "r7",
                      "*r0" , "*r1" , "*r2" , "*r3" , "*r4" , "*r5" , "*r6" , "*r7"};

char* instractions[] = {".data" , ".string" , ".extern" , ".entry"};


int file_valid(char* file)
{
    FILE *fh = fopen(file, "r");
    if (fh == NULL)
    {
        return -1;
    }

    return 0;
}

int line_overflow(char* file)
{
    int flag = 0;
    char str[LIMIT_NUMBER];
    FILE *fh = fopen(file, "r");

    while (fgets(str, LIMIT_NUMBER, fh) != NULL)
    {
        if (strlen(str) >= ONE_THUSAND)
        {
            flag = -1;
        }
    }

    fclose(fh);
    return flag;
}
int find_opcode (char* input)
{
    int i;
    if(input == NULL)
        return -1;
    for (i = 0; i < NUM_OPCODES ; i++)
    {
        if(strcmp(kind_of_opcode[i].op_name, input) == 0)
            return i;
    }

return -1;
}

int find_register(char* input)
{
    int i ;
    if(input == NULL)
        return -1;
    for(i=0 ; i < REG_NUM * 2 ; i++)
    {
        if(strcmp(registers[i],input) == 0)
            return i;
    }

    return -1;
}

int find_instraction(char* input)
{
    int i ;
    if(input == NULL)
        return -1;

    for(i=0 ; i < NUM_OF_INSTRACTIONS ; i++)
    {
        if(strcmp(instractions[i] , input) == 0)
        {
            return i;
        }
    }
    return -1;
}

int label_fine(char* input)
{
    if(input == NULL)
        return -1;
    if(strlen(input) > MAX_LABEL_LENGTH || find_opcode(input) < 0 ||
        find_instraction(input) < 0 || !isalpha(*input) || find_register(input))
    {
        return -1;
    }

    while(input != '\0' && (isalpha(*input) || isdigit(*input)))
    {
        ++input;
        if(*input == ':' && (input+1) == '\0')
        {
            *input = '\0';
            return 1;
        }
    }
    return -1;
}

int valid_macro (char* input)
{
    if(find_opcode(input) < 0 || find_register(input) < 0
         || find_instraction(input) < 0)
    {
        return 1;
    }
    return -1;
}

int valid_dec_macr(char* input)
{
    char* macr = "macr";
    int i = 0;

    while (macr[i] != '\0')
    {
        if (input[i] != macr[i])
        {
            return -1;
        }
        i++;
    }

    return 1;
}

void  remove_spaces (char* input)
{
    int i =0;
    while(isspace(input[i]))
    {
        i++;
    }

    memmove(input, input + i, strlen(input + i) + 1);
}
