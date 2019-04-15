#pragma once

#include <string>
#include "LoadTGA.h"
#include "Object.hh"

class Terrain : public Object {
    using Object::Object;

   public:
    void generate(std::string terrain_file);
    float height(float x, float z);

   private:
    float texHeight(int x, int z);

    TextureData tex {};
};