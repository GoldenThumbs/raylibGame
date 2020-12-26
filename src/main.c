#include "raylib.h"
#include "raymath.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    Camera camera;
    camera.position = (Vector3){0, 0.5f, 0};
    camera.target = (Vector3){0, 0.5f, 1.0};
    camera.up = (Vector3){0, 1.0, 0};
    camera.type = CAMERA_PERSPECTIVE;
    camera.fovy = 85;

    Image image = LoadImage("assets/cubicmap.png");
    Mesh mesh = GenMeshCubicmap(image, Vector3One());
    Model model = LoadModelFromMesh(mesh);

    Texture2D texture = LoadTexture("assets/cubicmap_atlas.png");    // Load map texture
    GenTextureMipmaps(&texture);
    SetTextureFilter(texture, FILTER_ANISOTROPIC_8X);
    model.materials[0].maps[MAP_DIFFUSE].texture = texture;             // Set map diffuse texture

    Vector3 modelPos = (Vector3){-16.0f, 0.0f, -8.0f};

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

                DrawModel(model, modelPos, 1.0, WHITE);

            EndMode3D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(model);
    UnloadTexture(texture);

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}