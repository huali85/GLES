//
// Created by lihua_000 on 3/7/2017.
//

#ifndef LIGHT_H
#define LIGHT_H

#include "glm.hpp"

struct BaseLight {
    glm::vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
};

struct DirectionalLight : public BaseLight {
    glm::vec3 direction;
};

struct LightAttenuation {
    float constant;
    float linear;
    float exp;
};

struct PointLight : public BaseLight {
    glm::vec3 position;
    LightAttenuation attenuation;
};

struct SpotLight : public PointLight {
    glm::vec3 direction;
    float cutoff;
};

#endif //GLSAMPLE_LIGHT_H
