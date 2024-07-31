#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "lexer.h"
#include "table.h"

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
        strncpy(input_check, copy_str, length);

        if (label_fine(input_check) > 0){
            if(search(input_check) != NULL){ // if the label already exist
                error= -1;
                return error;
            }
            memmove(copy_str, first_space + 1, strlen(first_space));
            remove_spaces(copy_str);
            int length = first_space - str;
            strncpy(input_check, copy_str, length);
            if(find_opcode(input_check) > 0){
                int num_op = (find_opcode(input_check));
                error = parse_parameters(copy_str , num_op);
                if(error != 1){
                    return error;
                }


            }

            if(find_instraction(input_check) > 0){
                //found instruction and need to oparete it
            }






        }

        if(find_opcode(input_check) > 0){

            int num_op = (find_opcode(input_check));

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
        else
            error = -1;

    }

    fclose(fp);
    return error;
}
