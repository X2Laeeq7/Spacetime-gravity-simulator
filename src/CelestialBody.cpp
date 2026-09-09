// CelestialBody.cpp

#pragma once

#include "raylib.h"
#include <cmath>
#include "rlgl.h"

enum class BodyType {
    STAR,
    PLANET,
    BLACK_HOLE,
    MOON,
    ASTEROID,MP
};

class CelestialBody{
    private:
        // --- Properties ---
        Vector3 position;
        float mass;
        float radius;
        Color color;
        BodyType type;
        float softening;
        float stretch;
        float gravityRadius;

        // --- Orbital Properties
        float orbitalRadius;
        float orbitalSpeed;
        float orbitalAngle;
        bool isOrbiting;
        Vector3 parentPos;

        // --- Saturn Properties
        bool hasRings;
        Color ringColor;
        float ringInnerScale;
        float ringOuterScale;
        float ringTilt;

    public:
        // Constructor ---
        CelestialBody(Vector3 pos,float m,float r,Color c,BodyType t){
            position = pos;
            mass = m;
            radius = r;
            gravityRadius = r;
            color = c;
            type = t;
            stretch = 1.0f;
            softening = 0.5f;

            hasRings = false;
            ringColor = {200, 200, 200, 255};
            ringInnerScale = 1.5f;
            ringOuterScale = 2.3f;
            ringTilt = 20.0f;

            orbitalRadius = 0.0f;
            orbitalSpeed = 0.0f;
            orbitalAngle = 0.0f;
            isOrbiting = false;
            parentPos = {0.0f,0.0f,0.0f};

            // Customise based on type
            if (type == BodyType::STAR){
                stretch = 0.3f;
            }else if (type == BodyType::BLACK_HOLE){
                stretch = 0.1;
            }else if (type == BodyType::PLANET){
                stretch = 0.8f;
            }
        }
        // --- Getters ---
        Vector3 getPosition() const {return position;}
        float getMass() const {return mass;}
        float getRadius() const {return radius;}
        float getGravityRadius() const {return gravityRadius;}
        Color getColor() const {return color;}
        BodyType getType() const {return type;}

        // --- Setters ---
        void setPosition(Vector3 pos){position=pos;}
        void setMass(float m){mass = m;}
        void setRadius(float r){radius = r;}
        void setGravityRadius(float r){gravityRadius = r;}
        void setColor(Color c){color =c;}
        void setParentPosition(Vector3 newParent){parentPos = newParent;}

        // --- Physics ---
        float calculateDeformation(float dist) const{
            if (type == BodyType::BLACK_HOLE){
                float G = 1.0f;
                float r_s = 2.0f * G * mass;
                float height = -mass / (dist * dist + 1.0f);  
                return height;
            } else {
                float depthScale = mass * 1.2f;
                float falloffRadius = gravityRadius * 4.0f + 1.0f;
                float height = -depthScale / (1.0f + (dist * dist) / (falloffRadius * falloffRadius));
                return height;
            }
        }

        // --- Orbit ---
        void startOrbiting(float radius, float speed, Vector3 parent){
            orbitalRadius = radius;
            orbitalSpeed = speed;
            parentPos = parent;
            isOrbiting = true;
            orbitalAngle = 0.0f;

            updateOrbitPosition();
        }
        void updateOrbitPosition(){
            if (isOrbiting){
                orbitalAngle += orbitalSpeed;
                float newX = parentPos.x + orbitalRadius * cosf(orbitalAngle);
                float newZ = parentPos.z + orbitalRadius * sinf(orbitalAngle);
                position.x = newX;
                position.z = newZ;
            }
        }

        // --- Drawing --
        void addRings(Color c, float innerScale = 1.5f, float outerScale = 2.3f, float tilt = 20.0f){
            hasRings = true;
            ringColor = c;
            ringInnerScale = innerScale;
            ringOuterScale = outerScale;
            ringTilt = tilt;
        }

