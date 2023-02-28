#include "EngineCore/Window.h"
#include "EngineCore/Log.h"
#include "EngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "EngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "EngineCore/Rendering/OpenGL/IndexBuffer.h"
#include "EngineCore/Rendering/OpenGL/VertexArray.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

namespace Engine {

    static bool s_GLFW_initialized = false;

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
            uniform mat4 scaleMat;
            uniform mat4 rotateMat;
            out vec3 color;
            void main() {
                color = vertexColor;
                gl_Position = rotateMat * scaleMat * vec4(vertexPosition, 1.0);
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
	Window::Window(std::string title, const unsigned int width, const unsigned int height) 
		  : mData({ std::move(title), width, height }) {
		int resultCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(mpWindow, true);
	}

    int Window::init() {
        LOG_INFO("Creating window '{0}' with size {1} x {2}", mData.title, mData.width, mData.height);

        if (!s_GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }
            s_GLFW_initialized = true;
        }
        /* Create a windowed mode window and its OpenGL context */
        mpWindow = glfwCreateWindow(mData.width, mData.height, mData.title.c_str(), nullptr, nullptr);
        if (!mpWindow)
        {
            LOG_CRITICAL("Can't create window '{0}' with size {1} x {2}", mData.title, mData.width, mData.height);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(mpWindow);

        // GLAD INITIALIZATION

        // linking GLAD to OpenGL using custom GLFW loader
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to initialize GLAD");
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
                glViewport(0, 0, width, height);
            });



        pShaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
        if (!pShaderProgram->isCompiled()) {
            return false;
        }
        
        BufferLayout bufferLayout2vec3{
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };
        
        pVao= std::make_unique<VertexArray>();
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
        glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], mBackgroundColor[3]);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();


        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background color", mBackgroundColor);
        ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
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

        pShaderProgram->setMatrix4("scaleMat", scaleMat);

        pVao->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(pVao->getIndicesCount()), GL_UNSIGNED_INT, nullptr);
        ImGui::End();


        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(mpWindow);
        /* Poll for and process events */
        glfwPollEvents();


    }

	void Window::shutdown() {
        glfwDestroyWindow(mpWindow);
        glfwTerminate();

	}
}