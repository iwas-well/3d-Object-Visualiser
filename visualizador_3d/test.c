//#include <SDL2/SDL_events.h>
//#include <SDL2/SDL_surface.h>
//#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>


//---------------------------
//create utils lib
//---------------------------

#define WIDTH 800   //window width
#define HEIGHT 800  //window height
#define FPS 60      //FPS per second

#define PI 3.14159265

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    Vec3* vertex;
    int vertexNum;
} Poly;

typedef struct {
    Poly* poly;
    int polyNum;
    Vec3 origin; //center of reference for rotation and movement of the mesh
} Mesh;

typedef struct {
    Vec2 pos;
    int track;
} Mouse;

typedef struct {
    //SDL_Surface* surface;
    SDL_Window* window;
    SDL_Renderer* renderer;
} Frame_SDL;

Frame_SDL gFrame;

void print_poly(Poly poly){
    for (int i = 0; i < poly.vertexNum; i++)
        printf("v%d %f %f %f\n", i, poly.vertex[i].x,poly.vertex[i].y,poly.vertex[i].z);
}

void print_mesh(Mesh mesh){
    for (int i = 0; i < mesh.polyNum; i++){
        printf("Polygon %d\n", i);
        print_poly(mesh.poly[i]);
        printf("\n");
    }
}

Vec2 make_vec2(float x, float y){
    Vec2 vec;
    vec.x = x;
    vec.y = y;
    return vec; 
}

Vec3 make_vec3(float x, float y, float z){
    Vec3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec; 
}

void sub_vec3(Vec3* veca, Vec3 vecb){
    veca->x -= vecb.x;
    veca->y -= vecb.y;
    veca->z -= vecb.z;
}

void add_vec3(Vec3* veca, Vec3 vecb){
    veca->x += vecb.x;
    veca->y += vecb.y;
    veca->z += vecb.z;
}


float min_f(float a, float b){
    if (a<b) return a;
    return b;
}

float max_f(float a, float b){
    if (a<b) return a;
    return b;
}

float clamp_f(float a, float min, float max){
    if (a<min) return min;
    if (a>max) return max;
    return a;
}
//-------------------------------------------------

//--- create projection lib -----------------------
Vec2 proj_vec3_screen(Vec3 vec3){
    Vec2 vec2;
    vec2.x = (vec3.x/vec3.z +1) * WIDTH/2;
    vec2.y = (vec3.y/vec3.z +1) * HEIGHT/2;
    return vec2;
}

int render_poly(Poly poly){
    Vec2 proj[poly.vertexNum];

    for (int i = 0; i < poly.vertexNum; i++)
        proj[i] = proj_vec3_screen( poly.vertex[i] );

    int i;
    for (i = 0; i < (poly.vertexNum-1); i++)
        SDL_RenderDrawLine(gFrame.renderer, proj[i].x, proj[i].y, proj[i+1].x, proj[i+1].y);
    SDL_RenderDrawLine(gFrame.renderer, proj[i].x, proj[i].y, proj[0].x, proj[0].y);
    return 0;
}

int render_mesh(Mesh mesh){
    for(int i = 0; i<mesh.polyNum; i++)
        render_poly(mesh.poly[i]);
    return 0;
}

int rotate_z(Mesh* cube, float dt){
    Poly** poly = &(cube->poly);
    float sinTheta = sinf(dt);
    float cosTheta = cosf(dt);
    float x,y;
    for (int i = 0; i < 6; i++)
        for (int j = 0; j<4; j++){
            sub_vec3(&((*poly)[i].vertex[j]), cube->origin);
            x = (*poly)[i].vertex[j].x;
            y = (*poly)[i].vertex[j].y;
            (*poly)[i].vertex[j].x = x * cosTheta - y * sinTheta;
            (*poly)[i].vertex[j].y = x * sinTheta + y * cosTheta;
            add_vec3(&((*poly)[i].vertex[j]), cube->origin);
        }
    return 0;
}

