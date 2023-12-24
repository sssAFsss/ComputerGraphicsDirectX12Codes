#include "d3dUtility.h"
#include "addition.h"

//just movement
void movmentX(float direction);
void movmentUp(float direction);
void movmentZ(float direction);
//hand , feet animation
void animation(float degreeToRotate);

//function that get first position and moved position of object and currentPosition of object and a rotation to rotate
D3DXMATRIX rotationInAnyPosition(D3DXMATRIX currentPos, Position firstPosition, Position movedPostion
	, DegreeWithDirection rotateToset);

//matrix that returns translate input and return it
D3DXMATRIX translateObjects(D3DXMATRIX objectToTranslate, Position move);

IDirect3DDevice9* Device = 0; 

const int Width  = 1080;
const int Height = 720;
 
const int numOfObject = 14;
ID3DXMesh* Objects[numOfObject] = {0};
D3DXMATRIX  Worlds[numOfObject]; 
D3DMATERIAL9 Mtrls[numOfObject];


//human rotation animation speed and movment speed
float humanRotationSpeed = 0.004f;
float humanRotation = 0.1f;
float speed = 0.1;
// positions of movements
float movedInX = 0.0f;
float movedInY = 0.0f;
float movedInZ = 0.0f;
// positions of camera
float CameraMovedInX = 0.0f;
float CameraMovedInY = 3.0f;
float CameraMovedInZ = -8.0f;


