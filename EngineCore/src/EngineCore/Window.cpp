#include "EngineCore/Window.h"
#include "EngineCore/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

    static bool s_GLFW_initialized = false;

	Window::Window(std::string title, const unsigned int width, const unsigned int height) 
		  : mTitle(std::move(title)),
		    mWidth(width),
		    mHeight(height) {
		int resultCode = init();
	}

	int Window::init() {
        LOG_INFO("Creating window '{0}' with size {1} x {2}", mTitle, mWidth, mHeight);

        if (!s_GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }
            s_GLFW_initialized = true;
        }
        /* Create a windowed mode window and its OpenGL context */
        mpWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
        if (!mpWindow)
        {
            LOG_CRITICAL("Can't create window '{0}' with size {1} x {2}", mTitle, mWidth, mHeight);
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