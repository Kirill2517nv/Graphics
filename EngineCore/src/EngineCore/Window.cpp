#include "EngineCore/Window.h"
#include "EngineCore/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

    static bool s_GLFW_initialized = false;

	Window::Window(std::string title, const unsigned int width, const unsigned int height) 
		  : mData({ std::move(title), width, height }) {
		int resultCode = init();
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
            }
        );

        glfwSetCursorPosCallback(mpWindow,
            [](GLFWwindow* pWindow, double x, double y) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            });

		return 0;
	}

    Window::~Window() {
        shutdown();
    }

    void Window::onUpdate() {
        glClearColor(0.6, 0.2, 0.6, 0);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
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