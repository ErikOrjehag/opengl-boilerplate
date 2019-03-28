#pragma once

#include "Object.hh"
#include "LoadTGA.h"
#include <string>

class Terrain : public Object {

public:
    void generate(std::string terrain_file);
    float height(float x, float z);

private:
    float texHeight(int x, int z);

    TextureData tex{};

};