int rotate_3d_x(Mesh* cube, float dt){
    Poly** poly = &(cube->poly);
    float sinTheta = sinf(dt);
    float cosTheta = cosf(dt);
    float y,z;

    for (int i = 0; i < 6; i++)
        for (int j = 0; j<4; j++){
            sub_vec3(&((*poly)[i].vertex[j]), cube->origin);
            y = (*poly)[i].vertex[j].y;
            z = (*poly)[i].vertex[j].z;
            (*poly)[i].vertex[j].y = y*cosTheta - z*sinTheta;
            (*poly)[i].vertex[j].z = y*sinTheta + z*cosTheta;
            add_vec3(&((*poly)[i].vertex[j]), cube->origin);
        }
    return 0;
}

int rotate_3d_y(Mesh* cube, float dt){
    Poly** poly = &(cube->poly);
    float sinTheta = sinf(dt);
    float cosTheta = cosf(dt);
    float x,z;

    for (int i = 0; i < 6; i++)
        for (int j = 0; j<4; j++){
            sub_vec3(&((*poly)[i].vertex[j]), cube->origin);
            x = (*poly)[i].vertex[j].x;
            z = (*poly)[i].vertex[j].z;
            (*poly)[i].vertex[j].x = x*cosTheta + z*sinTheta;
            (*poly)[i].vertex[j].z = -x*sinTheta + z*cosTheta;
            add_vec3(&((*poly)[i].vertex[j]), cube->origin);
        }
    return 0;
}
//-------------------------------------------------

int sdl_init(){

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "SDL nao pode inicializar: %s\n", SDL_GetError() );
        return -1;
    }

    gFrame.window = SDL_CreateWindow("teste", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (gFrame.window == NULL){
        fprintf(stderr, "SDL nao pode criar janela: %s\n", SDL_GetError() );
        return -1;
    }

    gFrame.renderer = SDL_CreateRenderer(gFrame.window, -1, 0);
    if (gFrame.renderer == NULL){
        fprintf(stderr, "SDL nao pode criar renderer: %s\n", SDL_GetError() );
        return -1;
    }

    return 0;
}

void sdl_end(){
    SDL_DestroyRenderer(gFrame.renderer);
    SDL_DestroyWindow(gFrame.window);
    SDL_Quit();
}

int create_cube(Mesh* cube, float z){
    
    cube->polyNum=6;

    Poly** poly = &(cube->poly);
    (*poly) = malloc(sizeof(Poly) *6);
    if (*poly == NULL){
        printf("Nao foi possivel alocar poligonos\n");
        return -1;
    }

    for (int i = 0; i < 6; i++){
        (*poly)[i].vertex = malloc(sizeof(Vec3)*4);
        if ((*poly)[i].vertex == NULL){
            free(*poly);
            printf("Nao foi possivel alocar vertices\n");
            return -1;
        }
        (*poly)[i].vertexNum = 4;
    }

    cube->origin.x=0;
    cube->origin.y=0;
    cube->origin.z=2.5;

    //front
    (*poly)[0].vertex[0] = (Vec3){-0.5,-0.5, z};
    (*poly)[0].vertex[1] = (Vec3){ 0.5,-0.5, z};
    (*poly)[0].vertex[2] = (Vec3){ 0.5, 0.5, z};
    (*poly)[0].vertex[3] = (Vec3){-0.5, 0.5, z};
    //back 
    (*poly)[1].vertex[0] = (Vec3){-0.5,-0.5, z+1};
    (*poly)[1].vertex[1] = (Vec3){ 0.5,-0.5, z+1};
    (*poly)[1].vertex[2] = (Vec3){ 0.5, 0.5, z+1};
    (*poly)[1].vertex[3] = (Vec3){-0.5, 0.5, z+1};
    //right
    (*poly)[2].vertex[0] = (Vec3){-0.5,-0.5, z};
    (*poly)[2].vertex[1] = (Vec3){-0.5,-0.5, z+1};
    (*poly)[2].vertex[2] = (Vec3){-0.5, 0.5, z+1};
    (*poly)[2].vertex[3] = (Vec3){-0.5, 0.5, z};
    //left 
    (*poly)[3].vertex[0] = (Vec3){0.5,-0.5, z};
    (*poly)[3].vertex[1] = (Vec3){0.5,-0.5, z+1};
    (*poly)[3].vertex[2] = (Vec3){0.5, 0.5, z+1};
    (*poly)[3].vertex[3] = (Vec3){0.5, 0.5, z};
    //up   
    (*poly)[4].vertex[0] = (Vec3){-0.5, 0.5, z};
    (*poly)[4].vertex[1] = (Vec3){ 0.5, 0.5, z};
    (*poly)[4].vertex[2] = (Vec3){ 0.5, 0.5, z+1};
    (*poly)[4].vertex[3] = (Vec3){-0.5, 0.5, z+1};
    //down 
    (*poly)[5].vertex[0] = (Vec3){-0.5, -0.5, z};
    (*poly)[5].vertex[1] = (Vec3){ 0.5, -0.5, z};
    (*poly)[5].vertex[2] = (Vec3){ 0.5, -0.5, z+1};
    (*poly)[5].vertex[3] = (Vec3){-0.5, -0.5, z+1};

    return 0;
}

