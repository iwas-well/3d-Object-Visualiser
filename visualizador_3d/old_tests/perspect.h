#ifndef __PERSP_VISU__
#define __PERSP_VISU__

#include "datatypes.h"

float mim(float a, float b);

float max(float a, float b);

float calculate_scale(float x, float y, cam* camera);

void calculate_mesh_position(mesh* object, cam* camera);

#endif
