#include "objread.h"

#define LINE_SIZE 1024

//************************************************************
//move to another lib
int realloc_vertice_array(mesh* object){

    if ( !(object->v = realloc(object->v, object->vmax + VERTEX_NUM )) )
        return 0;

    object->vmax += VERTEX_NUM;

    return 1;
}

int realloc_polygon_array(mesh* object){

    if ( !(object->pol = realloc(object->pol, object->pmax + POLYGON_NUM )) )
        return 0;

    object->pmax += POLYGON_NUM;
    return 1;
}

int initialize_mesh(mesh** object){

    if ( !((*object) = malloc(sizeof(mesh))) )
        return 0;

    if ( !((*object)->v = (vec3 *) malloc(sizeof(vec3) * VERTEX_NUM )) )
        return 0;

    if ( !((*object)->pol = (polygon *) malloc(sizeof(polygon) * POLYGON_NUM )) )
        return 0;

    (*object)->vnum = 0;
    (*object)->pnum = 0;
    (*object)->vmax = VERTEX_NUM;
    (*object)->pmax = POLYGON_NUM;

    return 1;
}
//************************************************************

int read_polygon(char* line, mesh* object){
    int vert_number;

    if ( object->pnum == object->pmax )
        if (!realloc_polygon_array(object))
            return 0;
    
    if ( !(object->pol[ object->pnum ].v = malloc(sizeof(vec3*) * 3)) )
        return 0;

    int i = 0;
    char* token;
    strtok(line," ");
    while ( (token = strtok(NULL," ")) ){
        if ( sscanf(token,"%d",&vert_number) == 1){
            if (i >= 3)
                if ( !(object->pol[ object->pnum ].v = realloc( object->pol[ object->pnum ].v, sizeof(vec3*)*(i+1) )) )
                    return 0;
            object->pol[ object->pnum ].v[i] = &(object->v[ vert_number - 1 ]);
            i++;
        }
    }

    object->pol[ object->pnum ].vnum = i;
    object->pnum++;

    return 1;
}

int read_vertice(char* line, mesh* object){
    int ind;
    float num;
    char* token;

    strtok(line," ");

    if ( object->vnum == object->vmax )
        if (!realloc_vertice_array(object))
            return 0;

    token = strtok(NULL," ");
    if ( sscanf(token,"%f",&num) == 1)
        object->v[ object->vnum ].x = num;

    token = strtok(NULL," ");
    if ( sscanf(token,"%f",&num) == 1)
        object->v[ object->vnum ].y = num;

    token = strtok(NULL," ");
    if ( sscanf(token,"%f",&num) == 1)
        object->v[ object->vnum ].z = num;

    object->vnum++;

    return 1;
}

int read_file(FILE* mesh_file, mesh* object){
    char line[LINE_SIZE];

    fgets(line, LINE_SIZE, mesh_file);
    while ( !feof(mesh_file) ){

        if (line[0] == 'v'){
            if ( !read_vertice(line, object) )
                return 0;
        }
        else if (line[0] == 'f'){
            if ( !read_polygon(line, object) )
                return 0;
        }

        fgets(line, LINE_SIZE, mesh_file);

    }

    return 1;
}
