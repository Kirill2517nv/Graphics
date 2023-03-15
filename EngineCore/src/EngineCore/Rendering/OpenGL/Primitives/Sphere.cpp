#include "EngineCore/Rendering/OpenGL/Primitives/Sphere.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {
    Sphere::Sphere(float radius,unsigned int slices, unsigned int stacks)
        : interleavedStride(32),
          m_radius(radius) {
        init(slices, stacks);
    }


    void Sphere::init(unsigned sectorCount, unsigned stackCount){
        
        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / m_radius;    // vertex normal
        float s, t;

        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = m_radius * cosf(stackAngle);             // r * cos(u)
            z = m_radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                m_vertices.push_back(x);
                m_vertices.push_back(y);
                m_vertices.push_back(z);
                if (j <= sectorCount / 2) {
                    m_vertices.push_back(1.0);
                    m_vertices.push_back(0.0);
                    m_vertices.push_back(0.0);
                }
                else {
                    m_vertices.push_back(0.0);
                    m_vertices.push_back(1.0);
                    m_vertices.push_back(0.0);
                }
                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                m_normals.push_back(nx);
                m_normals.push_back(ny);
                m_normals.push_back(nz);

                // vertex tex coord (s, t) range between [0, 1]
                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                m_texCoords.push_back(s);
                m_texCoords.push_back(t);

            }
        }

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    m_indicies.push_back(k1);
                    m_indicies.push_back(k2);
                    m_indicies.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1))
                {
                    m_indicies.push_back(k1 + 1);
                    m_indicies.push_back(k2);
                    m_indicies.push_back(k2 + 1);
                }
            }
        }
    }

    void Sphere::buildInterleavedVertices()
    {
        std::vector<float>().swap(interleavedVertices);

        std::size_t i, j;
        std::size_t count = m_vertices.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            interleavedVertices.push_back(m_vertices[i]);
            interleavedVertices.push_back(m_vertices[i + 1]);
            interleavedVertices.push_back(m_vertices[i + 2]);

            interleavedVertices.push_back(m_normals[i]);
            interleavedVertices.push_back(m_normals[i + 1]);
            interleavedVertices.push_back(m_normals[i + 2]);

            interleavedVertices.push_back(m_texCoords[j]);
            interleavedVertices.push_back(m_texCoords[j + 1]);
        }
    }


}