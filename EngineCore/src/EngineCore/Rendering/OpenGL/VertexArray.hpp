#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.h"

namespace Engine {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertex_buffer) noexcept;
        VertexArray(VertexArray&& vertex_buffer) noexcept;

        void addVertexBuffer(const VertexBuffer& vertexBuffer);
        void setIndexBuffer(const IndexBuffer& indexBuffer);
        void bind() const;
        static void unbind();
        size_t getIndicesCount() const { return mIndicesCount; };
    private:
        unsigned int mId = 0;
        unsigned int mElementsCount = 0;
        size_t mIndicesCount = 0;
    };

}