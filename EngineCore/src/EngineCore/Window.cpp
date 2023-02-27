#include "EngineCore/Window.h"
#include "EngineCore/Log.h"
#include "EngineCore/Rendering/OpenGL/ShaderProgram.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace Engine {

    static bool s_GLFW_initialized = false;

    GLfloat points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    const char* vertexShader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertexPosition;"
        "layout(location = 1) in vec3 vertexColor;"
        "out vec3 color;"
        "void main() {"
        "    color = vertexColor;"
        "    gl_Position = vec4(vertexPosition, 1.0);"
        "}";

    const char* fragmentShader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 fragColor;"
        "void main() {"
        "    fragColor = vec4(color, 1.0);"
        "}";

    std::unique_ptr<ShaderProgram> pShaderProgram;
    GLuint vao;

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


        /*GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShader, nullptr);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShader, nullptr);
        glCompileShader(fs);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);

        glDeleteShader(vs);
        glDeleteShader(fs);*/

        GLuint pointsVbo = 0;
        glGenBuffers(1, &pointsVbo);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

        GLuint colorsVbo = 0;
        glGenBuffers(1, &colorsVbo);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), &colors, GL_STATIC_DRAW);
        
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		return 0;
	}

    Window::~Window() {
        shutdown();
    }

    void Window::onUpdate() {
        glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], mBackgroundColor[3]);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        pShaderProgram->bind();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);




        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        ImGui::ShowDemoWindow();


        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background color", mBackgroundColor);
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