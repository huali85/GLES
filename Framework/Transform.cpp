//
// Created by lihua_000 on 3/19/2017.
//

#include "Transform.h"

glm::vec3 Transform::GetForward(const Transform& transform) {
    const glm::quat& orientation = transform.orientation;
    return glm::rotate(orientation, glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 Transform::GetRight(const Transform& transform) {
    const glm::quat& orientation = transform.orientation;
    return glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::mat4 Transform::GetLocalToWorldMatrix(const Transform& transform) {
    glm::mat4 r = glm::toMat4(transform.orientation);
    r[3] = glm::vec4(transform.position, 1.0f);
    return r;
}
