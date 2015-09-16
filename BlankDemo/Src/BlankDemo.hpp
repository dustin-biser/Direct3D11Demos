#pragma once

#include "Dx11DemoBase.hpp" 

class BlankDemo : public Dx11DemoBase {
public:
	BlankDemo (
		uint width,
		uint height,
		std::string name
	);

	virtual ~BlankDemo();

	static std::shared_ptr<Dx11DemoBase> getInstance();

	virtual void appLogic(float dt) override;

	virtual void render() override;

	virtual void shutdown() override;

protected:
	BlankDemo() = default; // Prevent direct construction

	virtual void init() override;

	virtual bool keyInputEvent(HWND hWindow, UINT message, WPARAM vKey) override;

	virtual bool mouseButtonEvent(HWND hWindow, UINT message, WPARAM wParam) override;

};
