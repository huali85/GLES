//
// Created by lihua_000 on 3/19/2017.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Transform
{
    struct Transform {
        glm::quat orientation;
        glm::vec3 position;
        float    scale;
    };

    glm::vec3 GetForward(const Transform& transform);
    glm::vec3 GetRight(const Transform& transform);
    glm::mat4 GetLocalToWorldMatrix(const Transform& transform);
}

typedef Transform::Transform Transform_t;

#endif //TRANSFORM_H
