#pragma once

#include <Dx11DemoBase.hpp>

class TriangleDemo : public Dx11DemoBase {
public:

	TriangleDemo (
		uint width,
		uint height,
		std::string name
	);

	virtual ~TriangleDemo();

	bool LoadContent();

	void UnloadContent();

	// Inherited via Dx11DemoBase
	virtual void Update(float dt) override;

	virtual void Render() override;

};
