#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "datatypes.h"

int read_polygon(char* line, mesh* object);
int read_vertices(char* line, mesh* object);
int initialize_mesh(mesh** object);
int read_file(FILE* mesh_file, mesh* object);
