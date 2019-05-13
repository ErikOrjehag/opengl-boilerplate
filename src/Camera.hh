#pragma once

#include <algorithm>
#include <iostream>

#include "VectorUtils3.h"

class Camera {
   public:
    Camera();

    mat4 projectionMatrix;
    mat4 camMatrix;

    // Changes camera speed
    void increase();
    void decrease();

    void up();
    void down();

    void updatePos(float mx, float mz);
    void updateCamMatrix();

    void rotateX(float dx);
    void rotateY(float dy);

    void invertPitch();

    float& x() { return camPos.x; };
    float& y() { return camPos.y; };
    float& z() { return camPos.z; };

    vec3 camPos = { 1, 8, 1 };

   private:
    // Movement
    float yrot = 0;
    float xrot = M_PI * 3.f / 4.f;
    float camSpeed = 0.7;

    const float SPEED_DELTA = 0.01;
};