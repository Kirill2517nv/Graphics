#include "EngineCore/Camera.hpp"

#include <glm/trigonometric.hpp>

namespace Engine {
    Camera::Camera(const glm::vec3& position,
        const glm::vec3& rotation,
        const ProjectionMode projectionMode)
        : mPosition(position)
        , mRotation(rotation)
        , mProjectionMode(projectionMode) {
        updateViewMatrix();
        updateProjectionMatrix();
    }

    void Camera::updateViewMatrix() {
        float rotateInRadiansX = glm::radians(-mRotation.x);
        glm::mat4 rotateMatrixX(1, 0, 0, 0,
            0, cos(rotateInRadiansX), sin(rotateInRadiansX), 0,
            0, -sin(rotateInRadiansX), cos(rotateInRadiansX), 0,
            0, 0, 0, 1);

        float rotateInRadiansY = glm::radians(-mRotation.y);
        glm::mat4 rotateMatrixY(cos(rotateInRadiansY), 0, -sin(rotateInRadiansY), 0,
            0, 1, 0, 0,
            sin(rotateInRadiansY), 0, cos(rotateInRadiansY), 0,
            0, 0, 0, 1);

        float rotateInRadiansZ = glm::radians(-mRotation.z);
        glm::mat4 rotateMatrixZ(cos(rotateInRadiansZ), sin(rotateInRadiansZ), 0, 0,
            -sin(rotateInRadiansZ), cos(rotateInRadiansZ), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        glm::mat4 translateMatrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -mPosition[0], -mPosition[1], -mPosition[2], 1);

        mViewMatrix = rotateMatrixY * rotateMatrixX * rotateMatrixZ * translateMatrix;
    }

    void Camera::updateProjectionMatrix() {
        if (mProjectionMode == ProjectionMode::Perspective) {
            float r = 0.1f;
            float t = 0.1f;
            float f = 10;
            float n = 0.1f;
            mProjectionMatrix = glm::mat4(n / r, 0, 0, 0,
                0, n / t, 0, 0,
                0, 0, (-f - n) / (f - n), -1,
                0, 0, -2 * f * n / (f - n), 0);
        }
        else
        {
            float r = 2;
            float t = 2;
            float f = 100;
            float n = 0.1f;
            mProjectionMatrix = glm::mat4(1 / r, 0, 0, 0,
                0, 1 / t, 0, 0,
                0, 0, -2 / (f - n), 0,
                0, 0, (-f - n) / (f - n), 1);
        }
    }

    void Camera::setPosition(const glm::vec3& position) {
        mPosition = position;
        updateViewMatrix();
    }

    void Camera::setRotation(const glm::vec3& rotation) {
        mRotation = rotation;
        updateViewMatrix();
    }

    void Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation) {
        mPosition = position;
        mRotation = rotation;
        updateViewMatrix();
    }

    void Camera::setProjectionMode(const ProjectionMode projection_mode) {
        mProjectionMode = projection_mode;
        updateProjectionMatrix();
    }
}