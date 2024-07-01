#include <stdio.h>





char *non_macro_line_processing(char *str){
    remove_spaces(str);
}

int empty_line{

};

int check_if_macr(char* input)
{
    char* temp;
    remove_spaces(input);
    temp  = (char*)malloc((strlen(input) + 1) * sizeof(char));

    temp = strncpy(temp , temp , 4);

    if(valid_dec_macro(temp) > 0)
        return 0;

    return -1;
}
