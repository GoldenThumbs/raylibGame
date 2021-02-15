#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "gbuffer.h"

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

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
    //shader.locs[LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    model.materials[0].shader = shader;

    Texture2D mapTexture = LoadTexture(ASSETS_PATH"MapAtlas.dds");
    model.materials[0].maps[MAP_ALBEDO].texture = mapTexture;

    Vector3 modelPos = (Vector3) { -8.0, 0.0, -4.0 };

    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    gbuffer_t gbuffer = gbuffer_new(screenWidth, screenHeight);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);

        //float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        //SetShaderValue(shader, shader.locs[LOC_VECTOR_VIEW], cameraPos, UNIFORM_VEC3);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(DARKGRAY);

            gbuffer_begin(&gbuffer);
                BeginMode3D(camera);
                    DrawModel(model, modelPos, 1.0, BROWN);
                EndMode3D();
            gbuffer_end();

            DrawTexturePro(gbuffer.position, (Rectangle){0, 0, gbuffer.width, -gbuffer.height}, (Rectangle){0, 0, gbuffer.width, gbuffer.height}, Vector2Zero(), 0.0f, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(model);
    UnloadShader(shader);
    UnloadTexture(mapTexture);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}