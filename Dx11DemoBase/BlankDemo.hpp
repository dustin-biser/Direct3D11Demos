#pragma once

#include "DxD11DemoBase.hpp" 

class BlankDemo : public DxD11DemoBase {
public:
	BlankDemo();

	virtual ~BlankDemo();

	bool LoadContent();

	void UnloadContent();

	virtual void Update(float dt) override;

	virtual void Render() override;
};
