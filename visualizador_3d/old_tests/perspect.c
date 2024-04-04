#include "datatypes.h"
#include "perspect.h"

float min(float a, float b){
    if (a <= b)
        return a;
    return b;
}

float max(float a, float b){
    if (a >= b)
        return a;
    return b;
}

float calculate_scale(float x, float y, cam* camera){
    float scale_x, scale_y;
    scale_x = camera->width/x;
    scale_y = camera->height/y;
    return min(scale_x, scale_y);
}

void calculate_mesh_position(mesh* object, cam* camera){
    vec2 aux_vec;
    int i, j;
    float xdiff, ydiff;
    float xmin, ymin;
    float xmax, ymax;
    float xcen, ycen;
    float scale;

    for (i = 0; i < object->pnum; i++){

        for (j = 0; j < object->pol->vnum; j++){
            //calculating [x,y] projected in the plane z = 0
            aux_vec.x = camera->v.x + camera->v.z * ( (object->pol->v[j]->x - camera->v.x) / 
                (object->pol->v[j]->z + camera->v.z) );
            aux_vec.y = camera->v.y + camera->v.z * ( (object->pol->v[j]->y - camera->v.y) /
                (object->pol->v[j]->z + camera->v.z) );

            xmin = min(aux_vec.x, xmin);
            ymin = min(aux_vec.y, ymin);
            xmax = max(aux_vec.x, xmax);
            ymax = max(aux_vec.y, ymax);
            xdiff = (xmax - xmin);
            ydiff = (ymax - ymin);
            xcen = xdiff/2;
            ycen = ydiff/2;

            scale = calculate_scale(xdiff, ydiff, camera);

            object->pol->v[j]->x = ((aux_vec.x - xcen) * scale) + (float)camera->width/2;
            object->pol->v[j]->y = ((aux_vec.y - xcen) * scale) + (float)camera->height/2;
        }

    }
}

/*De acordo com a especifiacao do projeto:
*  Considerando que as coordenadas da câmera são [xc yc zc] e que a câmera está olhando
*  para a origem [0 0 0], a conversão das coordenadas 3D de cada vértice v = [xv yv zv]
*  em sua projeção 2D p = [xp yp] no plano z = 0 pode ser calculada desta forma:
*   xp = xc + zc × ( (xv - xc) ÷ (zv + zc) )
*   yp = yc + zc × ( (yv - yc) ÷ (zv + zc) )
*  Assim é obtida uma coleção de pontos [xp yp] que representa os vértices do objeto
*  projetados no plano z=0.
*/
