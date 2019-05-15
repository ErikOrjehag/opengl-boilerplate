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

    void setVelocity(const vec3 new_velocity) {
        velocity = new_velocity;
        speed = std::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
    }

    float getSpeed() const { return speed; }

    float collisionRadius { 1.7 };

    /**
     *  Places the object at the edge of the camera, in regards to the collision
     *  radius.
     */
    void placeAtCamEdge(const Camera &camera);

    /**
     *  Updates the current position of the object based on its collision.
     */
    void updatePostion();

    vec3 position { 0, 0, 0 };

    static constexpr const float deacceleration_constant { 0.95 };
    static constexpr const float velocity_scaling { 0.2 };
    static constexpr const float elasticity_constant { 1.5 };

   protected:
    virtual void prepareDraw(const Camera &cam);

    vec3 scaling { 1, 1, 1 };

    float rotx { 0.0 };
    float roty { 0.0 };
    float rotz { 0.0 };

    float speed { 0.0 };

    vec3 velocity { 0, 0, 0 };

    Model model;

    // DEBUG
    std::string modelname { "UNNAMED OBJECT" };
};