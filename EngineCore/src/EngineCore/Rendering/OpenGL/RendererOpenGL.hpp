#pragma once

struct GLFWwindow;

namespace Engine {
    class VertexArray;

    class RendererOpenGL {
    public:
        static bool init(GLFWwindow* pWindow);

        static void draw(const VertexArray& vertexArray);
        static void setClearColor(const float r, const float g, const float b, const float a);
        static void clear();
        static void setViewport(const unsigned int width, const unsigned int height, const unsigned int leftOffset = 0, const unsigned int bottomOffset = 0);

        static const char* getVendorStr();
        static const char* getRendererStr();
        static const char* getVersionStr();
    };

}