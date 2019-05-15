#pragma once

#include <string>
#include "LoadTGA.h"
#include "Object.hh"

class Water : public Object {
    using Object::Object;

   public:
    Water();

    void generate(float x, float y, float z, float width, float height);

    float waveStrength = 0.01;
    float scale = 0.6;

   private:
    void prepareDraw(const Camera &cam) override;
};