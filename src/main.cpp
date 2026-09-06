#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <cmath>

// ============================================
// GRAVITY SHEET 
// 3D grid with gravitational deformation
// ============================================

int main(){
    // --- Window Setup ---
    const int screenWidth = 1024;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "Gravity Sheet");

    // --- Camera Setup ---
    Camera3D camera = {0};
    camera.position = (Vector3){15.0f, 12.0f, 15.0f};
    camera.target = (Vector3){0.0f,0.0f,0.0f};
    camera.up = (Vector3){0.0f,1.0f,0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // --- Grid Settings ---
    const int gridSize = 30;
    const float spacing = 1.0f;
    

    // Create a 2D vector to store grid points
    std::vector<std::vector<Vector3>> grid;
    grid.resize(gridSize);
    for (int i=0;i<gridSize;i++){
        grid[i].resize(gridSize);
    }

    // Initialize grid to flat (y=0)
    for (int i=0;i<gridSize;i++){
        for (int j = 0; j < gridSize; j++) {
            float x = (i - gridSize/2) * spacing;
            float z = (j - gridSize/2) * spacing;
            grid[i][j] = (Vector3){ x, 0.0f, z };
        }
    }

    // --- Planet Settings ---
    Vector3 planetPos = {0.0f,0.0f,0.0f};
    float planetMass = 8.0f;
    float planetRadius = 2.5f;

    SetTargetFPS(60);

    // --- Main Game Loop ---
    while (!WindowShouldClose()){
        // Camera Control
        UpdateCamera(&camera, CAMERA_ORBITAL);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            
            // Orbit around target
            Vector3 direction = Vector3Subtract(camera.position, camera.target);
            float length = Vector3Length(direction);
            direction = Vector3Normalize(direction);
            
            // Get right and up vectors
            Vector3 right = Vector3CrossProduct(direction, camera.up);
            right = Vector3Normalize(right);
            Vector3 up = Vector3CrossProduct(right, direction);
            up = Vector3Normalize(up);
            
            // Rotate
            float angleX = -delta.x * 0.01f;
            float angleY = -delta.y * 0.01f;
            
            // Apply rotation around up vector
            Vector3 rotated = Vector3RotateByAxisAngle(direction, up, angleX);
            rotated = Vector3RotateByAxisAngle(rotated, right, angleY);
            rotated = Vector3Normalize(rotated);
            
            camera.position = Vector3Add(camera.target, Vector3Scale(rotated, length));
        }

        // Zoom with scroll wheel
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector3 direction = Vector3Subtract(camera.position, camera.target);
            float length = Vector3Length(direction);
            length -= wheel * 1.5f;
            if (length < 2.0f) length = 2.0f;
            if (length > 50.0f) length = 50.0f;
            camera.position = Vector3Add(camera.target, Vector3Scale(Vector3Normalize(direction), length));
        }

        // Update grid deformation (real gravity)
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                float x = grid[i][j].x;
                float z = grid[i][j].z;

                // Gravitational potential z = -G * M / r
                float dx = x - planetPos.x;
                float dz = z - planetPos.z;
                float dist  = sqrtf(dx*dx + dz*dz + 0.5f); // +0.5 to avoid division by zero
                float height = -planetMass / dist; // Gravity deformation
                grid[i][j].y = height;
            }
        }
    

    // --- DRAW ---
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
            // Draw the grid as lines
            for (int i = 0; i < gridSize - 1; i++){
                for (int j = 0; j < gridSize - 1; j++){
                    // Draw horizontal line
                    DrawLine3D(grid[i][j],grid[i+1][j],DARKGRAY);
                    // Draw vertical line
                    DrawLine3D(grid[i][j],grid[i][j+1],DARKGRAY);

                }
            }

            // Draw grid points as dots
            for (int i = 0; i < gridSize - 1; i++){
                for (int j = 0; j < gridSize - 1; j++){
                    DrawPoint3D(grid[i][j],DARKGRAY);
                }
            }

            // Draw the planet as a sphere
            // Find the height of the grid at planet position
            float planetHeight = 0.0f;
            int pi = (int)(planetPos.x / spacing) + gridSize/2;
            int pj = (int)(planetPos.z / spacing) + gridSize/2;
            if (pi >= 0 && pi < gridSize && pj >=0 && pj<gridSize){
                planetHeight = grid[pi][pj].y;
            }
            float planetY = 2.0f;
            DrawSphere((Vector3){planetPos.x,planetY,planetPos.z},planetRadius,YELLOW);
            
        EndMode3D();

            // --- UI ---
            DrawFPS(10,10);
            DrawText("GRAVITY SHEET",10,30,20,DARKGRAY);
            DrawText("Drag mouse to orbit | Scroll to zoom",10,55,15,DARKGRAY);
            DrawText(("Grid: "+std::to_string(gridSize)+"x"+std::to_string(gridSize)).c_str(),10,80,15,DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}