#ifndef CAD_OPS_H
#define CAD_OPS_H

#include "tronics.h"

void simulate_iot(void);
void simulate_cad(void);
void add_cad(const char *filename);
void diff_cad(const char *filename);
int parse_stl_vertices(const char *filename);

#endif