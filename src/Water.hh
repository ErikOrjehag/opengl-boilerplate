#pragma once

#include <string>
#include "LoadTGA.h"
#include "Object.hh"

class Water : public Object {
    using Object::Object;

   public:
    Water();

    void generate(float x, float y, float z, float width, float height);

   private:
    void _draw(const Camera &cam) override;
};