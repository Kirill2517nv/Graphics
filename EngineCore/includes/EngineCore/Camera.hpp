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
        glm::mat4 getViewMatrix() const { return mViewMatrix; }
        glm::mat4 getProjectionMatrix() const { return mProjectionMatrix; }

    private:
        void updateViewMatrix();
        void updateProjectionMatrix();

        glm::vec3 mPosition;
        glm::vec3 mRotation;
        ProjectionMode mProjectionMode;
        glm::mat4 mViewMatrix;
        glm::mat4 mProjectionMatrix;
    };
}