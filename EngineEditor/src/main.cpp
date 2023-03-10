#include <iostream>
#include <memory>
#include <EngineCore/Application.h>
#include <EngineCore/Input.hpp>
#include <imgui/imgui.h>

class EngineEditor : public Engine::Application {
	virtual void onUpdate() override {
		//std::cout << "Update Frame: " << frame++ << std::endl;
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_W)) {
			cameraPosition[2] -= 0.01f;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_S)) {
			cameraPosition[2] += 0.01f;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_A)) {
			cameraPosition[0] -= 0.01f;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_D)) {
			cameraPosition[0] += 0.01f;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_R)) {
			cameraPosition[1] += 0.01f;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_F)) {
			cameraPosition[1] -= 0.01f;
		}
		// rotation
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_UP)) {
			cameraRotation[0] += 0.5f;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_DOWN)) {
			cameraRotation[0] -= 0.5f;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_RIGHT)) {
			cameraRotation[1] -= 0.5f;
		}
		if (Engine::Input::isKeyPressed(Engine::KeyCode::KEY_LEFT)) {
			cameraRotation[1] += 0.5f;
		}
	}

	virtual void onUiDraw() override
	{
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("camera position", cameraPosition, -10.f, 10.f);
		ImGui::SliderFloat3("camera rotation", cameraRotation, 0, 360.f);
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