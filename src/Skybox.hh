#pragma once

#include "Object.hh"

class Skybox : public Object {
   public:
    Skybox();

   private:
    void _draw(const Camera &cam) override;
};