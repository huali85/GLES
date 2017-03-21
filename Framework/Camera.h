//
// Created by lihua_000 on 3/7/2017.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"

namespace Camera
{
    struct Camera {
        Transform_t transform;
        float fov;
        float near_plane;
        float far_plane;
        float aspect;
    };

    class CameraConroller {
    public:
        CameraConroller(Camera& camera) : _camera(camera), _dirty(true) {}
        virtual ~CameraConroller() {}

        virtual void Rotate(glm::vec2 input) = 0;
        virtual void Move(glm::vec2 input) {}

        void SetProjection(float fov, float aspect, float near_plane, float far_plane);

        const glm::mat4& GetViewMatrix();
        const glm::mat4& GetProjectionMatrix() const { return _proj; }

    protected:
        Camera& _camera;
        bool _dirty;
        glm::mat4 _view;
        glm::mat4 _proj;
    };

    class FreeCameraController : public CameraConroller {
    public:
        FreeCameraController(Camera& camera, float speed);

        void Rotate(glm::vec2 input) override;
        void Move(glm::vec2 input) override;

    private:
        float _speed;
    };

    class TargetCameraController : public CameraConroller {
    public:
        TargetCameraController(Camera& camera, const glm::vec3& target);

        void Rotate(glm::vec2 input) override;

    private:
        glm::vec3 _target;
        float _distance;
    };
}

typedef Camera::Camera Camera_t;

#endif //CAMERA_H
