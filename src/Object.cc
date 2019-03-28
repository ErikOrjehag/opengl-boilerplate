#include "Object.hh"

Object::Object() : 
    toWorld(IdentityMatrix()) 
{

}

Object::~Object() {
    if (model) delete model;
}

void Object::setProgram(GLuint program) {
    this->program = program;
}

void Object::draw() {
    DrawModel(this->model, program, "inPosition", "inNormal", "inTexCoord");
}