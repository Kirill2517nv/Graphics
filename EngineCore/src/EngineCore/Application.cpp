#include <iostream>
#include "EngineCore/Application.h"
#include "EngineCore/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

	Application::Application() {

        LOG_INFO("TEST INFO MESSAGE");
        LOG_WARN("TEST WARN MESSAGE");
        LOG_ERROR("TEST ERROR MESSAGE");
        LOG_CRITICAL("TEST CRITICAL MESSAGE");
	}

	Application::~Application() {
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
	{
        std::cout << "window maker\n" << std::endl;

        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        // GLAD INITIALIZATION

        // linking GLAD to OpenGL using custom GLFW loader
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -1;
        }

        glClearColor(1, 0, 0, 0);


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            onUpdate();
        }

        glfwTerminate();
        return 0;


		return 0;
	}
}