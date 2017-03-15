//
// Created by lihua_000 on 3/3/2017.
//

#ifndef VERTEX_H
#define VERTEX_H

#include "glm/glm.hpp"

struct VertexBase {
    glm::vec3 pos;
    glm::vec3 normal;
};

struct VertexFull {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 texcoord;
};

#endif //VERTEX_H
