//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: directionalLight.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Demonstrates using a directional light with D3DX objects.  You can orbit
//       the scene using the left and right arrow keys.  In addition you can 
//       elevate the camera with the up and down arrow keys.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"

//
// Globals
//

IDirect3DDevice9* Device = 0; 

const int Width  = 1080;
const int Height = 720;
 
const int numOfObject = 16;
ID3DXMesh* Objects[numOfObject] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
D3DXMATRIX  Worlds[numOfObject]; 
D3DMATERIAL9 Mtrls[numOfObject];

//
// Framework Functions
//
bool Setup()
{
	//
	// Create objects.
	//
	
	
	D3DXCreateBox(Device, 3, 0.7, 0.7, &Objects[0], 0); // دست راست
	D3DXCreateSphere(Device, 1.5f, 40, 40, &Objects[1], 0); // سر
	D3DXCreateSphere(Device, 0.38f, 20, 20, &Objects[2], 0); // مفصل اتصال بدن و پای چپ
	D3DXCreateSphere(Device, 0.38f, 20, 20, &Objects[3], 0); // مفصل اتصال بدن و پای راست
	D3DXCreateBox(Device, 2, 4.1, 0.7, &Objects[4], 0); // بدن
	D3DXCreateBox(Device, 3, 0.7, 0.7, &Objects[5], 0); // دست چپ
	D3DXCreateBox(Device, 0.75, 4, 0.75, &Objects[6], 0); // پای راست
	D3DXCreateBox(Device, 0.75, 4, 0.75, &Objects[7], 0); // پای چپ
	D3DXCreateBox(Device, 0.75, 0.75, 1.5, &Objects[8], 0); // کفش راست
	D3DXCreateBox(Device, 0.75, 0.75, 1.5, &Objects[9], 0); // کفش چپ
	D3DXCreateBox(Device, 250, 0, 250, &Objects[10], 0); // آسمون
	D3DXCreateBox(Device, 250, 0, 250, &Objects[11], 0); // زمین
	D3DXCreateBox(Device, 250, 34, 0, &Objects[12], 0); // آسمون 1
	D3DXCreateBox(Device, 250, 34, 0, &Objects[13], 0); // آسمون 2
	D3DXCreateBox(Device, 0, 34,  250, &Objects[14], 0); // آسمون 3
	D3DXCreateBox(Device, 0, 34,  250, &Objects[15], 0); // آسمون 4

	

	//
	// Build world matrices - position the objects in world space.
	//

	


	D3DXMatrixTranslation(&Worlds[0],   1.85f,  3.7f, 0.0f);
	D3DXMatrixTranslation(&Worlds[1],   0.0f,   5.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[2],  -0.5f,   0.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[3],   0.5f,   0.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[4],   0.0f,   2.1f, 0.0f);
	D3DXMatrixTranslation(&Worlds[5],  -1.8f,   3.7f, 0.0f);
	D3DXMatrixTranslation(&Worlds[6],   0.5f,  -1.8f, 0.0f);
	D3DXMatrixTranslation(&Worlds[7],  -0.5f,  -1.8f, 0.0f);
	D3DXMatrixTranslation(&Worlds[8],   0.5f,  -3.5f, -0.4f);
	D3DXMatrixTranslation(&Worlds[9],  -0.5f,  -3.5f, -0.4f);
	D3DXMatrixTranslation(&Worlds[10],  0.0f,  30.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[11],  0.0f,  -3.9f, 0.0f);
	D3DXMatrixTranslation(&Worlds[12],  0.0f,  13.0f, 125.0f);
	D3DXMatrixTranslation(&Worlds[13],  0.0f,  13.0f, -125.0f);
	D3DXMatrixTranslation(&Worlds[14],  125.0f,  13.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[15], -125.0f,  13.0f, 0.0f);

	//
	// Setup the object's materials.
	//

	Mtrls[0] = d3d::YELLOW_MTRL;
	Mtrls[1] = d3d::YELLOW_MTRL;
	Mtrls[2] = d3d::YELLOW_MTRL;
	Mtrls[3] = d3d::YELLOW_MTRL;
	Mtrls[4] = d3d::YELLOW_MTRL;
	Mtrls[5] = d3d::YELLOW_MTRL;
	Mtrls[6] = d3d::YELLOW_MTRL;
	Mtrls[7] = d3d::YELLOW_MTRL;
	Mtrls[8] = d3d::YELLOW_MTRL;
	Mtrls[9] = d3d::YELLOW_MTRL;
	Mtrls[10] = d3d::BLUE_MTRL;
	Mtrls[11] = d3d::GREEN_MTRL;
	Mtrls[12] = d3d::BLUE_MTRL;
	Mtrls[13] = d3d::BLUE_MTRL;
	Mtrls[14] = d3d::BLUE_MTRL;
	Mtrls[15] = d3d::BLUE_MTRL;

	//
	// Setup a directional light.
	//

	D3DXVECTOR3 dir(1.0f, -0.0f, 0.25f);
	D3DXCOLOR   c = d3d::WHITE;
	D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);

	//
	// Set and Enable the light.
	//

	Device->SetLight(0, &dirLight);
	Device->LightEnable(0, true);

	//
	// Set lighting related render states.
	//

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, false);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.6f, // 45 - degree
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
	for(int i = 0; i < numOfObject; i++)
		d3d::Release<ID3DXMesh*>(Objects[i]);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		// 
		// Update the scene: update camera position.
		//

		static float angle  = (3.0f * D3DX_PI) / 2.0f;
		static float height = 5.0f;
	
		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			angle -= 0.5f * timeDelta;

		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			angle += 0.5f * timeDelta;

		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			height += 5.0f * timeDelta;

		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			height -= 5.0f * timeDelta;

		D3DXVECTOR3 position( cosf(angle) * 7.0f, height, sinf(angle) * 7.0f );
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		Device->SetTransform(D3DTS_VIEW, &V);

		//
		// Draw the scene:
		//
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();

		for(int i = 0; i < numOfObject; i++)
		{
			// set material and world matrix for ith object, then render
			// the ith object.
			Device->SetMaterial(&Mtrls[i]);
			Device->SetTransform(D3DTS_WORLD, &Worlds[i]);
			Objects[i]->DrawSubset(0);
		}

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}


//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}