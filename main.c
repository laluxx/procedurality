#include "raylib.h"
#include <math.h>
#include <stdlib.h>

// Screen Configurations
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BACKGROUND_COLOR (Color){40, 35, 30, 255}
#define INFO_TEXT_COLOR RAYWHITE

// Camera Configurations
#define MAX_ZOOM 3.0f
#define MIN_ZOOM 0.5f
#define CAMERA_INIT_ZOOM 1.0f
#define CAMERA_INIT_TARGET (Vector2){0, 0}
#define ZOOM_SPEED 0.05f

// Grid Configurations
#define GRID_SPACING 40
#define GRID_EXTENSION_FACTOR 20
#define GRID_LINE_WIDTH 2.0f
#define GRID_COLOR Fade((Color){100, 90, 80, 255}, 0.6f)

// Lerp Configuration
#define LERP_FACTOR 0.01f

// Grid Levels
#define GRID_LEVEL_COUNT 4
const int GRID_LEVELS[GRID_LEVEL_COUNT] = { 10, 20, 50, 100 };

float Lerp(float start, float end, float amount) {
    return start + amount * (end - start);
}

// forward declarations
float Lerp(float start, float end, float amount);
void DrawDynamicGrid(Camera2D camera, bool showGrid);
Vector2 Vector2SubtractCustom(Vector2 v1, Vector2 v2);
Vector2 Vector2AddCustom(Vector2 v1, Vector2 v2);
Vector2 Vector2ScaleCustom(Vector2 v, float scale);
float Clamp(float value, float min, float max);
int GetGridSpacing(float zoom);


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C Project Visualization");

    Camera2D camera = {0};
    camera.zoom = CAMERA_INIT_ZOOM;
    camera.target = CAMERA_INIT_TARGET;
    float desiredZoom = CAMERA_INIT_ZOOM;

    Vector2 dragStart = {0};
    bool isDragging = false;
    bool showGrid = true;

    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        Vector2 mousePosWorldBeforeZoom = GetScreenToWorld2D(mousePos, camera);

        int mouseWheelMove = GetMouseWheelMove();
        desiredZoom += mouseWheelMove * ZOOM_SPEED;
        desiredZoom = Clamp(desiredZoom, MIN_ZOOM, MAX_ZOOM);

        camera.zoom = Lerp(camera.zoom, desiredZoom, LERP_FACTOR);

        Vector2 mousePosWorldAfterZoom = GetScreenToWorld2D(mousePos, camera);
        camera.target.x += mousePosWorldBeforeZoom.x - mousePosWorldAfterZoom.x;
        camera.target.y += mousePosWorldBeforeZoom.y - mousePosWorldAfterZoom.y;

        if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
            if (!isDragging) {
                dragStart = GetMousePosition();
                isDragging = true;
            } else {
                Vector2 dragPosition = GetMousePosition();
                Vector2 dragDelta = Vector2SubtractCustom(dragStart, dragPosition);
                camera.target = Vector2AddCustom(camera.target, Vector2ScaleCustom(dragDelta, 1.0f / camera.zoom));
                dragStart = dragPosition;
            }
        } else {
            isDragging = false;
        }

        if (IsKeyPressed(KEY_G)) {
            showGrid = !showGrid;
        }

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        BeginMode2D(camera);
        DrawDynamicGrid(camera, showGrid);
        EndMode2D();

        DrawText("Hold Middle Mouse Button to Pan. Scroll to Zoom. Press G to toggle grid.", 10, SCREEN_HEIGHT - 20, 10, INFO_TEXT_COLOR);
        DrawText(TextFormat("Camera: [ %.2f , %.2f ]", camera.target.x, camera.target.y), 10, 10, 20, INFO_TEXT_COLOR);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// TODO separate into drawRuler and make variables like showruler []
