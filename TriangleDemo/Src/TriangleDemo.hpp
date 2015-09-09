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

	void CreateVertexShaderObject();

	void CreatePixelShaderObject();

	void UploadVertexBufferData();

	//-- Inherited from Dx11DemoBase:
	void LoadContent() override;
	void UnloadContent() override;
	void Update(float dt) override;
	void Render() override;


	//-- Shader Objects:
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;

	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11Buffer> m_vertexBuffer;

};
