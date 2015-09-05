#include "TriangleDemo.hpp"



//---------------------------------------------------------------------------------------
TriangleDemo::TriangleDemo (
	uint width,
	uint height, 
	std::string name
)
	: Dx11DemoBase(width, height, name)
{

}

//---------------------------------------------------------------------------------------
TriangleDemo::~TriangleDemo()
{

}

//---------------------------------------------------------------------------------------
bool TriangleDemo::LoadContent()
{
	return true;
}

//---------------------------------------------------------------------------------------
void TriangleDemo::UnloadContent()
{

}

//---------------------------------------------------------------------------------------
void TriangleDemo::Update(float dt)
{

}

//---------------------------------------------------------------------------------------
void TriangleDemo::Render()
{
	if (!m_d3dContext) return;

	float clearColor[4] = { 0.8f, 0.3f, 0.30f, 1.0f };

	// Clear the back buffer with the clearColor.
	m_d3dContext->ClearRenderTargetView(m_backBufferTarget.Get(), clearColor);

	// Do any additional rendering here.

	// Swap the back and front buffers.
	m_swapChain->Present(0, 0);

}
