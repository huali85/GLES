//
// Created by lihua_000 on 3/7/2017.
//

#include "Camera.h"
#include "gtx/euler_angles.hpp"
#include <algorithm>

void BaseCamera::Rotate(float diff_yaw, float diff_pitch)
{
    m_yaw += diff_yaw;
    m_pitch += diff_pitch;

    if (m_pitch > m_max_pitch)
        m_pitch = m_max_pitch;
    if (m_pitch < m_min_pitch)
        m_pitch = m_min_pitch;

    UpdateRotation();
}

const glm::mat4& BaseCamera::GetViewMatrix()
{
    if (m_should_update_view) {
        m_view = glm::lookAt(m_position, m_position + m_front, m_up);
        m_should_update_view = false;
    }
    return m_view;
}

void BaseCamera::UpdateRotation()
{
    //printf("yaw: %f pitch: %f\n", m_yaw, m_pitch);

    float rad_yaw = glm::radians(m_yaw);
    float rad_pitch = glm::radians(m_pitch);
    float cos_pitch = cos(rad_pitch);
    glm::vec3 front;
    front.x = cos(rad_yaw) * cos_pitch;
    front.y = sin(rad_pitch);
    front.z = sin(rad_yaw) * cos_pitch;
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up = glm::normalize(glm::cross(m_right, m_front));

    m_should_update_view = true;
}

void BaseCamera::SetupLookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
{
    m_position = pos;
    m_front = glm::normalize(target - pos);
    m_up = up;

    m_yaw = glm::degrees(float(atan2(m_front.z, m_front.x)));
    m_pitch = glm::degrees(asin(m_front.y));

    m_should_update_view = true;
}

void BaseCamera::SetupProjection(const float fovy, const float aspect_ratio, const float near, const float far)
{
    m_fov = fovy;
    m_aspect_ratio = aspect_ratio;
    m_near = near;
    m_far = far;
    UpdateProjection();
}

void FreeCamera::Move(Movement direction, float dt)
{
    float velocity = m_speed * dt;
    switch (direction)
    {
        case FORWARD:
            m_position += m_front * velocity;
            break;

        case BACKWARD:
            m_position -= m_front * velocity;
            break;

        case LEFT:
            m_position -= m_right * velocity;
            break;

        case RIGHT:
            m_position += m_right * velocity;
            break;
    }

    m_should_update_view = true;
}

void FreeCamera::Zoom(float diff)
{
    float old_fov = m_fov;
    m_fov -= diff;
    m_fov = glm::clamp(m_fov, 1.0f, 90.0f);
    if (old_fov != m_fov)
        UpdateProjection();
}

void TargetCamera::Zoom(float diff)
{
    SetDistance(m_distance - diff);
}

void TargetCamera::SetupLookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
{
    BaseCamera::SetupLookAt(pos, target, up);
    m_target = target;
    SetDistance(glm::distance(m_position, m_target));
}

void TargetCamera::SetDistance(const float distance)
{
    m_distance = std::max(m_min_distance, std::min(distance, m_max_distance));
    m_position = m_target - m_front * m_distance;

    m_should_update_view = true;
}

void TargetCamera::UpdateRotation()
{
    BaseCamera::UpdateRotation();
    m_position = m_target - m_front * m_distance;
}