// TODO separate the grid into its hown submodule []
// TODO the grid should be a perfect grid, and give the illusion of being infinite []
void DrawDynamicGrid(Camera2D camera, bool showGrid) {
    if (!showGrid) return;

    int spacing = GetGridSpacing(camera.zoom);
    int visibleLinesX = (SCREEN_WIDTH / spacing) + 2;
    int visibleLinesY = (SCREEN_HEIGHT / spacing) + 2;
    Vector2 cameraSpace = {
        .x = camera.target.x - (SCREEN_WIDTH / 2) * camera.zoom,
        .y = camera.target.y - (SCREEN_HEIGHT / 2) * camera.zoom
    };

    int offsetX = ((int)cameraSpace.x % spacing);
    int offsetY = ((int)cameraSpace.y % spacing);

    Color gridColor = GRID_COLOR;
    gridColor.a = (unsigned char)Clamp(255.0f * (1.0f - (float)spacing/GRID_LEVELS[GRID_LEVEL_COUNT - 1]), 50, 255);

    for (int i = -visibleLinesX; i < visibleLinesX; i++) {
        int x = i * spacing - offsetX;
        DrawLineEx((Vector2){x, -SCREEN_HEIGHT}, (Vector2){x, SCREEN_HEIGHT}, GRID_LINE_WIDTH, gridColor);
        DrawText(TextFormat("%d", (int)(cameraSpace.x + i * spacing)), x, 0, 10, Fade(GRID_COLOR, 0.7f));
    }

    for (int i = -visibleLinesY; i < visibleLinesY; i++) {
        int y = i * spacing - offsetY;
        DrawLineEx((Vector2){-SCREEN_WIDTH, y}, (Vector2){SCREEN_WIDTH, y}, GRID_LINE_WIDTH, gridColor);
        DrawText(TextFormat("%d", (int)(cameraSpace.y + i * spacing)), 0, y, 10, Fade(GRID_COLOR, 0.7f));
    }
}


int GetGridSpacing(float zoom) {
    float logZoom = logf(zoom + 1.0f);
    int spacing = (int)(Lerp(GRID_LEVELS[0], GRID_LEVELS[GRID_LEVEL_COUNT - 1], logZoom));

    int closestSpacing = GRID_LEVELS[0];
    for (int i = 1; i < GRID_LEVEL_COUNT; i++) {
        if (abs(spacing - GRID_LEVELS[i]) < abs(spacing - closestSpacing)) {
            closestSpacing = GRID_LEVELS[i];
        }
    }
    return closestSpacing;
}


// TODO move into commons.c
Vector2 Vector2SubtractCustom(Vector2 v1, Vector2 v2) {
    Vector2 result = { v1.x - v2.x, v1.y - v2.y };
    return result;
}

Vector2 Vector2AddCustom(Vector2 v1, Vector2 v2) {
    Vector2 result = { v1.x + v2.x, v1.y + v2.y };
    return result;
}

Vector2 Vector2ScaleCustom(Vector2 v, float scale) {
    Vector2 result = { v.x * scale, v.y * scale };
    return result;
}

float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}










// BASE
/* #include "raylib.h" */

/* // Screen Configurations */
/* #define SCREEN_WIDTH 1920 */
/* #define SCREEN_HEIGHT 1080 */
/* #define BACKGROUND_COLOR (Color){40, 35, 30, 255} */
/* #define INFO_TEXT_COLOR RAYWHITE */

/* // Camera Configurations */
/* #define MAX_ZOOM 3.0f */
/* #define MIN_ZOOM 0.5f */
/* #define CAMERA_INIT_ZOOM 1.0f */
/* #define CAMERA_INIT_TARGET (Vector2){0, 0} */
/* #define ZOOM_SPEED 0.05f */

/* // Grid Configurations */
/* #define GRID_SPACING 40 */
/* #define GRID_EXTENSION_FACTOR 20 */
/* #define GRID_COLOR Fade((Color){100, 90, 80, 255}, 0.6f) */

/* void DrawDynamicGrid(Camera2D camera, bool showGrid); */
/* Vector2 Vector2SubtractCustom(Vector2 v1, Vector2 v2); */
/* Vector2 Vector2AddCustom(Vector2 v1, Vector2 v2); */
/* Vector2 Vector2ScaleCustom(Vector2 v, float scale); */
/* float Clamp(float value, float min, float max); */

/* int main(void) { */
/*     InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C Project Visualization"); */

/*     Camera2D camera = {0}; */
/*     camera.zoom = CAMERA_INIT_ZOOM; */
/*     camera.target = CAMERA_INIT_TARGET; */

/*     Vector2 dragStart = {0}; */
/*     bool isDragging = false; */
/*     bool showGrid = true; */

/*     SetTargetFPS(144); */

/*     while (!WindowShouldClose()) { */
/*         Vector2 mousePos = GetMousePosition(); */
/*         Vector2 mousePosWorldBeforeZoom = GetScreenToWorld2D(mousePos, camera); */