//
// Framework Functions
//
bool Setup()
{
	//
	// Create objects.
	//
	
	
	D3DXCreateBox(Device, 0.7, 3, 0.7, &Objects[0], 0); // دست راست
	D3DXCreateSphere(Device, 1.5f, 40, 40, &Objects[1], 0); // سر
	D3DXCreateSphere(Device, 0.38f, 20, 20, &Objects[2], 0); // مفصل اتصال بدن و پای چپ
	D3DXCreateSphere(Device, 0.38f, 20, 20, &Objects[3], 0); // مفصل اتصال بدن و پای راست
	D3DXCreateBox(Device, 2, 4.1, 0.7, &Objects[4], 0); // بدن
	D3DXCreateBox(Device, 0.7, 3, 0.7, &Objects[5], 0); // دست چپ
	D3DXCreateBox(Device, 0.75, 4, 0.75, &Objects[6], 0); // پای راست
	D3DXCreateBox(Device, 0.75, 4, 0.75, &Objects[7], 0); // پای چپ
	D3DXCreateBox(Device, 250, 0, 250, &Objects[8], 0); // آسمون
	D3DXCreateBox(Device, 250, 0, 250, &Objects[9], 0); // زمین
	D3DXCreateBox(Device, 250, 34, 0, &Objects[10], 0); // آسمون 1
	D3DXCreateBox(Device, 250, 34, 0, &Objects[11], 0); // آسمون 2
	D3DXCreateBox(Device, 0, 34,  250, &Objects[12], 0); // آسمون 3
	D3DXCreateBox(Device, 0, 34,  250, &Objects[13], 0); // آسمون 4

	

	//
	// Build world matrices - position the objects in world space.
	//

	


	D3DXMatrixTranslation(&Worlds[0],   1.3f,  2.5f, 0.0f);
	D3DXMatrixTranslation(&Worlds[1],   0.0f,   5.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[2],  -0.5f,   0.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[3],   0.5f,   0.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[4],   0.0f,   2.1f, 0.0f);
	D3DXMatrixTranslation(&Worlds[5],  -1.3f,   2.5f, 0.0f);
	D3DXMatrixTranslation(&Worlds[6],   0.5f,  -1.8f, 0.0f);
	D3DXMatrixTranslation(&Worlds[7],  -0.5f,  -1.8f, 0.0f);
	D3DXMatrixTranslation(&Worlds[8],  0.0f,  30.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[9],  0.0f,  -3.9f, 0.0f);
	D3DXMatrixTranslation(&Worlds[10],  0.0f,  13.0f, 125.0f);
	D3DXMatrixTranslation(&Worlds[11],  0.0f,  13.0f, -125.0f);
	D3DXMatrixTranslation(&Worlds[12],  125.0f,  13.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[13], -125.0f,  13.0f, 0.0f);

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
	Mtrls[8] = d3d::BLUE_MTRL;
	Mtrls[9] = d3d::GREEN_MTRL;
	Mtrls[10] = d3d::BLUE_MTRL;
	Mtrls[11] = d3d::BLUE_MTRL;
	Mtrls[12] = d3d::BLUE_MTRL;
	Mtrls[13] = d3d::BLUE_MTRL;

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
		float speedCamera = 0.05f;
		
	
				if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		{
			
			CameraMovedInX -= speedCamera;
		}
			

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		{

			CameraMovedInX += speedCamera;
		}
			

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		{

			CameraMovedInZ += speedCamera;
		}
		
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		{

			CameraMovedInZ -= speedCamera;
		}


		if (::GetAsyncKeyState(0x53) & 0x8000f) // Key(S)
		{
			if (movedInZ <= 123.0f)
			{
				//change direction of rotation 
				if (humanRotation >= 0.68f || humanRotation <= -0.68f)
				{
					humanRotationSpeed *= -1;

				}
				humanRotation += humanRotationSpeed;
				animation(humanRotationSpeed);
				movmentZ(+1.0f);
			}


		}
		if (::GetAsyncKeyState(0x57) & 0x8000f) // Key(W)
		{
		
			if (movedInZ >= -123.0f)
			{
				//change direction of rotation 
				if (humanRotation >= 0.68f || humanRotation <= -0.68f)
				{
					humanRotationSpeed *= -1;
				}
				humanRotation += humanRotationSpeed;
				animation(humanRotationSpeed);
				movmentZ(-1.0f);

			}
			

		}
		if (::GetAsyncKeyState(0x41) & 0x8000f) // Key(A)
		{
			if (movedInX >= -123.0f)
			{
				movmentX(-1.0f); // go left
			}

		}
		if (::GetAsyncKeyState(0x44) & 0x8000f) // Key(D)
		{
			if (movedInX <= 123.0f)
			{
				movmentX(+1.0f); // go right
			}

		}


		D3DXVECTOR3 position(CameraMovedInX, CameraMovedInY, CameraMovedInZ);
		D3DXVECTOR3 target(movedInX, movedInY, movedInZ);
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
int WINAPI WinMain(
	HINSTANCE hinstance,
	HINSTANCE prevInstance, 
	PSTR cmdLine,
	int showCmd
)
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


void animation(float degreeToRotate)
{
	Worlds[0] = rotationInAnyPosition(Worlds[0]
		, Position(1.3f, 3.5f, 0.0f)
		, Position(movedInX, movedInY, movedInZ)
		, DegreeWithDirection(+degreeToRotate, 'x'));
	Device->SetTransform(D3DTS_WORLD, &Worlds[0]);
	Objects[0]->DrawSubset(0);

	Worlds[5] = rotationInAnyPosition(Worlds[5]
		, Position(-1.3f, 3.5f, 0.0f)
		, Position(movedInX, movedInY, movedInZ)
		, DegreeWithDirection(-degreeToRotate, 'x'));
	Device->SetTransform(D3DTS_WORLD, &Worlds[5]);
	Objects[5]->DrawSubset(0);

	Worlds[6] = rotationInAnyPosition(Worlds[6]
		, Position(0.5f, 0.0f, 0.0f)
		, Position(movedInX, movedInY, movedInZ)
		, DegreeWithDirection(+degreeToRotate, 'x'));
	Device->SetTransform(D3DTS_WORLD, &Worlds[6]);
	Objects[6]->DrawSubset(0);

	Worlds[7] = rotationInAnyPosition(Worlds[7]
		, Position(-0.5f, 0.0f, 0.0f)
		, Position(movedInX, movedInY, movedInZ)
		, DegreeWithDirection(-degreeToRotate, 'x'));
	Device->SetTransform(D3DTS_WORLD, &Worlds[7]);
	Objects[7]->DrawSubset(0);
}

D3DXMATRIX rotationInAnyPosition(D3DXMATRIX currentPos, Position firstPosition,
	Position movedPostion, DegreeWithDirection rotateToset)
{
	D3DXMATRIX bringToZero, matrixRotation, bringBack;
	D3DXMatrixTranslation(&bringToZero, -(firstPosition.x + movedPostion.x)
		, -(firstPosition.y + movedPostion.y), -(firstPosition.z + movedPostion.z));
	//now the object is in 0.0.0
	currentPos *= bringToZero;
	switch (rotateToset.direction)
	{
	case 'x':
		D3DXMatrixRotationX(&matrixRotation, rotateToset.deg);
		break;
	case 'y':
		D3DXMatrixRotationX(&matrixRotation, rotateToset.deg);
		break;
	case 'z':
		D3DXMatrixRotationX(&matrixRotation, rotateToset.deg);
		break;
	}
	currentPos *= matrixRotation;
	//bring back object to position that it had
	D3DXMatrixTranslation(&bringBack, (firstPosition.x + movedPostion.x)
		, (firstPosition.y + movedPostion.y), (firstPosition.z + movedPostion.z));
	currentPos *= bringBack;
	return currentPos;
}

void movmentX(float direction)
{

	if (direction > 0)
	{
		movedInX += speed;
		for (int i = 0; i < 8; i++)
		{
			Worlds[i] = translateObjects(Worlds[i], Position(speed, 0.0f, 0.0f));
		}
	}
	else
	{
		movedInX -= speed;
		for (int i = 0; i < 8; i++)
		{
			Worlds[i] = translateObjects(Worlds[i], Position(-speed, 0.0f, 0.0f));
		}
	}
}
void movmentUp(float direction)
{
	if (direction > 0)
	{
		movedInY += speed;
		for (int i = 0; i < 8; i++)
		{
			Worlds[i] = translateObjects(Worlds[i], Position(0.0f, speed, 0.0f));
		}
	}
	else
	{
		movedInY -= speed;
		for (int i = 0; i < 8; i++)
		{
			Worlds[i] = translateObjects(Worlds[i], Position(0.0f, -speed, 0.0f));
		}
	}
}
void movmentZ(float direction)
{
	if (direction > 0)
	{
		movedInZ += speed;
		for (int i = 0; i < 8; i++)
		{
			Worlds[i] = translateObjects(Worlds[i], Position(0.0f, 0.0f, speed));
		}
	}
	else
	{
		movedInZ -= speed;
		for (int i = 0; i < 8; i++)
		{
			Worlds[i] = translateObjects(Worlds[i], Position(0.0f, 0.0f, -speed));
		}
	}



}
D3DXMATRIX translateObjects(D3DXMATRIX objectToTranslate, Position move)
{
	D3DXMATRIX helper;
	D3DXMatrixTranslation(&helper, move.x, move.y, move.z);
	return(objectToTranslate *= helper);
}
