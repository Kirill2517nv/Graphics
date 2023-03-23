#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.h"
#include <memory>

namespace Engine {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertex_buffer) noexcept;
        VertexArray(VertexArray&& vertex_buffer) noexcept;

        void addVertexBuffer(const std::shared_ptr<VertexBuffer>& p_vertex_buffer);
        void setIndexBuffer(const std::shared_ptr<IndexBuffer>& p_index_buffer);
        void bind() const;
        static void unbind();
        size_t getIndicesCount() const { return mIndicesCount; };
    private:
        unsigned int mId = 0;
        unsigned int mElementsCount = 0;
        size_t mIndicesCount = 0;
        std::shared_ptr<IndexBuffer> mp_index_buffer = nullptr;
    };

}