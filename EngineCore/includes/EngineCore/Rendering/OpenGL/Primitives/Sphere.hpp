#pragma once

#include "PrimitiveObject.hpp"

namespace Engine {

    class Sphere : public PrimitiveObject {
    public:
        Sphere(const glm::vec3& position, const float& radius);
        Sphere(const Sphere& other) = delete;
        Sphere(Sphere&& other) = delete;

        virtual void draw() override;
    protected:
        virtual void configureObject() override;
    
    
    private:

        float m_radius;
        float m_sectorCount = 100;
        float m_stackCount = 100;
        int interleavedStride;
    };
}