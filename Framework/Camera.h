//
// Created by lihua_000 on 3/7/2017.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

struct Transform {
    glm::quat orientation;
    glm::vec3 position;
    float scale;
};

class BaseCamera
{
public:
    BaseCamera(const float min_pitch, const float max_pitch) : m_min_pitch(min_pitch), m_max_pitch(max_pitch) {}
    virtual ~BaseCamera() {}

    void Rotate(float diff_yaw, float diff_pitch);
    virtual void Zoom(float diff) = 0;

    virtual void SetupLookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);
    void SetupProjection(const float fovy, const float aspect_ratio, const float near, const float far);

    float GetFOV() const { return m_fov; }
    const glm::mat4& GetViewMatrix();
    const glm::mat4& GetProjMatrix() const { return m_proj; }
    const glm::vec3& GetPosition() const { return m_position; }

protected:
    virtual void UpdateRotation();
    void UpdateProjection() { m_proj = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far); }

    float m_yaw;
    float m_pitch;
    float m_fov;
    float m_aspect_ratio;
    float m_near;
    float m_far;

    float m_min_pitch;
    float m_max_pitch;

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;

    glm::mat4 m_view;
    glm::mat4 m_proj;

    bool m_should_update_view;
};

class FreeCamera : public BaseCamera
{
public:
    FreeCamera(const float min_pitch = -89.0f, const float max_pitch = 89.0f) : BaseCamera(min_pitch, max_pitch) {}
    ~FreeCamera() override {}

    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    void Move(Movement direction, float dt);
    void Zoom(float diff) override;

    void SetSpeed(const float speed) { m_speed = speed; }

protected:

    float m_speed;
};

class TargetCamera : public BaseCamera
{
public:
    TargetCamera(const float min_pitch = -60.0f, const float max_pitch = 60.0f, const float min_distance = 1.0f, const float max_distance= 10.0f) :
            BaseCamera(min_pitch, max_pitch), m_min_distance(min_distance), m_max_distance(max_distance) {}
    ~TargetCamera() override {}

    void Zoom(float diff) override;

    void SetupLookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up) override;

    void SetDistance(const float distance);

protected:
    void UpdateRotation() override;

    glm::vec3 m_target;
    float m_distance;
    float m_min_distance;
    float m_max_distance;
};

#endif //CAMERA_H
