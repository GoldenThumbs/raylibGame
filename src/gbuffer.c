#include "gbuffer.h"

gbuffer_t gbuffer_new(int width, int height)
{
    gbuffer_t gbuffer;
    gbuffer_init(&gbuffer, width, height);
    return gbuffer;
}

void gbuffer_init(gbuffer_t* gbuffer, int width, int height)
{
    gbuffer->id = rlLoadFramebuffer(width, height);
    gbuffer->width = width;
    gbuffer->height = height;
    
    gbuffer->color.id = rlLoadTexture(NULL, width, height, UNCOMPRESSED_R8G8B8A8, 1);
    gbuffer->color.width = width;
    gbuffer->color.height = height;
    gbuffer->color.format = UNCOMPRESSED_R8G8B8A8;
    gbuffer->color.mipmaps = 1;
    
    gbuffer->normal.id = rlLoadTexture(NULL, width, height, UNCOMPRESSED_R8G8B8A8, 1);
    gbuffer->normal.width = width;
    gbuffer->normal.height = height;
    gbuffer->normal.format = UNCOMPRESSED_R8G8B8A8;
    gbuffer->normal.mipmaps = 1;
    
    gbuffer->position.id = rlLoadTexture(NULL, width, height, UNCOMPRESSED_R8G8B8A8, 1);
    gbuffer->position.width = width;
    gbuffer->position.height = height;
    gbuffer->position.format = UNCOMPRESSED_R8G8B8A8;
    gbuffer->position.mipmaps = 1;

    gbuffer->depth.id = rlLoadTextureDepth(width, height, false);
    gbuffer->depth.width = width;
    gbuffer->depth.height = height;
    gbuffer->depth.format = UNCOMPRESSED_R32;
    gbuffer->depth.mipmaps = 1;

    rlFramebufferAttach(gbuffer->id, gbuffer->color.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D);
    rlFramebufferAttach(gbuffer->id, gbuffer->normal.id, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D);
    rlFramebufferAttach(gbuffer->id, gbuffer->position.id, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D);
    rlFramebufferAttach(gbuffer->id, gbuffer->depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D);
}

void gbuffer_begin(gbuffer_t* gbuffer)
{
    rlglDraw();
    
    rlEnableFramebuffer(gbuffer->id);
    
    rlClearScreenBuffers();
    
    rlViewport(0, 0, gbuffer->width, gbuffer->height);
    
    rlMatrixMode(RL_PROJECTION);
    rlLoadIdentity();
    
    rlOrtho(0, gbuffer->width, gbuffer->height, 0, 0, 1);
    
    rlMatrixMode(RL_MODELVIEW);
    rlLoadIdentity();
}

void gbuffer_end()
{
    rlglDraw();
    
    rlDisableTexture();         // Unbind texture
    rlDisableFramebuffer();
    
    rlViewport(0, 0, GetScreenWidth(), GetScreenHeight());
    
    rlMatrixMode(RL_PROJECTION);
    rlLoadIdentity();
    
    rlOrtho(0, GetScreenWidth(), GetScreenHeight(), 0, 0, 1);
    
    rlMatrixMode(RL_MODELVIEW);
    rlLoadIdentity();
}

void gbuffer_free(gbuffer_t* gbuffer)
{
    rlUnloadFramebuffer(gbuffer->id);   // Unload framebuffer (and automatically attached depth texture/renderbuffer)
}