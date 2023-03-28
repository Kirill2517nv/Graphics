#include "EngineCore/Rendering/OpenGL/Primitives/Sphere.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

    Sphere::Sphere(const glm::vec3& position, const float& radius) :
        PrimitiveObject(position),
        m_radius(radius) {
        configureObject();

    }


    void Sphere::configureObject() {

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / m_radius;    // vertex normal
        float s, t;

        std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_texCoords;
        std::vector<uint32_t> m_indicies;
        std::vector<float> interleavedVertices;

        float sectorStep = 2 * M_PI / m_sectorCount;
        float stackStep = M_PI / m_stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= m_stackCount; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = m_radius * cosf(stackAngle);             // r * cos(u)
            z = m_radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= m_sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                m_vertices.push_back(x);
                m_vertices.push_back(y);
                m_vertices.push_back(z);
                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                m_normals.push_back(nx);
                m_normals.push_back(ny);
                m_normals.push_back(nz);

                // vertex tex coord (s, t) range between [0, 1]
                s = 10.f*(float)j / m_sectorCount;
                t = 10.f*(float)i / m_stackCount;
                m_texCoords.push_back(s);
                m_texCoords.push_back(t);

            }
        }

        int k1, k2;
        for (int i = 0; i < m_stackCount; ++i)
        {
            k1 = i * (m_sectorCount + 1);     // beginning of current stack
            k2 = k1 + m_sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < m_sectorCount; ++j, ++k1, ++k2)
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
                if (i != (m_stackCount - 1))
                {
                    m_indicies.push_back(k1 + 1);
                    m_indicies.push_back(k2);
                    m_indicies.push_back(k2 + 1);
                }
            }
        }

        std::vector<float>().swap(interleavedVertices);

        std::size_t i, j, k;
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


        BufferLayout buffer_layout_vec3_vec3_vec2
        {
            ShaderDataType::Float3, // vertex pos
            ShaderDataType::Float3, // normals
            ShaderDataType::Float2  // UV
        };

        m_vao = std::make_shared<VertexArray>();
        m_vbo = std::make_shared<VertexBuffer>(interleavedVertices.data(), (unsigned int)interleavedVertices.size() * sizeof(float), buffer_layout_vec3_vec3_vec2);
        m_ib = std::make_shared<IndexBuffer>(m_indicies.data(), m_indicies.size());

        m_vao->addVertexBuffer(m_vbo);
        m_vao->setIndexBuffer(m_ib);
    }


    void Sphere::draw() {
        if (m_vao != nullptr) {
            // RendererOpenGL functions here
            if (m_sp != nullptr) {
                m_sp->bind();
                m_sp->setMatrix4("model_matrix", m_model_matrix);
                m_sp->setVec3("camera_position", m_camera_position);
                m_sp->setVec3("light_position", m_light_source_position);
                m_sp->setVec3("light_color", m_light_source_color);
                // set material to shader
                m_sp->setFloat("ambient_factor", m_material->m_ambient_factor);
                m_sp->setFloat("diffuse_factor", m_material->m_diffuse_factor);
                m_sp->setFloat("specular_factor", m_material->m_specular_factor);
                m_sp->setFloat("shininess", m_material->m_shininess);
            }
            else {
                LOG_ERROR("No shader");
            }
            RendererOpenGL::draw(m_vao);
        }
        else {
            LOG_ERROR("No vao");
        }
    }
}