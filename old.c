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
