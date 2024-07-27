#include <stdlib.h>
#include <string.h>
#include "table.h"


int add_symbol(symbol_table **sym_table, int count, char *name , int line_count){
    symbol_table *temp;
    temp = *sym_table;

    temp = realloc(sym_table, (count + 1) * sizeof(symbol_table));
    if (temp == NULL) {
        return -1;
    }

    temp[count].name = strdup(name);
    if (temp[count].name == NULL) {
        return -1;
    }
    temp[count].adress = count;
    temp[count].line_count = line_count;

    *sym_table = temp;

    return 1;

}

