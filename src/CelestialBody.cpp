// CelestialBody.cpp

#pragma once

#include "raylib.h"
#include <cmath>

enum class BodyType {
    STAR,
    PLANET,
    BLACK_HOLE,
    MOON
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

        // --- Orbital properties
        float orbitalRadius;
        float orbitalSpeed;
        float orbitalAngle;
        bool isOrbiting;
        Vector3 parentPos;

    public:
        // Constructor ---
        CelestialBody(Vector3 pos,float m,float r,Color c,BodyType t){
            position = pos;
            mass = m;
            radius = r;
            color = c;
            type = t;
            stretch = 1.0f;
            softening = 0.5f;

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
        Color getColor() const {return color;}
        BodyType getType() const {return type;}

        // --- Setters ---
        void setPosition(Vector3 pos){position=pos;}
        void setMass(float m){mass = m;}
        void setRadius(float r){radius = r;}
        void setColor(Color c){color =c;}
        void setParentPosition(Vector3 newParent){parentPos = newParent;}

        // --- Physics ---
        float calculateDeformation(float dist) const{
            if (type == BodyType::BLACK_HOLE){
                float G = 1.0f;
                float r_s = 2.0f * G * mass;
                if (dist > r_s){
                    return -2.0f * sqrtf(2.0f * G * mass * (dist - r_s));
                }else{
                    return -20.0f;
                }
            }else{
                return -mass / (dist * stretch + softening);
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
        void draw() const{
            Vector3 pos = {position.x,position.y,position.z};
            // Draw body
            DrawSphere(pos,radius,color);

            // Glow for stars
            if (type == BodyType::STAR){
                DrawSphere(pos,radius * 1.2f,(Color){color.r,color.g,color.b,40});
            }
            // Glow for black holes
            else if (type== BodyType::BLACK_HOLE){
                DrawSphere(pos,radius * 2.0f,(Color){255,100,50,20});
                // Event horizon
                float G = 1.0f;
                float r_s = 2.0f * G * mass;
                //DrawRing3D(pos,r_s - 0.3f,r_s+0.3f,(Vector3){0.0f,1.0f,0.0f},0.0f,360.0f,(Color){255,0,0,80});
            }
        }
};