/*         int mouseWheelMove = GetMouseWheelMove(); */
/*         camera.zoom += mouseWheelMove * ZOOM_SPEED; */
/*         camera.zoom = Clamp(camera.zoom, MIN_ZOOM, MAX_ZOOM); */

/*         Vector2 mousePosWorldAfterZoom = GetScreenToWorld2D(mousePos, camera); */
/*         camera.target.x += mousePosWorldBeforeZoom.x - mousePosWorldAfterZoom.x; */
/*         camera.target.y += mousePosWorldBeforeZoom.y - mousePosWorldAfterZoom.y; */


/*         if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) { */
/*             if (!isDragging) { */
/*                 dragStart = GetMousePosition(); */
/*                 isDragging = true; */
/*             } else { */
/*                 Vector2 dragPosition = GetMousePosition(); */
/*                 Vector2 dragDelta = Vector2SubtractCustom(dragStart, dragPosition); */
/*                 camera.target = Vector2AddCustom(camera.target, Vector2ScaleCustom(dragDelta, 1.0f / camera.zoom)); */
/*                 dragStart = dragPosition; */
/*             } */
/*         } else { */
/*             isDragging = false; */
/*         } */

/*         if (IsKeyPressed(KEY_G)) { */
/*             showGrid = !showGrid; */
/*         } */

/*         BeginDrawing(); */
/*         ClearBackground(BACKGROUND_COLOR); */

/*         BeginMode2D(camera); */
/*         DrawDynamicGrid(camera, showGrid); */
/*         EndMode2D(); */

/*         DrawText("Hold Middle Mouse Button to Pan. Scroll to Zoom. Press G to toggle grid.", 10, SCREEN_HEIGHT - 20, 10, INFO_TEXT_COLOR); */

/*         EndDrawing(); */
/*     } */

/*     CloseWindow(); */
/*     return 0; */
/* } */

/* void DrawDynamicGrid(Camera2D camera, bool showGrid) { */
/*     if (!showGrid) return; */

/*     int spacing = GRID_SPACING; */
/*     int visibleLinesX = SCREEN_WIDTH / spacing * GRID_EXTENSION_FACTOR; */
/*     int visibleLinesY = SCREEN_HEIGHT / spacing * GRID_EXTENSION_FACTOR; */

/*     float scaledSpacing = spacing * camera.zoom; */

/*     Vector2 cameraSpace = { */
/*         .x = camera.target.x - (SCREEN_WIDTH / 2) * camera.zoom, */
/*         .y = camera.target.y - (SCREEN_HEIGHT / 2) * camera.zoom */
/*     }; */

/*     int offsetX = ((int)cameraSpace.x % (int)scaledSpacing); */
/*     int offsetY = ((int)cameraSpace.y % (int)scaledSpacing); */

/*     for (int i = -visibleLinesX; i < visibleLinesX; i++) { */
/*         int x = i * spacing - offsetX; */
/*         DrawLine(x, -SCREEN_HEIGHT * GRID_EXTENSION_FACTOR, x, SCREEN_HEIGHT * GRID_EXTENSION_FACTOR, GRID_COLOR); */
/*     } */

/*     for (int i = -visibleLinesY; i < visibleLinesY; i++) { */
/*         int y = i * spacing - offsetY; */
/*         DrawLine(-SCREEN_WIDTH * GRID_EXTENSION_FACTOR, y, SCREEN_WIDTH * GRID_EXTENSION_FACTOR, y, GRID_COLOR); */
/*     } */
/* } */

/* Vector2 Vector2SubtractCustom(Vector2 v1, Vector2 v2) { */
/*     Vector2 result = { v1.x - v2.x, v1.y - v2.y }; */
/*     return result; */
/* } */

/* Vector2 Vector2AddCustom(Vector2 v1, Vector2 v2) { */
/*     Vector2 result = { v1.x + v2.x, v1.y + v2.y }; */
/*     return result; */
/* } */

/* Vector2 Vector2ScaleCustom(Vector2 v, float scale) { */
/*     Vector2 result = { v.x * scale, v.y * scale }; */
/*     return result; */
/* } */

/* float Clamp(float value, float min, float max) { */
/*     if (value < min) return min; */
/*     if (value > max) return max; */
/*     return value; */
/* } */
