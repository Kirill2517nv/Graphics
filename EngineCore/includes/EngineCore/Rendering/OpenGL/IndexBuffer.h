#pragma once
#include "VertexBuffer.hpp"

namespace Engine {

    class IndexBuffer {
    public:

        IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage = VertexBuffer::EUsage::Static);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer& operator=(IndexBuffer&& indexBuffer) noexcept;
        IndexBuffer(IndexBuffer&& indexBuffer) noexcept;

        void bind() const;
        static void unbind();
        size_t getCount() const { return mCount; }

    private:
        unsigned int mId = 0;
        size_t mCount;
    };

}