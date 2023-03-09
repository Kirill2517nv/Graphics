#include "VertexArray.hpp"

#include "EngineCore/Log.h"

#include <glad/glad.h>

namespace Engine {

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &mId);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &mId);
    }

    VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept {
        mId = vertexArray.mId;
        mElementsCount = vertexArray.mId;
        vertexArray.mId = 0;
        vertexArray.mElementsCount = 0;
        return *this;
    }

    VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
        : mId(vertexArray.mId)
        , mElementsCount(vertexArray.mElementsCount) {
        vertexArray.mId = 0;
        vertexArray.mElementsCount = 0;
    }

    void VertexArray::bind() const {
        glBindVertexArray(mId);
    }

    void VertexArray::unbind() {
        glBindVertexArray(0);
    }

    void VertexArray::addVertexBuffer(const VertexBuffer& vertexBuffer) {
        bind();
        vertexBuffer.bind();

        for (const BufferElement& currElement : vertexBuffer.getLayout().getElemets()) {

            glEnableVertexAttribArray(mElementsCount);
            glVertexAttribPointer(
                mElementsCount,
                static_cast<GLint>(currElement.componentCount),
                currElement.componentType,
                GL_FALSE,
                static_cast<GLsizei>(vertexBuffer.getLayout().getStride()),
                reinterpret_cast<const void*>(currElement.offset)
            );
            ++mElementsCount;
        }

        //TODO - use buffer layout

    }
    void VertexArray::setIndexBuffer(const IndexBuffer& indexBuffer) {
        bind();
        indexBuffer.bind();

        mIndicesCount = indexBuffer.getCount();
    }
}