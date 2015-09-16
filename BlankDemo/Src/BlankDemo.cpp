#include "BlankDemo.hpp"

#include <cassert>


//---------------------------------------------------------------------------------------
BlankDemo::~BlankDemo() {

}

std::shared_ptr<Dx11DemoBase> BlankDemo::getInstance()
{
	static Dx11DemoBase * staticInstance = new BlankDemo();
	if (m_pInstance == nullptr) {
		m_pInstance = std::shared_ptr<Dx11DemoBase>(staticInstance);
	}

	return m_pInstance;
}

//---------------------------------------------------------------------------------------
void BlankDemo::appLogic(float dt)
{

}

//---------------------------------------------------------------------------------------
void BlankDemo::render() {
	if (!m_d3dContext) return;

	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };

	// Clear the back buffer with the clearColor.
	m_d3dContext->ClearRenderTargetView(m_backBufferTarget.Get(), clearColor);

	// Do any additional rendering here.

	// Swap the back and front buffers.
	m_swapChain->Present(0, 0);
}


//---------------------------------------------------------------------------------------
void BlankDemo::shutdown()
{

}
