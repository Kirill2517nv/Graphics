#include "EngineCore/Camera.hpp"

#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

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
        const float rollInRadians = glm::radians(mRotation.x);
        const float pitchInRadians = glm::radians(mRotation.y);
        const float yawInRadians = glm::radians(mRotation.z);

        const glm::mat3 rotateMatrixX(1, 0, 0,
            0, cos(rollInRadians), sin(rollInRadians),
            0, -sin(rollInRadians), cos(rollInRadians));

        const glm::mat3 rotateMatrixY(cos(pitchInRadians), 0, -sin(pitchInRadians),
            0, 1, 0,
            sin(pitchInRadians), 0, cos(pitchInRadians));

        const glm::mat3 rotateMatrixZ(cos(yawInRadians), sin(yawInRadians), 0,
            -sin(yawInRadians), cos(yawInRadians), 0,
            0, 0, 1);

        const glm::mat3 eulerRotateMatrix = rotateMatrixZ * rotateMatrixY * rotateMatrixX;
        mDirection = glm::normalize(eulerRotateMatrix * sWorldForward);
        mRight = glm::normalize(eulerRotateMatrix * sWorldRight);
        mUp = glm::cross(mRight, mDirection);

        mViewMatrix = glm::lookAt(mPosition, mPosition + mDirection, mUp);
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
        mUpdateViewMatrix = true;
    }

    void Camera::setRotation(const glm::vec3& rotation) {
        mRotation = rotation;
        mUpdateViewMatrix = true;
    }

    void Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation) {
        mPosition = position;
        mRotation = rotation;
        mUpdateViewMatrix = true;
    }

    void Camera::setProjectionMode(const ProjectionMode projection_mode) {
        mProjectionMode = projection_mode;
        updateProjectionMatrix();
    }

    const glm::mat4& Camera::getViewMatrix()
    {
        if (mUpdateViewMatrix)
        {
            updateViewMatrix();
        }
        return mViewMatrix;
    }

    void Camera::moveForward(const float delta)
    {
        mPosition += mDirection * delta;
        mUpdateViewMatrix = true;
    }

    void Camera::moveRight(const float delta)
    {
        mPosition += mRight * delta;
        mUpdateViewMatrix = true;
    }

    void Camera::moveUp(const float delta)
    {
        mPosition += sWorldUp * delta;
        mUpdateViewMatrix = true;
    }

    void Camera::addMovementAndRotation(const glm::vec3& movementDelta,
        const glm::vec3& rotationDelta)
    {
        mPosition += mDirection * movementDelta.x;
        mPosition += mRight * movementDelta.y;
        mPosition += mUp * movementDelta.z;
        mRotation += rotationDelta;
        mUpdateViewMatrix = true;
    }
}