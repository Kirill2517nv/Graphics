#include "RendererOpenGL.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "EngineCore/Log.h"


namespace Engine {

    bool RendererOpenGL::init(GLFWwindow* pWindow) {
        glfwMakeContextCurrent(pWindow);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return false;
        }

        LOG_INFO("OpenGL context initialized:");
        LOG_INFO("  Vendor: {0}", getVendorStr());
        LOG_INFO("  Renderer: {0}", getRendererStr());
        LOG_INFO("  Version: {0}", getVersionStr());

        return true;
    }

    void RendererOpenGL::draw(const VertexArray& vertexArray) {
        vertexArray.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray.getIndicesCount()), GL_UNSIGNED_INT, nullptr);
    }

    void RendererOpenGL::setClearColor(const float r, const float g, const float b, const float a) {
        glClearColor(r, g, b, a);
    }

    void RendererOpenGL::clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RendererOpenGL::setViewport(const unsigned int width, const unsigned int height, const unsigned int leftOffset, const unsigned int bottomOffset) {
        glViewport(leftOffset, bottomOffset, width, height);
    }

    const char* RendererOpenGL::getVendorStr() {
        return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    }

    const char* RendererOpenGL::getRendererStr(){
        return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    }

    const char* RendererOpenGL::getVersionStr() {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }

}