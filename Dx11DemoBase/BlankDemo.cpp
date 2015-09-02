#include "BlankDemo.hpp"


//---------------------------------------------------------------------------------------
BlankDemo::BlankDemo (
	uint width,
	uint height, 
	std::wstring name
) : 
	Dx11DemoBase(width, height, name)
{

}

//---------------------------------------------------------------------------------------
BlankDemo::~BlankDemo() {

}

//---------------------------------------------------------------------------------------
bool BlankDemo::LoadContent() {
	return true;
}

//---------------------------------------------------------------------------------------
void BlankDemo::UnloadContent() {

}

//---------------------------------------------------------------------------------------
void BlankDemo::Update(float dt) {

}

//---------------------------------------------------------------------------------------
void BlankDemo::Render() {
	if (!d3dContext) return;

	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };

	// Clear the back buffer with the clearColor.
	d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);

	// Do any additional rendering here.

	// Swap the back and front buffers.
	swapChain->Present(0, 0);
}
