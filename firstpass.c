#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "lexer.h"
int first_pass(char *file){
    int IC , error;
    char str[MAX_LINE_LENGTH];
    char copy_str[MAX_LINE_LENGTH];

    char *first_space;
    char *input_check;

    FILE *fp = fopen(file , "r");
    error = 1;
    IC = 99;

    while (fgets(str,MAX_LINE_LENGTH,fp )){
        strcpy(copy_str , str);

        if(str[0] == '\n'){
            continue;
        }
        first_space = strchr(copy_str, ' ');
        int length = first_space - str;
        strncpy(input_check, str, length);

        if(find_opcode(input_check) > 0){
            IC++;
            int num_op = (find_opcode(input_check));

            if(num_op < 5){

            }

            if(num_op > 5 && num_op < 14) {

            }
            else
            {

            }

        }

        else if(find_instraction(input_check) > 0){
            int num_instraction = (find_instraction(input_check));

            switch (num_instraction) {

                case 0:

                case 1:

                case 2:

                case 3:
                
            }

        }

        else if (label_fine(input_check) > 0){

        }
        else
            error = -1;

    }

    fclose(fp);
    return error;
}