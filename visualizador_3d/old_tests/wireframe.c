#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "datatypes.h"
#include "objread.h"

int main(int argc, char** argv){

    if (argc != 2){
        fprintf(stderr, "Uso: %s <arquivo.obj>\n", argv[0]);
        exit(1);
    }
    
    mesh* object;
    FILE* file;
    file = fopen(argv[1], "r");

    if ( !initialize_mesh(&object) ){
        fprintf(stderr, "Erro ao inicialilzar mesh struct\n");
        exit(1);
    }

    read_file(file, object);

    for (int i = 0; i < object->vnum; i++)
        printf("V %5.1f %5.1f %5.1f \n", object->v[i].x, object->v[i].y, object->v[i].z);

    for (int i = 0; i < object->pnum; i++){
        printf("F ");

        for (int j = 0; j < object->pol[i].vnum; j++)
            printf("(%5.1f,%5.1f,%5.1f) ", object->pol[i].v[j]->x, object->pol[i].v[j]->y, object->pol[i].v[j]->z);
        printf("\n");
    }

    return 0;
}
