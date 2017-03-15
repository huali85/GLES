//
// Created by lihua_000 on 3/8/2017.
//

#ifndef SCENE_H
#define SCENE_H

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>

namespace Scene
{
    struct Transform {
        glm::quat orientation;
        glm::vec3 position;
        float scale;

        glm::vec3 GetForward() {
            float x = 2.0f * (orientation.x * orientation.z + orientation.y * orientation.w);
            float y = 2.0f * (orientation.y * orientation.z - orientation.x * orientation.w);
            float z = 1.0f - 2.0f * (orientation.x * orientation.x + orientation.y * orientation.y);
            return glm::normalize(glm::vec3(x, y, z));
        }

        glm::vec3 GetRight() {
            float x = 1.0f - 2.0f * (orientation.y * orientation.y + orientation.z * orientation.z);
            float y = 2.0f * (orientation.x * orientation.y + orientation.w * orientation.z);
            float z = 2.0f * (orientation.x * orientation.z - orientation.y * orientation.w);
            return glm::normalize(glm::vec3(x, y, z));
        }
    };

    struct Camera {
        Transform transform;
        glm::mat4 projection;

        glm::mat4 GetViewMatrix() {
            glm::mat4 r = glm::toMat4(transform.orientation);
            r[3] = glm::vec4(transform.position, 1.0f);
            return glm::inverse(r);
        }
    };

}

#endif //SCENE_H
