#include "Object.hh"

Object::Object() : toWorld { IdentityMatrix() } {}

void Object::setShader(Shader shader) { this->shader = shader; }

void Object::addTexture(GLuint texture) { textures.push_back(texture); }

void Object::draw(const Camera &cam, std::optional<vec4> plane) {
    shader.activate();

    for (size_t i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

    if (plane) {
        shader.upload("plane", plane.value());
    }

    shader.upload("projection", cam.projectionMatrix);
    shader.upload("modelToWorld", toWorld);
    shader.upload("worldToView", cam.camMatrix);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    prepareDraw(cam);

    int ok = DrawModel(&model, shader.program, "inPosition",
                       shader.hasNormals ? "inNormal" : NULL,
                       shader.hasTextureCoords ? "inTexCoord" : NULL);

    if (ok == 0) {
        std::cerr << "Error in model " << this->modelname << std::endl;
    }
}

void Object::prepareDraw(const Camera &cam) {}

void Object::loadModel(const std::string &modelName) {
    model = *LoadModelPlus(modelName.c_str());
    this->modelname = modelName;
}

float Object::distance(const Camera &camera) const {
    float x_diff = position.x - camera.camPos.x;
    float y_diff = position.y - camera.camPos.y;
    float z_diff = position.z - camera.camPos.z;

    return std::sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
}

float Object::distance(const Object &object) const {
    float x_diff = position.x - object.getPosition().x;
    float y_diff = position.y - object.getPosition().y;
    float z_diff = position.z - object.getPosition().z;

    return std::sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
}

vec3 Object::forceVector(const Camera &camera) const {
    vec3 forces = position - camera.camPos;
    return Normalize(forces);
}

vec3 Object::forceVector(const Object &object) const {
    vec3 forces = position - object.getPosition();
    return Normalize(forces);
}

bool Object::colliding(const Camera &camera) const {
    float centerDistance = distance(camera);
    return (centerDistance - (camera.collisionRadius + collisionRadius)) <= 0.0;
}

bool Object::colliding(const Object &object) const {
    float centerDistance = distance(object);
    return (centerDistance - (object.collisionRadius + collisionRadius)) <= 0.0;
}

// Very unclear if this is the correct solution to this problem :D
void Object::placeAtCamEdge(const Camera &camera) {
    float dx = position.x - camera.camPos.x;
    float dz = position.z - camera.camPos.z;

    float dist_diff =
        std::abs(distance(camera) - (collisionRadius + camera.collisionRadius));

    position.x += dx * dist_diff;
    position.z += dz * dist_diff;
    updateToWorld();
}

void Object::updatePostion() {
    vec3 new_pos = position;
    new_pos.x += velocity.x * velocity_scaling;
    new_pos.y += velocity.y * velocity_scaling;
    new_pos.z += velocity.z * velocity_scaling;

    velocity.x *= deacceleration_constant;
    velocity.y *= deacceleration_constant;
    velocity.z *= deacceleration_constant;

    // Also does speed calulations
    setPosition(new_pos);
    updateToWorld();
}

void Object::updateToWorld() {
    toWorld = T(position.x, position.y, position.z) * Rx(rotx) * Ry(roty) *
              Rz(rotz) * S(scaling.x, scaling.y, scaling.z);
}

void Object::setPosition(const vec3 new_pos) {
    position = new_pos;
    updateToWorld();
}
void Object::setScaling(const vec3 new_scaling) {
    scaling = new_scaling;
    updateToWorld();
}
void Object::setRotX(float rot) {
    rotx = rot;
    updateToWorld();
}
void Object::setRotY(float rot) {
    roty = rot;
    updateToWorld();
}
void Object::setRotZ(float rot) {
    rotz = rot;
    updateToWorld();
}