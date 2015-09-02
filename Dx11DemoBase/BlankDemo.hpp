#pragma once

#include "Dx11DemoBase.hpp" 

class BlankDemo : public Dx11DemoBase {
public:
	BlankDemo (
		uint width,
		uint height,
		std::wstring name
	);

	virtual ~BlankDemo();

	bool LoadContent();

	void UnloadContent();

	virtual void Update(float dt) override;

	virtual void Render() override;
};
