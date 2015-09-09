#include "BlankDemo.hpp"

#include <cassert>


//---------------------------------------------------------------------------------------
BlankDemo::BlankDemo(
	uint width,
	uint height, 
	std::string name
) : 
	Dx11DemoBase(width, height, name)
{

}

//---------------------------------------------------------------------------------------
BlankDemo::~BlankDemo() {

}

//---------------------------------------------------------------------------------------
void BlankDemo::Update(float dt) {

}

//---------------------------------------------------------------------------------------
void BlankDemo::Render() {
	if (!m_d3dContext) return;

	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };

	// Clear the back buffer with the clearColor.
	m_d3dContext->ClearRenderTargetView(m_backBufferTarget.Get(), clearColor);

	// Do any additional rendering here.

	// Swap the back and front buffers.
	m_swapChain->Present(0, 0);
}
