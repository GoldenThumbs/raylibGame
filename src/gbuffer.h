#ifndef GBUFFER_H
#define GBUFFER_H

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <external/glad.h>

typedef struct gbuffer_t {
    unsigned int id;
    int width;
    int height;
    Texture2D color;
    Texture2D normal;
    Texture2D position;
    Texture2D depth;
} gbuffer_t;

gbuffer_t gbuffer_new(int, int);
void gbuffer_begin(gbuffer_t);
void gbuffer_end();
void gbuffer_free(gbuffer_t);

#endif