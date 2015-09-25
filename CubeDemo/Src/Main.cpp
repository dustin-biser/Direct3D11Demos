#include <Windows.h>

#include "CubeDemo.hpp"

//---------------------------------------------------------------------------------------
_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	std::shared_ptr<Dx11DemoBase> demo = CubeDemo::getInstance();
	return demo->run (hInstance, nCmdShow, 1024, 768, "Cube Demo - DirectX11");
}
