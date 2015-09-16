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

	virtual void appLogic(float dt) override;

	virtual void render() override;

	virtual void shutdown() override;

};
