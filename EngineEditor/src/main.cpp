#include <iostream>
#include <memory>
#include <EngineCore/Application.h>
#include <EngineCore/Input.hpp>
#include <imgui/imgui.h>

class EngineEditor : public Engine::Application {
	virtual void onUpdate() override {

		bool moveCamera = false;
		glm::vec3 movementDelta{ 0, 0, 0 };
		glm::vec3 rotationDelta{ 0, 0, 0 };
		//std::cout << "Update Frame: " << frame++ << std::endl;
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_W)) {
			movementDelta.x += 0.01f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_S)) {
			movementDelta.x -= 0.01f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_A)) {
			movementDelta.y -= 0.01f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_D)) {
			movementDelta.y += 0.01f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_R)) {
			movementDelta.z += 0.01f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_F)) {
			movementDelta.z -= 0.01f;
			moveCamera = true;
		}
		// rotation
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_UP)) {
			rotationDelta.y += 0.5f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_DOWN)) {
			rotationDelta.y -= 0.5f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_RIGHT)) {
			rotationDelta.z -= 0.5f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_LEFT)) {
			rotationDelta.z += 0.5f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_P))
		{
			rotationDelta.x += 0.5f;
			moveCamera = true;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_O))
		{
			rotationDelta.x -= 0.5f;
			moveCamera = true;
		}

		if (moveCamera)
		{
			camera.addMovementAndRotatition(movementDelta, rotationDelta);
		}
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