#include "EngineCore/Window.h"
#include "EngineCore/Log.h"
#include "EngineCore/Modules/UIModule.hpp"


#include "EngineCore/Rendering/OpenGL/RendererOpenGL.hpp"
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>


namespace Engine {


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

        glfwSetKeyCallback(mpWindow,
            [](GLFWwindow* mpWindow, int key, int scancode, int action, int mods) {
            
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(mpWindow));
                switch (action) {
                case GLFW_PRESS: {
                    EventKeyPressed event(static_cast<KeyCode>(key), false);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE: {
                    EventKeyReleased event(static_cast<KeyCode>(key));
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_REPEAT: {
                    EventKeyPressed event(static_cast<KeyCode>(key), true);
                    data.eventCallbackFn(event);
                    break;
                }
            }
            });

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

		return 0;
	}

    Window::~Window() {
        shutdown();
    }

    void Window::onUpdate() {

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