#include <Windows.h>

#include "TriangleDemo.hpp"

//---------------------------------------------------------------------------------------
_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	TriangleDemo demo(1024, 768, "Triangle Direct3D Demo", 30.0f);
	return demo.run(hInstance, nCmdShow);
}
