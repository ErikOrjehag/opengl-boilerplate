#pragma once

#include "Object.hh"
#include "LoadTGA.h"
#include <string>

class Terrain : public Object {

    void generate(std::string terrain_file);

};