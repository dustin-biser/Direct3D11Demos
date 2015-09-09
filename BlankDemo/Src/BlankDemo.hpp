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

	void Update(float dt) override;

	void Render() override;
};
