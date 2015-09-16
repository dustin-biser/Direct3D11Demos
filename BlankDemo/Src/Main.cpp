#include <Windows.h>

#include "BlankDemo.hpp"

//---------------------------------------------------------------------------------------
_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	std::shared_ptr<Dx11DemoBase> demo = BlankDemo::getInstance();
	return demo->run(hInstance, nCmdShow, 1024, 768, "Blank Direct3d Window");
}

