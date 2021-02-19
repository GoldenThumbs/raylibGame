#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "gbuffer.h"

#define MAX_LIGHTS 64

typedef struct Light
{
    Vector3 position;
    Color color;
    float radius;
} Light;

Light Lights[MAX_LIGHTS];

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    gbuffer_t gBuffer = gbuffer_new(screenWidth, screenHeight);

    Shader deferredLight = LoadShader(0, ASSETS_PATH"shaders/deferred.fs");
    deferredLight.locs[LOC_VECTOR_VIEW] = GetShaderLocation(deferredLight, "viewPos");
    int lightLocs[3];
    lightLocs[0] = GetShaderLocation(deferredLight, "lightPos");
    lightLocs[1] = GetShaderLocation(deferredLight, "lightCol");
    lightLocs[2] = GetShaderLocation(deferredLight, "lightRadius");
    int gbufferLocs[3];
    gbufferLocs[0] = GetShaderLocation(deferredLight, "colorbuffer");
    gbufferLocs[1] = GetShaderLocation(deferredLight, "normalbuffer");
    gbufferLocs[2] = GetShaderLocation(deferredLight, "positionbuffer");

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        Lights[i].position = (Vector3){ (float)GetRandomValue(-256, 256) / 32, 0.5f, (float)GetRandomValue(-256, 256) / 64 };
        Lights[i].color = (Color){ GetRandomValue(100, 255), GetRandomValue(100, 255), GetRandomValue(100, 255), 255 };
        Lights[i].radius = 1.5f;
    }

    Camera camera;
    camera.position = (Vector3) { 0, 0.5, 0 };
    camera.target = (Vector3) { 0, 0.5, 1.0 };
    camera.up = (Vector3) { 0, 1.0, 0 };
    camera.type = CAMERA_PERSPECTIVE;
    camera.fovy = 85;

    Image map = LoadImage(ASSETS_PATH"map.png");
    Mesh mesh = GenMeshCubicmap(map, Vector3One());
    Model model = LoadModelFromMesh(mesh);
    UnloadImage(map);
    
    Shader shader = LoadShader(ASSETS_PATH"shaders/geom.vs", ASSETS_PATH"shaders/geom.fs");
    shader.locs[LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
    model.materials[0].shader = shader;

    Texture2D mapTexture = LoadTexture(ASSETS_PATH"MapAtlas.dds");
    model.materials[0].maps[MAP_ALBEDO].texture = mapTexture;

    Vector3 modelPos = (Vector3) { -8.0, 0.0, -4.0 };

    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);

        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(deferredLight, deferredLight.locs[LOC_VECTOR_VIEW], cameraPos, UNIFORM_VEC3);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            //ClearBackground(DARKGRAY);
            gbuffer_begin(gBuffer);
                ClearBackground(DARKGRAY);

                BeginMode3D(camera);
                    DrawModel(model, modelPos, 1.0, BROWN);
                EndMode3D();
            gbuffer_end();

            for (int i = 0; i < MAX_LIGHTS; i++)
            {
                if (i != 0) BeginBlendMode(BLEND_ADDITIVE);
                BeginShaderMode(deferredLight);
                    SetShaderValueTexture(deferredLight, gbufferLocs[0], gBuffer.color);
                    SetShaderValueTexture(deferredLight, gbufferLocs[1], gBuffer.normal);
                    SetShaderValueTexture(deferredLight, gbufferLocs[2], gBuffer.position);
                    SetShaderValue(deferredLight, lightLocs[0], (float*)&Lights[i].position, UNIFORM_VEC3);
                    Vector4 LightColor = ColorNormalize(Lights[i].color);
                    SetShaderValue(deferredLight, lightLocs[1], (float*)&LightColor, UNIFORM_VEC4);
                    SetShaderValue(deferredLight, lightLocs[2], &Lights[i].radius, UNIFORM_FLOAT);

                    DrawTextureRec(gBuffer.color, (Rectangle){ 0.0, 0.0, (float)screenWidth, -(float)screenHeight }, (Vector2){ 0.0, 0.0 }, BLUE);
                EndShaderMode();
                if (i != 0) EndBlendMode();
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(deferredLight);
    UnloadModel(model);
    UnloadShader(shader);
    UnloadTexture(mapTexture);
    gbuffer_free(gBuffer);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}