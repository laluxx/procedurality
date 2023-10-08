#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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



// forward declarations
float Lerp(float start, float end, float amount);
void DrawDynamicGrid(Camera2D camera, bool showGrid);
Vector2 Vector2SubtractCustom(Vector2 v1, Vector2 v2);
Vector2 Vector2AddCustom(Vector2 v1, Vector2 v2);
Vector2 Vector2ScaleCustom(Vector2 v, float scale);
float Clamp(float value, float min, float max);
int GetGridSpacing(float zoom);
void DrawRuler(Camera2D camera, int ShowRuler);
void UpdateGridVariables(Camera2D camera);
void DrawCenter(Camera2D camera);
void DrawCoordinates(Camera2D camera);

float Lerp(float start, float end, float amount) {
    return start + amount * (end - start);
}

int gridSizeFactor = 6;
int ShowRuler = 1;



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


        UpdateGridVariables(camera);
        BeginMode2D(camera);
        DrawDynamicGrid(camera, showGrid);

        EndMode2D();

        DrawRuler(camera, ShowRuler);
        DrawCoordinates(camera);


        DrawText("Hold Middle Mouse Button to Pan. Scroll to Zoom. Press G to toggle grid.", 10, SCREEN_HEIGHT - 20, 10, INFO_TEXT_COLOR);

        /* DrawText(TextFormat("Camera: [ %.2f , %.2f ]", camera.target.x, camera.target.y), 40, 40, 20, INFO_TEXT_COLOR); */







        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Grid Global variables
int gridSpacing;
int gridVisibleLinesX;
int gridVisibleLinesY;
Vector2 gridCameraSpace;
int gridOffsetX;
int gridOffsetY;
Color gridColor;


// TODO separate the grid into its hown submodule []
void UpdateGridVariables(Camera2D camera) {
    gridSpacing = GetGridSpacing(camera.zoom);
    gridVisibleLinesX = gridSizeFactor * (SCREEN_WIDTH / gridSpacing) + 2;
    gridVisibleLinesY = gridSizeFactor * (SCREEN_HEIGHT / gridSpacing) + 2;
    gridCameraSpace = (Vector2){
        .x = camera.target.x - (SCREEN_WIDTH / 2) * camera.zoom,
        .y = camera.target.y - (SCREEN_HEIGHT / 2) * camera.zoom
    };
    gridOffsetX = ((int)gridCameraSpace.x % gridSpacing);
    gridOffsetY = ((int)gridCameraSpace.y % gridSpacing);
    gridColor = GRID_COLOR;
    gridColor.a = (unsigned char)Clamp(255.0f * (1.0f - (float)gridSpacing/GRID_LEVELS[GRID_LEVEL_COUNT - 1]), 50, 255);
}

void DrawDynamicGrid(Camera2D camera, bool showGrid) {
    if (!showGrid) return;

    for (int i = -gridVisibleLinesX; i < gridVisibleLinesX; i++) {
        int x = i * gridSpacing - gridOffsetX;
        DrawLineEx((Vector2){x, -SCREEN_HEIGHT * gridSizeFactor}, (Vector2){x, SCREEN_HEIGHT * gridSizeFactor}, GRID_LINE_WIDTH, gridColor);
    }

    for (int i = -gridVisibleLinesY; i < gridVisibleLinesY; i++) {
        int y = i * gridSpacing - gridOffsetY;
        DrawLineEx((Vector2){-SCREEN_WIDTH * gridSizeFactor, y}, (Vector2){SCREEN_WIDTH * gridSizeFactor, y}, GRID_LINE_WIDTH, gridColor);
    }
}

void DrawRuler(Camera2D camera, int showRuler) {
    if (!showRuler) return;

    for (int i = -gridVisibleLinesX; i < gridVisibleLinesX; i++) {
        int x = i * gridSpacing - gridOffsetX;
        char* text = TextFormat("%d", (int)(gridCameraSpace.x + i * gridSpacing));
        int textWidth = MeasureText(text, 10);
        DrawText(text, x - textWidth / 2, 10, 10, Fade(GRID_COLOR, 0.7f));
        DrawText("|", x, 20, 10, Fade(GRID_COLOR, 0.7f));
    }

    for (int i = -gridVisibleLinesY; i < gridVisibleLinesY; i++) {
        int y = i * gridSpacing - gridOffsetY;
        char* text = TextFormat("%d", (int)(gridCameraSpace.y + i * gridSpacing));
        int textWidth = MeasureText(text, 10);
        DrawText(text, SCREEN_WIDTH - 60 - textWidth, y, 10, Fade(GRID_COLOR, 0.7f));
        DrawText("--", SCREEN_WIDTH - 20, y, 10, Fade(GRID_COLOR, 0.7f));
    }
}


void DrawCoordinates(Camera2D camera) {
    char xValue[20];
    char yValue[20];
    sprintf(xValue, "%.2f", camera.target.x);
    sprintf(yValue, "%.2f", camera.target.y);

    int xOffset = 40;
    int yOffset = 1035;
    int fontSize = 20;

    DrawText("   x :  ", xOffset, yOffset, fontSize, INFO_TEXT_COLOR);
    xOffset += MeasureText("   x :  ", fontSize);

    DrawText(xValue, xOffset, yOffset, fontSize, RED);
    xOffset += MeasureText(xValue, fontSize);

    DrawText("   y :  ", xOffset, yOffset, fontSize, INFO_TEXT_COLOR);
    xOffset += MeasureText("   y :  ", fontSize);

    DrawText(yValue, xOffset, yOffset, fontSize, BLUE);
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


// TODO move into common.c
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
