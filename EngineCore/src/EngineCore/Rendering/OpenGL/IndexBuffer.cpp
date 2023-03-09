#include "IndexBuffer.h"

#include "EngineCore/Log.h"

#include <glad/glad.h>

namespace Engine {

    constexpr GLenum usageToGLenum(const VertexBuffer::EUsage usage)
    {
        switch (usage)
        {
        case VertexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
        case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
        case VertexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
        }

        LOG_ERROR("Unknown VertexBuffer usage");
        return GL_STREAM_DRAW;
    }

    IndexBuffer::IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage)
        : mCount(count)
    {
        glGenBuffers(1, &mId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, usageToGLenum(usage));
    }


    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &mId);
    }


    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& indexBuffer) noexcept
    {
        mId = indexBuffer.mId;
        mCount = indexBuffer.mCount;
        indexBuffer.mId = 0;
        indexBuffer.mCount = 0;
        return *this;
    }


    IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept
        : mId(indexBuffer.mId)
        , mCount(indexBuffer.mCount)
    {
        indexBuffer.mId = 0;
        indexBuffer.mCount = 0;
    }


    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
    }


    void IndexBuffer::unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}