#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_output.h"
#include "pre_assembler.h"
#include "lexer.h"

int pre_assembler(char * input_file_name){
    char line[MAX_LINE_SIZE];
    char word[MAX_LINE_SIZE];
    char *pointer = line;
    int in_macro;
    node *cur_macro;
    node *head;
    char *temp_char;
    char *origin_file_name;
    char *new_file_name;
    FILE *input_file;
    FILE *output_file;
    head = NULL;
    in_macro = 0;
    memset(line, '\0', MAX_LINE_SIZE);
    memset(word, '\0', MAX_LINE_SIZE);
    new_file_name = full_file_name(input_file_name,".am");
    origin_file_name = full_file_name(input_file_name, ".as");
    output_file = fopen(new_file_name, "w");/*creating new file*/
    input_file = fopen(origin_file_name, "r");
    while(fgets(line, MAX_LINE_SIZE, input_file) != NULL) {
        remove_multiple_spaces(line);
        pointer = copy(line, ' ', word);
        if(*word == '\0')
            continue;
        if (in_macro) {
            if (!(int)strcmp(word, "endmacr")){/*checks if the new line start with word that represent the end of macro*/
                pointer = remove_beginning_spaces(pointer);/*remove the spaces after the first word */
                if(*pointer!='\n'){/*check if there is any more chars after the frist word if yes its error*/
                    printf("error");
                    remove(new_file_name);
                    break;
                }
                in_macro = 0;
            }
            else
                append_content(cur_macro,line);
        }
        else {
            switch (word_type(word,head)) {
                case 1:/*new macro delcleratio*/
                    pointer = copy(pointer, ' ', word);
                    pointer = remove_beginning_spaces(pointer);
                    if (!valid_macro(word)||search_node(head,word) != NULL||*pointer!='\n' ) {
                        remove(new_file_name);
                        break;
                    }
                    cur_macro= create_node(word,"");
                    add_node(&head,cur_macro);
                    in_macro = 1;
                    break;
//search in all the node for suteable macro and replaced it to the new file
                case 2:
                    cur_macro = search_node(head,word);
                    fprintf(output_file,(cur_macro->content));
                    break;

                case 3:
                    fprintf(output_file, line);
                    break;


            }
            pointer = line;
        }
    }
    fclose(output_file);
    free_list(head);
    free(origin_file_name);
    free(new_file_name);
}

char* copy(char * source,char end, char *target){
    int i = 0;
    source = remove_beginning_spaces(source);
    while (*source != end && *source != '\t' && *source != '\n' && *source != '\0'){
        target[i] = *source;
        source++;
        i++;
    }
    target[i] = '\0';
    return source;
}

char*  remove_beginning_spaces(char *line){
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    return line;
}


int word_type(char *word,node *head){
    if(!(int)strcmp(word, "macr")){
        return 1;
    }
    else if(search_node(head,word) != NULL){
        return 2;
    }
    return 3;
}


void remove_multiple_spaces(char *str) {
    int i = 0;  // Index to read the original string
    int j = 0;  // Index to write to the new string
    int space_found = 0;  // Flag to check if a space or tab was found

    // Trim leading spaces and tabs
    while (str[i]==' ' || str[i]=='\t') {
        i++;
    }

    // Process the string to remove multiple spaces and tabs
    while (str[i] != '\0') {
        if (str[i]==' ' || str[i]=='\t') {
            if (!space_found) {
                // Copy the first space or tab found
                str[j++] = ' ';
                space_found = 1;
            }
        } else {
            // Copy the non-space character and reset the space flag
            str[j++] = str[i];
            space_found = 0;
        }
        i++;
    }
    str[j] = '\0';
    j--;
    while (j >= 0 && (str[j]==' ' || str[j]=='\t')) {
        str[j] = '\0';
        j--;
    }
}








/*

temp_char = strchr(line,':');
if(temp_char){
temp_char = strchr(temp_char,' ')+1;//getting the first char after the space ... מאקרו בתוך תייות
copy(temp_char,' ',word);
}
else{
copy(line,' ',word);
}
*/
