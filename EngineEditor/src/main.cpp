#include <iostream>
#include <memory>
#include <EngineCore/Application.h>

class App : public Engine::Application {
	virtual void onUpdate() override {
		//std::cout << "Update Frame: " << frame++ << std::endl;
	}

	int frame = 0;
};

int main()
{
	auto testApp = std::make_unique<App>();

	int returnCode = testApp->start(1024, 768, "APP TESTING");

	return returnCode;
}