        void DrawDiskRing(Vector3 center, float innerRadius, float outerRadius,
                   float tiltAngleDeg, Vector3 tiltAxis, float spinDeg,
                   Color innerColor, Color outerColor, int segments) const
        {
            rlPushMatrix();
            rlTranslatef(center.x, center.y, center.z);
            rlRotatef(tiltAngleDeg, tiltAxis.x, tiltAxis.y, tiltAxis.z);
            rlRotatef(spinDeg, 0.0f, 1.0f, 0.0f); 

            rlBegin(RL_TRIANGLES);
            for (int i = 0; i < segments; i++)
            {
                float t0 = (float)i / segments * 2.0f * PI;
                float t1 = (float)(i + 1) / segments * 2.0f * PI;

                float xo0 = cosf(t0) * outerRadius, zo0 = sinf(t0) * outerRadius;
                float xo1 = cosf(t1) * outerRadius, zo1 = sinf(t1) * outerRadius;
                float xi0 = cosf(t0) * innerRadius, zi0 = sinf(t0) * innerRadius;
                float xi1 = cosf(t1) * innerRadius, zi1 = sinf(t1) * innerRadius;

                rlColor4ub(innerColor.r, innerColor.g, innerColor.b, innerColor.a);
                rlVertex3f(xi0, 0, zi0);
                rlColor4ub(outerColor.r, outerColor.g, outerColor.b, outerColor.a);
                rlVertex3f(xo0, 0, zo0);
                rlVertex3f(xo1, 0, zo1);

                rlColor4ub(innerColor.r, innerColor.g, innerColor.b, innerColor.a);
                rlVertex3f(xi0, 0, zi0);
                rlVertex3f(xi1, 0, zi1);
                rlColor4ub(outerColor.r, outerColor.g, outerColor.b, outerColor.a);
                rlVertex3f(xo1, 0, zo1);
            }
            rlEnd();

            rlPopMatrix();
        }

        void drawBlackHole(Vector3 pos) const {
            Vector3 tiltAxis = {1.0f, 0.0f, 0.0f};
            float tiltAngle = 0.0f; 
            float spinDeg = fmodf((float)GetTime() * 18.0f, 360.0f); 
            DrawSphere(pos, radius, BLACK);
            rlDisableBackfaceCulling();
            rlDisableDepthMask();
            DrawDiskRing(pos, radius * 1.5f, radius * 1.9f, tiltAngle, tiltAxis, spinDeg,
                         (Color){255, 255, 255, 230}, (Color){255, 235, 190, 190}, 64);
            DrawDiskRing(pos, radius * 1.9f, radius * 2.4f, tiltAngle, tiltAxis, spinDeg,
                         (Color){255, 235, 190, 190}, (Color){255, 200, 120, 150}, 64);
            DrawDiskRing(pos, radius * 2.4f, radius * 3.0f, tiltAngle, tiltAxis, spinDeg,
                         (Color){255, 200, 120, 150}, (Color){255, 150, 70, 110}, 64);
            DrawDiskRing(pos, radius * 3.0f, radius * 3.8f, tiltAngle, tiltAxis, spinDeg,
                         (Color){255, 150, 70, 110}, (Color){220, 90, 40, 60}, 64);
            DrawDiskRing(pos, radius * 3.8f, radius * 4.8f, tiltAngle, tiltAxis, spinDeg,
                         (Color){220, 90, 40, 60}, (Color){150, 50, 30, 0}, 64);
            rlEnableDepthMask();
            rlEnableBackfaceCulling();
        } 

        void draw() const{
            Vector3 pos = {position.x,position.y,position.z};

            if (type == BodyType::BLACK_HOLE){
                drawBlackHole(pos);
                return; 
            }

            // Draw body (stars, planets, moons)
            DrawSphere(pos,radius,color);

            // Glow for stars
            if (type == BodyType::STAR){
                DrawSphere(pos,radius * 1.1f,(Color){color.r,color.g,color.b,40});
            }

            if (hasRings){
                rlDisableBackfaceCulling();
                rlDisableDepthMask();
                Color ringOuterColor = ringColor;
                ringOuterColor.a = (unsigned char)(ringColor.a / 3);
                DrawDiskRing(pos, radius * ringInnerScale, radius * ringOuterScale,
                             ringTilt, (Vector3){1.0f, 0.0f, 0.0f}, 0.0f,
                             ringColor, ringOuterColor, 64);
                DrawDiskRing(pos, radius * ringOuterScale, radius * ringOuterScale*1.5,
                             ringTilt, (Vector3){1.0f, 0.0f, 0.0f}, 0.0f,
                             ringColor, ringOuterColor, 64);
                rlEnableDepthMask();
                rlEnableBackfaceCulling();
            }
        }

        
};
