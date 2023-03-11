#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace Engine {

    class Camera {
    public:
        enum class ProjectionMode
        {
            Perspective,
            Orthographic
        };

        Camera(const glm::vec3& position = { 0, 0, 0 },
            const glm::vec3& rotation = { 0, 0, 0 },
            const ProjectionMode projectionMode = ProjectionMode::Perspective);

        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& rotation);
        void setPositionRotation(const glm::vec3& position, const glm::vec3& rotation);
        void setProjectionMode(const ProjectionMode projectionMode);
        const glm::mat4& getViewMatrix();
        const glm::mat4& getProjectionMatrix() const { return mProjectionMatrix; }

        void moveForward(const float delta);
        void moveRight(const float delta);
        void moveUp(const float delta);

        const glm::vec3& getCameraPosition() const { return mPosition; }
        const glm::vec3& getCameraRotation() const { return mRotation; }

        // movement_delta.x - forward, movement_delta.y - right, movement_delta.z - up
        // rotation_delta.x - roll, rotation_delta.y - pitch, rotation_delta.z - yaw
        void addMovementAndRotation(const glm::vec3& movementDelta,
                                      const glm::vec3& rotationDelta);

    private:
        void updateViewMatrix();
        void updateProjectionMatrix();

        glm::vec3 mPosition;
        glm::vec3 mRotation;// X - Roll, Y - Pitch, Z - Yaw
        ProjectionMode mProjectionMode;

        glm::vec3 mDirection;
        glm::vec3 mRight;
        glm::vec3 mUp;

        static constexpr glm::vec3 sWorldUp{ 0.f, 0.f, 1.f };
        static constexpr glm::vec3 sWorldRight{ 0.f, -1.f, 0.f };
        static constexpr glm::vec3 sWorldForward{ 1.f, 0.f, 0.f };

        glm::mat4 mViewMatrix;
        glm::mat4 mProjectionMatrix;

        bool mUpdateViewMatrix = false;
    };
}