void free_mesh(Mesh mesh) {
    for(int i = 0; i < mesh.polyNum; i++)
            free(mesh.poly[i].vertex);
    free(mesh.poly);
}

void handle_events(SDL_Event event, float ang, float * ang_x, float* ang_y, int* running, int* reload, Mouse* mouse) {

    while ( SDL_PollEvent(&event) != 0 ) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP)
                *ang_x = -ang;
            else if (event.key.keysym.sym == SDLK_DOWN)
                *ang_x = ang;
            else if (event.key.keysym.sym == SDLK_LEFT)
                *ang_y = ang;
            else if (event.key.keysym.sym == SDLK_RIGHT)
                *ang_y = -ang;
            else if ((event.key.keysym.sym == SDLK_q) || (event.key.keysym.sym == SDLK_ESCAPE))
                *running = 0;
            else if (event.key.keysym.sym == SDLK_SPACE)
                *reload = 1;
        }
        else if (event.type == SDL_KEYUP) {
            if ((event.key.keysym.sym == SDLK_UP) || (event.key.keysym.sym == SDLK_DOWN))
                *ang_x = 0;
            else if ((event.key.keysym.sym == SDLK_LEFT) || (event.key.keysym.sym == SDLK_RIGHT))
                *ang_y = 0;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            mouse->pos.x = event.motion.x;
            mouse->pos.y = event.motion.y;
            mouse->track = 1;
        }
        else if (event.type == SDL_MOUSEBUTTONUP){
            mouse->track = 0;
            *ang_x=0; *ang_y=0;
        }
        else if (event.type == SDL_MOUSEMOTION) {
            int x=event.motion.x;
            int y=event.motion.y;
            if (mouse->track){
                if ((x < 0 ) || (x > WIDTH) || (y < 0 ) || (y > HEIGHT)) {
                    mouse->track = 0;
                    *ang_x=0; *ang_y=0;
                }
                else {
                    *ang_x = -0.08*(mouse->pos.y - y);
                    *ang_y = 0.08*(mouse->pos.x - x);
                } 
            }
            mouse->pos.x = x;
            mouse->pos.y = y;
            *ang_x = clamp_f(*ang_x, -ang, ang);
            *ang_y = clamp_f(*ang_y, -ang, ang);
        }
    }

}


int main(){

    Mouse mouse;
    mouse.track = 0;
    mouse.pos.x = -1;
    mouse.pos.y = -1;

    float z = 2;//starting point of the cube
    Mesh cube;
    if (create_cube(&cube, z) < 0)
        return -1;

    float ang=9./FPS;    //angle rotated per frame

    float ang_x, ang_y;
    ang_x = ang_y = 0;


    sdl_init();
    SDL_Event event;
    int reload = 0;
    int running = 1;
    while (running) {

        handle_events(event, ang, &ang_x, &ang_y, &running, &reload, &mouse);

        if (reload == 1){
            free_mesh(cube);
            if (create_cube(&cube, z) < 0) return -1;
        }
        else{
            rotate_3d_x(&cube, ang_x);
            rotate_3d_y(&cube, ang_y);
            if (mouse.track) ang_x=ang_y=0;
        }

        SDL_SetRenderDrawColor(gFrame.renderer, 10, 10, 10, 255);
        SDL_RenderClear(gFrame.renderer);
        SDL_SetRenderDrawColor(gFrame.renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(gFrame.renderer, mouse.pos.x, mouse.pos.y);
        render_mesh(cube);
        SDL_RenderPresent(gFrame.renderer);

        SDL_Delay( 1000/FPS );
    }

    sdl_end();
    return 0;
}
