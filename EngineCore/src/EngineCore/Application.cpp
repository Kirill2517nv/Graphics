
#include <iostream>
#include <GLFW/glfw3.h>
#include "EngineCore/Application.h"

namespace Engine {

	Application::Application() {

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

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //glClear(GL_COLOR_BUFFER_BIT);

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