#include <iostream>
#include <memory>
#include <EngineCore/Application.h>
#include <imgui/imgui.h>

class EngineEditor : public Engine::Application {
	virtual void onUpdate() override {
		//std::cout << "Update Frame: " << frame++ << std::endl;
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