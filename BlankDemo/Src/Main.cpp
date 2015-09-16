#include <Windows.h>

#include "BlankDemo.hpp"

//---------------------------------------------------------------------------------------
_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	BlankDemo demo(1024, 768, "Blank Direct3d Window");
	return demo.run(hInstance, nCmdShow);
}

