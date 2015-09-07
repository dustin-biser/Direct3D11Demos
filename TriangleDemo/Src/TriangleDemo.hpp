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

protected:
	void LoadContent();

	void UnloadContent();

	void CreateVertexShaderObject();

	void CreatePixelShaderObject();

	void UploadVertexBufferData();

	//-- Inherited from Dx11DemoBase:
	virtual void Update(float dt) override;
	virtual void Render() override;


	//-- Shader Objects:
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;

	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11Buffer> m_vertexBuffer;

};
