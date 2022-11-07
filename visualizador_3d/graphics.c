#include "graphics.h"

void render_polygon(SDL_Renderer* renderer, polygon* pol){

    //draw lines v0,v1 and v1,v2
    SDL_RenderDrawLine(renderer, pol->v[0].x, pol->v[0].y, pol->v[1].x, pol->v[1].y);
    SDL_RenderDrawLine(renderer, pol->v[1].x, pol->v[1].y, pol->v[2].x, pol->v[2].y);

    if (pol->vnum == 4){
        //draw lines v2,v3 and v3,v0
        SDL_RenderDrawLine(renderer, pol->v[2].x, pol->v[2].y, pol->v[3].x, pol->v[3].y);
        SDL_RenderDrawLine(renderer, pol->v[3].x, pol->v[3].y, pol->v[0].x, pol->v[0].y);
    }
    else
        //draw line v2,v0
        SDL_RenderDrawLine(renderer, pol->v[2].x, pol->v[2].y, pol->v[0].x, pol->v[0].y);
}

void render_object(mesh* object, cam* camera, SDL_Renderer* renderer){
    vec3 cam;

    calculate_mesh_position(object, camera);

    for (int i = 0; i < object->pnum; i++)
        render_polygon(renderer, &object->pol[i]);
}
