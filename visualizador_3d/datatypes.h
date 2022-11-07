#ifndef __DATA_VISU__
#define __DATA_VISU__

struct vec3{
    float x,y,z;
}; typedef struct vec3 vec3;

struct vec2{
    float x,y;
}; typedef struct vec2 vec2;

struct polygon{
    int vnum;       //specifies number of vertices of the plane
    vec3 v[4];      //four vertices in each polygon   
}; typedef struct polygon polygon;

struct mesh{
    polygon* pol;
    int pnum;       //specifies number of polygons of the mesh
}; typedef struct mesh mesh;

struct cam{
    vec3 v;
    int width, height;
}; typedef struct cam cam;

#endif
