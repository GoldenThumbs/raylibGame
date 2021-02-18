#include "gbuffer.h"

gbuffer_t gbuffer_new(int width, int height)
{
    gbuffer_t gbuffer;
    gbuffer.id = 0;
    gbuffer.width = width;
    gbuffer.height = height;

    gbuffer.color.id = 0;
    gbuffer.color.width = width;
    gbuffer.color.height = height;
    gbuffer.color.format = UNCOMPRESSED_R8G8B8A8;
    gbuffer.color.mipmaps = 0;

    gbuffer.normal.id = 0;
    gbuffer.normal.width = width;
    gbuffer.normal.height = height;
    gbuffer.normal.format = UNCOMPRESSED_R8G8B8A8;
    gbuffer.normal.mipmaps = 0;

    gbuffer.position.id = 0;
    gbuffer.position.width = width;
    gbuffer.position.height = height;
    gbuffer.position.format = UNCOMPRESSED_R8G8B8A8;
    gbuffer.position.mipmaps = 0;

    gbuffer.depth.id = 0;
    gbuffer.depth.width = width;
    gbuffer.depth.height = height;
    gbuffer.depth.format = 19;
    gbuffer.depth.mipmaps = 0;

    gbuffer.id = rlLoadFramebuffer(width, height);
    rlEnableFramebuffer(gbuffer.id);

    gbuffer.color.id = rlLoadTexture(NULL, width, height, UNCOMPRESSED_R8G8B8A8, 1);
    rlTextureParameters(gbuffer.color.id, RL_TEXTURE_MIN_FILTER, RL_FILTER_NEAREST);
    rlTextureParameters(gbuffer.color.id, RL_TEXTURE_MAG_FILTER, RL_FILTER_NEAREST);
    rlFramebufferAttach(gbuffer.id, gbuffer.color.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D);

    gbuffer.normal.id = rlLoadTexture(NULL, width, height, UNCOMPRESSED_R8G8B8A8, 1);
    rlTextureParameters(gbuffer.normal.id, RL_TEXTURE_MIN_FILTER, RL_FILTER_NEAREST);
    rlTextureParameters(gbuffer.normal.id, RL_TEXTURE_MAG_FILTER, RL_FILTER_NEAREST);
    rlFramebufferAttach(gbuffer.id, gbuffer.normal.id, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D);

    gbuffer.position.id = rlLoadTexture(NULL, width, height, UNCOMPRESSED_R8G8B8A8, 1);
    rlTextureParameters(gbuffer.position.id, RL_TEXTURE_MIN_FILTER, RL_FILTER_NEAREST);
    rlTextureParameters(gbuffer.position.id, RL_TEXTURE_MAG_FILTER, RL_FILTER_NEAREST);
    rlFramebufferAttach(gbuffer.id, gbuffer.position.id, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D);

    gbuffer.depth.id = rlLoadTextureDepth(width, height, false);
    rlTextureParameters(gbuffer.depth.id, RL_TEXTURE_WRAP_S, RL_WRAP_CLAMP);
    rlTextureParameters(gbuffer.depth.id, RL_TEXTURE_WRAP_T, RL_WRAP_CLAMP);
    rlTextureParameters(gbuffer.depth.id, RL_TEXTURE_MIN_FILTER, RL_FILTER_NEAREST);
    rlTextureParameters(gbuffer.depth.id, RL_TEXTURE_MAG_FILTER, RL_FILTER_NEAREST);
    rlFramebufferAttach(gbuffer.id, gbuffer.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D);

    rlDisableFramebuffer();

    if (!rlFramebufferComplete(gbuffer.id))
    {
        TraceLog(LOG_WARNING, "GBuffer could not be created!");
    }else TraceLog(LOG_INFO, "GBuffer created successfully.");

    return gbuffer;
}

void gbuffer_begin(gbuffer_t gbuffer)
{
    rlglDraw();                         // Draw Buffers (Only OpenGL 3+ and ES2)
    rlEnableFramebuffer(gbuffer.id);     // Enable render target

    rlClearScreenBuffers();

    // Set viewport to framebuffer size
    rlViewport(0, 0, gbuffer.width, gbuffer.height);

    rlMatrixMode(RL_PROJECTION);        // Switch to projection matrix
    rlLoadIdentity();                   // Reset current matrix (projection)

    // Set orthographic projection to current framebuffer size
    // NOTE: Configured top-left corner as (0, 0)
    rlOrtho(0, gbuffer.width, gbuffer.height, 0, 0.0f, 1.0f);

    rlMatrixMode(RL_MODELVIEW);         // Switch back to modelview matrix
    rlLoadIdentity();                   // Reset current matrix (modelview)
}

void gbuffer_end()
{
    rlglDraw();
        
    rlDisableFramebuffer();
    
    rlViewport(0, 0, GetScreenWidth(), GetScreenHeight());
    
    rlMatrixMode(RL_PROJECTION);
    rlLoadIdentity();
    
    rlOrtho(0, GetScreenWidth(), GetScreenHeight(), 0, 0, 1);
    
    rlMatrixMode(RL_MODELVIEW);
    rlLoadIdentity();
}

void gbuffer_free(gbuffer_t gbuffer)
{
    rlUnloadFramebuffer(gbuffer.id);   // Unload framebuffer (and automatically attached depth texture/renderbuffer)
    rlUnloadTexture(gbuffer.color.id);
    rlUnloadTexture(gbuffer.normal.id);
    rlUnloadTexture(gbuffer.position.id);
    rlUnloadTexture(gbuffer.depth.id);
}