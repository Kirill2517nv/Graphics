#include "EngineCore/Window.h"
#include "EngineCore/Log.h"
#include "EngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "EngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "EngineCore/Rendering/OpenGL/IndexBuffer.h"
#include "EngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "EngineCore/Modules/UIModule.hpp"
#include "EngineCore/Camera.hpp"

#include "EngineCore/Rendering/OpenGL/RendererOpenGL.hpp"
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

namespace Engine {


    GLfloat positionsColors[] = {
       -0.5f,  -0.5f, 0.0f,    1.0f, 1.0f, 0.0f,
        0.5f,  -0.5f, 0.0f,    0.0f, 1.0f, 1.0f,
       -0.5f,   0.5f, 0.0f,    1.0f, 0.0f, 1.0f,

        0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 1.0f
    };

    GLfloat positionsColors2[] = {
       -0.5f,  -0.5f, 0.0f,    1.0f, 1.0f, 0.0f,
        0.5f,  -0.5f, 0.0f,    0.0f, 1.0f, 1.0f,
       -0.5f,   0.5f, 0.0f,    1.0f, 0.0f, 1.0f,
        0.5f,   0.5f, 0.0f,    1.0f, 0.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
    };

    const char* vertexShader =
        R"(
            #version 460
            layout(location = 0) in vec3 vertexPosition;
            layout(location = 1) in vec3 vertexColor;
            uniform mat4 modelMat;
            uniform mat4 viewProjectionMatrix;
            out vec3 color;
            void main() {
                color = vertexColor;
                gl_Position = viewProjectionMatrix * modelMat * vec4(vertexPosition, 1.0);
            }
        )";

    const char* fragmentShader =
        R"(
            #version 460
            in vec3 color;
            out vec4 fragColor;
            void main() {
                fragColor = vec4(color, 1.0);
            }
        )";

    std::unique_ptr<ShaderProgram> pShaderProgram;
    std::unique_ptr<VertexBuffer> pPositionsColorsVbo;
    std::unique_ptr<IndexBuffer> pIndexBuffer;
    std::unique_ptr<VertexArray> pVao;

    float scale[3] = { 1.0f, 1.0f, 1.0f };
    float rotate = 0;
    float translate[3] = { 0.f, 0.f, 0.f };

    float cameraPosition[3] = { 0.f, 0.f, 1.f };
    float cameraRotation[3] = { 0.f, 0.f, 0.f };
    bool perspectiveCamera = false;
    Camera camera;


	Window::Window(std::string title, const unsigned int width, const unsigned int height) 
		  : mData({ std::move(title), width, height }) {
		int resultCode = init();

	}

    int Window::init() {
        LOG_INFO("Creating window '{0}' with size {1} x {2}", mData.title, mData.width, mData.height);

        glfwSetErrorCallback([](int errorCode, const char* description) {
                LOG_CRITICAL("GLFW error: {0}", description);
            });

        if (!glfwInit())
        {
            LOG_CRITICAL("Can't initialize GLFW!");
            return -1;
        }


        /* Create a windowed mode window and its OpenGL context */
        mpWindow = glfwCreateWindow(mData.width, mData.height, mData.title.c_str(), nullptr, nullptr);
        if (!mpWindow)
        {
            LOG_CRITICAL("Can't create window '{0}' with size {1} x {2}", mData.title, mData.width, mData.height);
            return -2;
        }

        if (!RendererOpenGL::init(mpWindow))
        {
            LOG_CRITICAL("Failed to initialize OpenGL renderer");
            return -3;
        }

        glfwSetWindowUserPointer(mpWindow, &mData);

        glfwSetWindowSizeCallback(mpWindow,
            [](GLFWwindow* pWindow, int width, int height) {

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;
                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            });

        glfwSetCursorPosCallback(mpWindow,
            [](GLFWwindow* pWindow, double x, double y) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            });

        glfwSetWindowCloseCallback(mpWindow,
            [](GLFWwindow* pWindow) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventWindowClose event;
                data.eventCallbackFn(event);
            });

        glfwSetFramebufferSizeCallback(mpWindow,
            [](GLFWwindow* pWindow, int width, int height) {
                RendererOpenGL::setViewport(width, height);
            });
        
        UIModule::onWindowCreate(mpWindow);

        pShaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
        if (!pShaderProgram->isCompiled()) {
            return false;
        }
        
        BufferLayout bufferLayout2vec3{
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };
        
        pVao = std::make_unique<VertexArray>();
        pPositionsColorsVbo = std::make_unique<VertexBuffer>(positionsColors, sizeof(positionsColors), bufferLayout2vec3);
        pIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        pVao->add_vertex_buffer(*pPositionsColorsVbo); // loading pos and colors to VAO
        pVao->set_index_buffer(*pIndexBuffer);

		return 0;
	}

    Window::~Window() {
        shutdown();
    }

    void Window::onUpdate() {
        RendererOpenGL::setClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], mBackgroundColor[3]);
        RendererOpenGL::clear();

        pShaderProgram->bind();

        glm::mat4 scaleMat   (scale[0],     0,              0,          0,
                              0,            scale[1],       0,          0,
                              0,            0,              scale[2],   0,
                              0,            0,              0,          1);

        float angle = glm::radians(rotate);

        glm::mat4 rotateMat(cos(angle), sin(angle), 0, 0,
                           -sin(angle), cos(angle), 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1);

        glm::mat4 translateMat(1,         0,            0,               0,
                               0,         1,            0,               0,
                               0,         0,            1,               0,
                               translate[0], translate[1], translate[2], 1);

        glm::mat4 modelMat = translateMat * rotateMat * scaleMat;
        pShaderProgram->setMatrix4("modelMat", modelMat);

        camera.setPositionRotation(glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]),
            glm::vec3(cameraRotation[0], cameraRotation[1], cameraRotation[2]));
        camera.setProjectionMode(perspectiveCamera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
        pShaderProgram->setMatrix4("viewProjectionMatrix", camera.getProjectionMatrix() * camera.getViewMatrix());


        RendererOpenGL::draw(*pVao);


        UIModule::onUiDrawBegin();
        bool show = true;
        UIModule::ShowExampleAppDockSpace(&show);
        ImGui::ShowDemoWindow();
        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background Color", mBackgroundColor);
        ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("translate", translate, -1.f, 1.f);
        ImGui::SliderFloat3("camera position", cameraPosition, -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", cameraRotation, 0, 360.f);
        ImGui::Checkbox("Perspective camera", &perspectiveCamera);
        ImGui::End();

        UIModule::onUiDrawEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(mpWindow);
        /* Poll for and process events */
        glfwPollEvents();


    }

	void Window::shutdown() {
        UIModule::onWindowClose();
        glfwDestroyWindow(mpWindow);
        glfwTerminate();

	}
}