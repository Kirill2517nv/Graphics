#include <iostream>
#include <memory>
#include <EngineCore/Application.h>
#include <EngineCore/Input.hpp>
#include <imgui/imgui.h>

class EngineEditor : public Engine::Application {
	double mInitialMoisePosX = 0;
	double mInitialMoisePosY = 0;

	virtual void onUpdate() override {
		glm::vec3 movementDelta{ 0, 0, 0 };
		glm::vec3 rotationDelta{ 0, 0, 0 };
		//std::cout << "Update Frame: " << frame++ << std::endl;
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_W)) {
			movementDelta.x += 0.01f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_S)) {
			movementDelta.x -= 0.01f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_A)) {
			movementDelta.y -= 0.01f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_D)) {
			movementDelta.y += 0.01f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_R)) {
			movementDelta.z += 0.01f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_F)) {
			movementDelta.z -= 0.01f;			
		}
		// rotation
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_UP)) {
			rotationDelta.y -= 0.5f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_DOWN)) {
			rotationDelta.y += 0.5f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_RIGHT)) {
			rotationDelta.z -= 0.5f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_LEFT)) {
			rotationDelta.z += 0.5f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_Q))
		{
			rotationDelta.x -= 0.5f;			
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_E))
		{
			rotationDelta.x += 0.5f;			
		}
		
		// rotate camera with a mouse using RMB
		if (Engine::Input::isMouseButtonPressed(Engine::MouseButton::MOUSE_BUTTON_RIGHT)) {
			// pan camera with two buttons pressed
			auto currentCursorPosition = getCurrentCursorPosition();
			
			if (Engine::Input::isMouseButtonPressed(Engine::MouseButton::MOUSE_BUTTON_LEFT)) {
				camera.moveRight((mInitialMoisePosX - currentCursorPosition.x) / 100.0);
				camera.moveUp((mInitialMoisePosY - currentCursorPosition.y) / 100.0);
			}
			else {
				rotationDelta.z += (mInitialMoisePosX - currentCursorPosition.x) / 5.0; // yaw
				rotationDelta.y -= (mInitialMoisePosY - currentCursorPosition.y) / 5.0; // pitch
			}

			mInitialMoisePosX = currentCursorPosition.x;
			mInitialMoisePosY = currentCursorPosition.y;
		}

		camera.addMovementAndRotation(movementDelta, rotationDelta);
	}

	virtual void onMouseButtonEvent(const Engine::MouseButton button, const double xPos, const double yPos, const bool pressed) override {
		mInitialMoisePosX = xPos;
		mInitialMoisePosY = yPos;
	}

	virtual void onUiDraw() override
	{
		cameraPosition[0] = camera.getCameraPosition().x;
		cameraPosition[1] = camera.getCameraPosition().y;
		cameraPosition[2] = camera.getCameraPosition().z;
		cameraRotation[0] = camera.getCameraRotation().x;
		cameraRotation[1] = camera.getCameraRotation().y;
		cameraRotation[2] = camera.getCameraRotation().z;

		ImGui::Begin("Editor");
		if (ImGui::SliderFloat3("camera position", cameraPosition, -10.f, 10.f))
		{
			camera.setPosition(glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]));
		}
		if (ImGui::SliderFloat3("camera rotation", cameraRotation, 0, 360.f))
		{
			camera.setRotation(glm::vec3(cameraRotation[0], cameraRotation[1], cameraRotation[2]));
		}
		ImGui::Checkbox("Perspective camera", &perspectiveCamera);
		ImGui::End();
	}

	int frame = 0;
};

int main()
{
	auto pEngineEditor = std::make_unique<EngineEditor>();

	int returnCode = pEngineEditor->start(1024, 768, "Engine Editor");

	return returnCode;
}