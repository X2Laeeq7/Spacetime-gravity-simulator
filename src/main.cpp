#include "raylib.h"
#include "raymath.h"
#include "CelestialBody.cpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>

// ============================================
// GRAVITY SHEET 
// 3D grid with gravitational deformation
// ============================================

int main(int argc,char* argv[]){
    bool showBlackHole = false;
    bool showSolarSystem = false;

    if (argc > 1){
        if (strcmp(argv[1], "blackhole") == 0) {
            showBlackHole = true;
            std::cout << "Running BLACK HOLE simulation" << std::endl;
        } else if (strcmp(argv[1], "solar") == 0) {
            showSolarSystem = true;
            std::cout << "Running SOLAR SYSTEM simulation" << std::endl;
        } else { // default
            showSolarSystem = true;
            std::cout << "Running SOLAR SYSTEM simulation (default)" << std::endl;
        }
    } else {
        showSolarSystem = true;
        std::cout << "Running SOLAR SYSTEM simulation (default)" << std::endl;
    }
    // --- Window Setup ---
    const int screenWidth = 1024;
    const int screenHeight = 768;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "Gravity Sheet");
    

    // --- Camera Setup ---
    Camera3D camera = {0};
    camera.target = (Vector3){0.0f,0.0f,0.0f};
    camera.up = (Vector3){0.0f,1.0f,0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_CUSTOM;
    camera.position = (Vector3){35.0f, 25.0f, 35.0f};

    // --- Grid Settings ---
    const int gridSize = 100;
    const float spacing = 1.0f;
    
    // Create grid
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

    // --- Create Celestial Bodies ---
    std::vector<CelestialBody> bodies;
    std::vector<CelestialBody> asteroids;
    if (showSolarSystem){
        // --- Solar System ---
        CelestialBody Sun = CelestialBody({0.0f,0.0f,0.0f},20.0f,3.0f,{250, 222, 133, 255},BodyType::STAR);
        Sun.setGravityRadius(1.2f);

        CelestialBody Mercury = CelestialBody({4.0f,0.0f,0.0f},0.5f,0.3f,{183, 184, 185,255},BodyType::PLANET);
        Mercury.startOrbiting(4.0f,0.04f,{0.0f, 0.0f, 0.0f});

        CelestialBody Venus = CelestialBody({7.0f,0.0f,0.0f},0.9,0.5,{245, 230, 195,255},BodyType::PLANET);
        Venus.startOrbiting(7.0f,0.025f,{0.0f, 0.0f, 0.0f});

        CelestialBody Earth = CelestialBody({10.0f,0.0f,0.0f},1.0f,0.6f,{77, 143, 234, 255},BodyType::PLANET);
        Earth.startOrbiting(10.0f,0.02f,{0.0f, 0.0f, 0.0f});

        CelestialBody Mars = CelestialBody({15.0f,0.0f,0.0f},0.8,0.5,{153, 61, 0,255},BodyType::PLANET);
        Mars.startOrbiting(15.0f,0.015f,{0.0f, 0.0f, 0.0f});

        CelestialBody Jupiter = CelestialBody({52.0f,0.0f,0.0f},5.0f,1.5f,{209, 167, 127,255},BodyType::PLANET);
        Jupiter.startOrbiting(52.0f,0.008f,{0.0f, 0.0f, 0.0f});

        CelestialBody Saturn = CelestialBody({95.0f,0.0f,0.0f},4.0,1.2,{250, 229, 191,255},BodyType::PLANET);
        Saturn.startOrbiting(95.0f,0.005f,{0.0f, 0.0f, 0.0f});

        bodies.push_back(Sun);
        bodies.push_back(Earth);
        bodies.push_back(Mercury);
        bodies.push_back(Venus);
        bodies.push_back(Mars);
        bodies.push_back(Jupiter);
        bodies.push_back(Saturn);

        // --- Asteroid Belt ---
        const int asteroidCount = 500;
        for (int i = 0; i < asteroidCount; i++) {
            float radius = 20.0f + (rand() % 35);;
            float angle = (rand() % 360) * DEG2RAD;
            float heightOffset = (rand() % 20 - 10) / 20.0f;
            Vector3 pos = {radius * cosf(angle),heightOffset * 2.0f,radius * sinf(angle)};
            float size = 0.05f + (rand() % 10) / 200.0f;
            float brightness = 0.3f + (rand() % 100) / 140.0f;
            unsigned char b = (unsigned char)(brightness * 255);
            
            CelestialBody asteroid(pos,0.001f, size,{b, b-20, b-40, 255},BodyType::ASTEROID);
            asteroids.push_back(asteroid);
        }
    }else if (showBlackHole){
        // --- BLACK HOLE ---
        CelestialBody BLACKHOLE = CelestialBody({0.0f,0.0f,0.0f},5000.0f,5.0f, {0, 0, 0, 255}, BodyType::BLACK_HOLE);

        CelestialBody MillersPlanet = CelestialBody({45.0f,0.0f,0.0f},1.3f,0.5f,{100,180,255,200},BodyType::PLANET);
        MillersPlanet.startOrbiting(45.0f,0.15f,{0.0f,0.0f,0.0f});

        bodies.push_back(BLACKHOLE);
        bodies.push_back(MillersPlanet);

    }
    
    // --- Background Stars --
    std::vector<CelestialBody> stars;
    const int starCount = 1500;
    srand((unsigned)time(NULL));

    for (int i=0;i<starCount;i++){
        float radius = 80 + rand()%120;
        float theta = (rand()%360) * DEG2RAD;
        float phi = (rand()%180) * DEG2RAD;
        Vector3 pos = {radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta)};
        int colorType = rand() % 100;
        Color starColor;
        if (colorType < 15) {      
            int b = 180 + (rand() % 75);
            starColor = {150, 200, (unsigned char)b, 255};
        } else if (colorType < 30) {
            starColor = {255, 230, 180, 255};
        } else if (colorType < 45) {
            int r = 200 + (rand() % 55);
            starColor = {(unsigned char)r, 160, 100, 255};
        } else if (colorType < 60) { // 
            starColor = {255, 180, 180, 255};
        } else if (colorType < 75) { 
            int b = 200 + (rand() % 55);
            starColor = {(unsigned char)b, (unsigned char)b, (unsigned char)b, 255};
        } else {                   
            int b = 200 + (rand() % 55);
            starColor = {180, 210, (unsigned char)b, 255};
        }
        float starRadius = 0.05f + (rand()%20)/500.0f;
        float brightness = 0.5f + (rand() % 100) / 150.0f;
        starColor.r = (unsigned char)(starColor.r * brightness);
        starColor.g = (unsigned char)(starColor.g * brightness);
        starColor.b = (unsigned char)(starColor.b * brightness);
        
        CelestialBody star(pos,0.0f,starRadius,starColor,BodyType::STAR);
        stars.push_back(star);
        
    }

    SetTargetFPS(60);

    // --- Main Game Loop ---
    while (!WindowShouldClose()){
        // Camera Control
        UpdateCamera(&camera, CAMERA_PERSPECTIVE);
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
            camera.fovy = 45.0f;
        }

        // Zoom with scroll wheel
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector3 direction = Vector3Subtract(camera.position, camera.target);
            float length = Vector3Length(direction);
            length -= wheel * 1.5f;
            if (length < 2.0f) length = 2.0f;
            if (length > 100.0f) length = 100.0f;
            camera.position = Vector3Add(camera.target, Vector3Scale(Vector3Normalize(direction), length));
        }
        
        // --- Update Orbits ---
        for (auto& body : bodies){body.updateOrbitPosition();}

        // Update grid deformation (real gravity)
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                float x = grid[i][j].x;
                float z = grid[i][j].z;
                float totalHeight = 0.0f;
                for (const auto& body : bodies){
                    float dx = x - body.getPosition().x;
                    float dz = z - body.getPosition().z;
                    float dist = sqrtf(dx*dx + dz*dz);
                    totalHeight += body.calculateDeformation(dist);
                }
                grid[i][j].y = totalHeight;
            }
        }

    // --- DRAW ---
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
            // Draw stars
            for (const auto& star : stars){
                Vector3 pos = star.getPosition();
                DrawPoint3D(pos,star.getColor());
            }


            // Draw the grid as lines
            for (int i = 0; i < gridSize - 1; i++){
                for (int j = 0; j < gridSize - 1; j++){
                    // Draw horizontal line
                    DrawLine3D(grid[i][j],grid[i+1][j],{90, 110, 150, 70});
                    // Draw vertical line
                    DrawLine3D(grid[i][j],grid[i][j+1],{90, 110, 150,70});

                    DrawTriangle3D(grid[i][j], grid[i+1][j], grid[i][j+1], {20, 20, 20, 200});
                    DrawTriangle3D(grid[i+1][j], grid[i+1][j+1], grid[i][j+1],{20, 20, 20, 200});

                }
            }
            
            // Draw grid points as dots
            for (int i = 0; i < gridSize - 1; i++){
                for (int j = 0; j < gridSize - 1; j++){
                    DrawPoint3D(grid[i][j],{90, 110, 150, 100});
                }
            }

            // Draw the bodies
            for (const auto& body : bodies){
                body.draw();
            }
            // --- DRAW ASTEROID BELT (3D) ---
            for (const auto& asteroid : asteroids) {
                asteroid.draw();
            }
            
        EndMode3D();

            // --- UI ---
            DrawFPS(10,10);
            if (showBlackHole){
                DrawText("BLACK HOLE SIMULATION",10,30,20,DARKGRAY);
            }else{
                DrawText("SOLAR SYSTEM SIMULATION",10,30,20,DARKGRAY);
            }
            DrawText("Drag mouse to orbit | Scroll to zoom",10,55,15,DARKGRAY);
            DrawText(("Grid: "+std::to_string(gridSize)+"x"+std::to_string(gridSize)).c_str(),10,80,15,DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}