#pragma once

#include <Dx11DemoBase.hpp>

class IndexRenderingDemo : public Dx11DemoBase {
public:
	virtual ~IndexRenderingDemo();

	static std::shared_ptr<Dx11DemoBase> getInstance();

protected:
	IndexRenderingDemo() = default; // Prevent direct construction

	void createVertexShaderObject();
	void createPixelShaderObject();
	void uploadVertexDataToBuffer();
    void setShaderConstants();

	//-- Inherited from Dx11DemoBase:
	void init() override;
	void appLogic(float dt) override;
	void render() override;
	void shutdown() override;

	bool keyInputEvent( 
		HWND hWindow,
		UINT message,
		WPARAM vKey
	) override;

	bool mouseButtonEvent(
		HWND hWindow,
		UINT message,
		WPARAM wParam
	) override;


	//-- Shader Objects:
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;

	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11Buffer> m_constantBuffer;
};

