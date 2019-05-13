#pragma once
#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "Camera.hh"
#include "Shader.hh"
#include "VectorUtils3.h"
#include "loadobj.h"

class Object {
   public:
    Object();
    virtual ~Object() = default;

    void setShader(Shader shader);
    void addTexture(GLuint texture);
    void draw(const Camera &cam, std::optional<vec4> plane = std::nullopt);
    void loadModel(const std::string &modelName);
    mat4 toWorld;
    bool useTexCoord { true };
    bool useNormals { true };

    std::vector<GLuint> textures {};

    Shader shader;

    void updateToWorld();

    void setPosition(const vec3 new_pos);
    void setScaling(const vec3 new_scaling);
    void setRotX(float rot);
    void setRotY(float rot);
    void setRotZ(float rot);

    vec3 getPosition() const { return position; }
    vec3 getScaling() const { return scaling; };
    float getRotX() const { return rotx; };
    float getRotY() const { return roty; };
    float getRotZ() const { return rotz; };

    float distance(const Camera &camera) const;
    float distance(const Object &object) const;

    /**
     *   Return normalized force vector depending on position of cam or object.
     */
    vec3 forceVector(const Camera &camera) const;
    vec3 forceVector(const Object &object) const;

    bool colliding(const Camera &camera) const;
    bool colliding(const Object &object) const;

    void setVelocity(const vec3 new_velocity) { velocity = new_velocity; }

    float collisionRadius { 1.5 };

    /**
     *  Updates the current position of the object based on its collision.
     */
    void updatePostion();

    vec3 position { 0, 0, 0 };

   protected:
    virtual void prepareDraw(const Camera &cam);

    vec3 scaling { 0, 0, 0 };

    float rotx { 0.0 };
    float roty { 0.0 };
    float rotz { 0.0 };

    vec3 velocity { 0, 0, 0 };

    const float deacceleration_constant { 0.95 };
    const float velocity_scaling { 0.2 };

    Model model;

    // DEBUG
    std::string modelname { "UNNAMED OBJECT" };
};