#pragma once

#include "Object.hh"

class Skybox : public Object {
   public:
    Skybox();

   private:
    void prepareDraw(const Camera &cam) override;
};