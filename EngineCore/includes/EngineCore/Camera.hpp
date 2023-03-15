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

        Camera( const glm::vec3& position = { 0, 0, 0 },
                const glm::vec3& rotation = { 0, 0, 0 },
                const ProjectionMode projectionMode = ProjectionMode::Perspective);

        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& rotation);
        void setPositionRotation(const glm::vec3& position, const glm::vec3& rotation);
        void setProjectionMode(const ProjectionMode projectionMode);
        // camera frustum parameters
        void setFarClipPlane(const float far);
        void setNearClipPlane(const float near);
        void setViewportSize(const float width, const float height);
        void setFieldOfView(const float fov);
        
        const glm::mat4& getViewMatrix();
        const glm::mat4& getProjectionMatrix() const { return mProjectionMatrix; }

        const float getFarClipPlane() const { return m_far_clip_plane; }
        const float getNearClipPlane() const { return m_near_clip_plane; }
        const float getFieldOfView() const { return m_field_of_view; }

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

        float m_far_clip_plane{ 100.f };
        float m_near_clip_plane{ 0.1f };
        float m_viewport_width{ 800.f };
        float m_viewport_height{ 600.f };
        float m_field_of_view{ 60.f };

        static constexpr glm::vec3 sWorldUp{ 0.f, 0.f, 1.f };
        static constexpr glm::vec3 sWorldRight{ 0.f, -1.f, 0.f };
        static constexpr glm::vec3 sWorldForward{ 1.f, 0.f, 0.f };

        glm::mat4 mViewMatrix;
        glm::mat4 mProjectionMatrix;

        bool mUpdateViewMatrix = false;
    };
}