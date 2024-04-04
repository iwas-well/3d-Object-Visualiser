#ifndef __DATA_VISU__
#define __DATA_VISU__

#define VERTEX_NUM 150    //number of vertices which are allocated each time
#define POLYGON_NUM 50    //number of polygons which are allocated each time

typedef struct vec3{
    float x,y,z;
} vec3;

typedef struct vec2{
    float x,y;
} vec2;

typedef struct polygon{
    int vnum;       //specifies number of vertices of the plane
    //int* pv;         //specifies the vertices that form the polygon
    vec3** v;      //array of pointers to the vertices that form the polygon
} polygon;

typedef struct mesh{
    vec3* v;
    polygon* pol;
    int vnum;       //number of vertices of the mesh
    int pnum;       //number of polygons of the mesh
    int vmax;       //maximum number of verteces (allocated)
    int pmax;       //maximum number of polygons (allocated)
} mesh;

struct cam{
    vec3 v;
    int width, height;
}; typedef struct cam cam;

#endif
