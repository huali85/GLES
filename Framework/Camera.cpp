//
// Created by lihua_000 on 3/7/2017.
//

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace Camera;

void CameraConroller::SetProjection(float fov, float aspect, float near_plane, float far_plane) {
    _camera.fov = fov;
    _camera.aspect = aspect;
    _camera.near_plane = near_plane;
    _camera.far_plane = far_plane;
    _proj = glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);
}

const glm::mat4 &CameraConroller::GetViewMatrix() {
    if (_dirty) {
        _view = glm::inverse(Transform::GetLocalToWorldMatrix(_camera.transform));
        _dirty = false;
    }
    return _view;
}

FreeCameraController::FreeCameraController(Camera &camera, float speed) :
    CameraConroller(camera),
    _speed(speed)
{ }

void FreeCameraController::Rotate(glm::vec2 input) {
    const float sensitivity = 0.2f;
    Transform_t& transform = _camera.transform;
    glm::quat q = glm::angleAxis(-input.x * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
    transform.orientation = q * transform.orientation;
    glm::vec3 right = Transform::GetRight(transform);
    q = glm::angleAxis(-input.y * sensitivity, right);
    transform.orientation = q * transform.orientation;
    _dirty = true;
}

void FreeCameraController::Move(glm::vec2 input) {
    Transform_t& transform = _camera.transform;
    glm::vec3 forward = Transform::GetForward(transform);
    glm::vec3 right = Transform::GetRight(transform);
    forward *= -input.y * _speed;
    right *= input.x * _speed;
    transform.position += forward;
    transform.position += right;
    _dirty = true;
}

TargetCameraController::TargetCameraController(Camera &camera, const glm::vec3 &target) :
        CameraConroller(camera),
        _target(target)
{
    glm::vec3 dir = _target - _camera.transform.position;
    _distance = sqrt(glm::dot(dir, dir));
    // Create quaternion from look at matrix
    glm::mat4 look_at = glm::lookAt(_camera.transform.position, _target, glm::vec3(0.0f, 1.0f, 0.0f));
    _camera.transform.orientation = glm::conjugate(glm::toQuat(look_at));
}

void TargetCameraController::Rotate(glm::vec2 input) {
    const float sensitivity = 0.2f;
    Transform_t& transform = _camera.transform;
    glm::quat q = glm::angleAxis(-input.x * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
    transform.orientation = q * transform.orientation;
    glm::vec3 right = Transform::GetRight(transform);
    q = glm::angleAxis(-input.y * sensitivity, right);
    transform.orientation = q * transform.orientation;
    glm::vec3 forward = Transform::GetForward(transform);
    transform.position = _target - forward * _distance;
    _dirty = true;
}
