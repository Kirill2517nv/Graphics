#pragma once

#include <vector>

namespace Engine {

    enum class ShaderDataType {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4
    };

    struct BufferElement {
        ShaderDataType type;
        uint32_t componentType;
        size_t componentCount;
        size_t size;
        size_t offset;

        BufferElement(const ShaderDataType type);
    };

    class BufferLayout {
    public:
        BufferLayout(std::initializer_list<BufferElement> elements)
            : mElements(std::move(elements))
        {
            size_t offset = 0;
            mStride = 0;
            for (auto& element : mElements) {
                element.offset = offset;
                offset += element.size;
                mStride += element.size;
            }
        }

        const std::vector<BufferElement>& getElemets() const { return mElements; };
        size_t getStride() const { return mStride; };
    private:
        std::vector<BufferElement> mElements;
        size_t mStride = 0;
    };

    class VertexBuffer {
    public:

        enum class EUsage {
            Static,
            Dynamic,
            Stream
        };

        VertexBuffer(const void* data, const size_t size, BufferLayout bufferLayout, const EUsage usage = VertexBuffer::EUsage::Static);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
        VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

        void bind() const;
        static void unbind();

        const BufferLayout& getLayout() const { return mBufferLayout; };

    private:
        unsigned int m_id = 0;
        BufferLayout mBufferLayout;
    };

}