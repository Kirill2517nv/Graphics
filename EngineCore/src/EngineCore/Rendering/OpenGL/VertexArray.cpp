#include "VertexArray.hpp"

#include "EngineCore/Log.h"

#include <glad/glad.h>

namespace Engine {

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_id);
    }

    VertexArray& VertexArray::operator=(VertexArray&& vertex_array) noexcept {
        m_id = vertex_array.m_id;
        m_elements_count = vertex_array.m_id;
        vertex_array.m_id = 0;
        vertex_array.m_elements_count = 0;
        return *this;
    }

    VertexArray::VertexArray(VertexArray&& vertex_array) noexcept
        : m_id(vertex_array.m_id)
        , m_elements_count(vertex_array.m_elements_count) {
        vertex_array.m_id = 0;
        vertex_array.m_elements_count = 0;
    }

    void VertexArray::bind() const {
        glBindVertexArray(m_id);
    }

    void VertexArray::unbind() {
        glBindVertexArray(0);
    }

    void VertexArray::add_vertex_buffer(const VertexBuffer& vertex_buffer) {
        bind();
        vertex_buffer.bind();

        for (const BufferElement& currElement : vertex_buffer.getLayout().getElemets()) {

            glEnableVertexAttribArray(m_elements_count);
            glVertexAttribPointer(
                m_elements_count,
                static_cast<GLint>(currElement.componentCount),
                currElement.componentType,
                GL_FALSE,
                static_cast<GLsizei>(vertex_buffer.getLayout().getStride()),
                reinterpret_cast<const void*>(currElement.offset)
            );
            ++m_elements_count;
        }

        //TODO - use buffer layout

    }
    void VertexArray::set_index_buffer(const IndexBuffer& index_buffer) {
        bind();
        index_buffer.bind();

        mIndicesCount = index_buffer.get_count();
    }
}