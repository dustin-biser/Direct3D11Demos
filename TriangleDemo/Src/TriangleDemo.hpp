#pragma once

#include <Dx11DemoBase.hpp>

class TriangleDemo : public Dx11DemoBase {
public:
	TriangleDemo (
		uint width,
		uint height,
		std::string name,
		float desiredFramesPerSecond = 60.0f
	);

	virtual ~TriangleDemo();

protected:

	void createVertexShaderObject();
	void createPixelShaderObject();
	void uploadVertexBufferData();

	//-- Inherited from Dx11DemoBase:
	void init() override;
	void appLogic(float dt) override;
	void render() override;
	void shutdown() override;


	//-- Shader Objects:
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;

	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11Buffer> m_vertexBuffer;

};
