#pragma once
#include <vector>

namespace Engine {

    class Sphere {
    public:
        Sphere(float radius, unsigned int slices, unsigned int stacks);
        std::vector<float> getVerticies() { return m_vertices; }
        std::vector<uint32_t> getIndicies() { return m_indicies; }
    private:
        void init(unsigned slices, unsigned stacks);
        void buildInterleavedVertices();



        float m_radius;
        std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_texCoords;
        std::vector<uint32_t> m_indicies;
        std::vector<float> interleavedVertices;
        int interleavedStride;
    };
}