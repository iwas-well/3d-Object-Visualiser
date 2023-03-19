#include <stdio.h>
#include "objread.h"

int main(int argc, char** argv){
    FILE* arquivo;
    arquivo = fopen(argv[1], "r");
    read_file(arquivo);
    return 0;
}
