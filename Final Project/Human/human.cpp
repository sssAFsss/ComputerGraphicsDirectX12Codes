
#include"d3dUtility.h"
#include "addition.h"
#include "PlaneCreator.h"
#include <vector>


//just movement
void movmentX(float direction);
void movmentUp(float direction);
void movmentZ(float direction);
//hand , feet animation
void animation(float degreeToRotate);

//create sky and flore and set positions of it
void create_flore_sky();

//set position of action wall
void set_action_wall();

//function that get first position and moved position of object and currentPosition of object and a rotation to rotate
D3DXMATRIX rotationInAnyPosition(D3DXMATRIX currentPos, Position firstPosition, Position movedPostion
	, DegreeWithDirection rotateToset);

//matrix that returns translate input and return it
D3DXMATRIX translateObjects(D3DXMATRIX objectToTranslate, Position move);

//CreateMeshFromFileFunction
bool createMeshFromFile
(ID3DXMesh*& Mesh, std::vector<D3DMATERIAL9>& Mtrls, std::vector<IDirect3DTexture9*>& Textures, LPCWSTR fileName);

//drawXFile this function should call in beginScene
void drawXFile
(ID3DXMesh* Mesh, std::vector<D3DMATERIAL9>Mtrls, std::vector<IDirect3DTexture9*> Textures, D3DXMATRIX World);

//Function for Cleanup that clean mesh we add with XFile
void cleanupXFile(ID3DXMesh* Mesh, std::vector<IDirect3DTexture9*> Textures);

//Set postion of inserted files
void posXFiles();

//shadow of human
void RenderShadow();

IDirect3DDevice9* Device = 0;

const int Width = 1080;
const int Height = 720;

const int numOfObject = 14;
ID3DXMesh* Objects[numOfObject] = { 0 };
D3DXMATRIX  Worlds[numOfObject];
D3DMATERIAL9 Mtrls[numOfObject];

//sky and flore
D3DXMATRIX Worlds_sky_flore[2];
IDirect3DTexture9* TexFlore = 0;
IDirect3DTexture9* TexSky = 0;
PlaneCreator* flore = 0;
PlaneCreator* sky = 0;
D3DMATERIAL9 white_material;

//create wall for action
D3DXMATRIX world_action_wall;
IDirect3DTexture9* Tex_action_wall = 0;
PlaneCreator* action_wall = 0;

//human rotation animation speed and movment speed
float humanRotationSpeed = 0.1f;
float humanRotation = 0.1f;
float speed = 0.1;
// positions of movements
float movedInX = 0.0f;
float movedInY = 0.0f;
float movedInZ = 0.0f;
// positions of camera
float CameraMovedInX = 0.0f;
float CameraMovedInY = 3.0f;
float CameraMovedInZ = 8.0f;

//create mesh to load as XFile
D3DXMATRIX world_car, world_jet;
ID3DXMesh* mesh_car = 0;
std::vector<D3DMATERIAL9>mtrls_car(0);
std::vector<IDirect3DTexture9*> textures_car(0);
ID3DXMesh* mesh_jet = 0;
std::vector<D3DMATERIAL9>mtrls_jet(0);
std::vector<IDirect3DTexture9*> textures_jet(0);

//move of action wall 
float action_wall_move = 0.25f;
float speed_action_wall = 0.25f;

//
// Framework Functions
//
bool Setup()
{
	createMeshFromFile(mesh_car, mtrls_car, textures_car, L"car.x");
	createMeshFromFile(mesh_jet, mtrls_jet, textures_jet, L"jet.x");
	 create_flore_sky();
	 posXFiles();
	 set_action_wall();

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
	D3DXCreateBox(Device, 0, 34, 250, &Objects[12], 0); // آسمون 3
	D3DXCreateBox(Device, 0, 34, 250, &Objects[13], 0); // آسمون 4



	//
	// Build world matrices - position the objects in world space.
	//




	D3DXMatrixTranslation(&Worlds[0], 1.3f, 2.5f, 0.0f);
	D3DXMatrixTranslation(&Worlds[1], 0.0f, 5.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[2], -0.5f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[3], 0.5f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[4], 0.0f, 2.1f, 0.0f);
	D3DXMatrixTranslation(&Worlds[5], -1.3f, 2.5f, 0.0f);
	D3DXMatrixTranslation(&Worlds[6], 0.5f, -1.8f, 0.0f);
	D3DXMatrixTranslation(&Worlds[7], -0.5f, -1.8f, 0.0f);
	D3DXMatrixTranslation(&Worlds[8], 0.0f, 30.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[9], 0.0f, -3.9f, 0.0f);
	D3DXMatrixTranslation(&Worlds[10], 0.0f, 13.0f, 125.0f);
	D3DXMatrixTranslation(&Worlds[11], 0.0f, 13.0f, -125.0f);
	D3DXMatrixTranslation(&Worlds[12], 125.0f, 13.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[13], -125.0f, 13.0f, 0.0f);

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
	white_material = d3d::WHITE_MTRL;
	//
	// Setup a directional light.
	//

	//D3DXVECTOR3 dir(1.0f, -0.0f, 0.25f);
	D3DXVECTOR3 dir(2.0f, 1.0f, 2.0f);
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
	for (int i = 0; i < numOfObject; i++)
		d3d::Release<ID3DXMesh*>(Objects[i]);
	cleanupXFile(mesh_car, textures_car);
	cleanupXFile(mesh_jet, textures_jet);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		// 
		// Update the scene: update camera position.
		//
		float speedCamera = 0.1f;


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
				//check if human close to wall it go down
				if (action_wall_move >= 0)
				{
					world_action_wall = translateObjects(world_action_wall, Position(0.0f, -speed_action_wall, 0.0f));
					action_wall_move -= speed_action_wall;
				}
				humanRotation += humanRotationSpeed;
				animation(humanRotationSpeed);
				movmentZ(+1.0f);

				//test
				CameraMovedInZ += speed;
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
				//check if human close to wall it go top
				if (movedInZ <= -20.0f)
				{
					action_wall_move += speed_action_wall;
					world_action_wall = translateObjects(world_action_wall, Position(0.0f, speed_action_wall, 0.0f));
				}
				humanRotation += humanRotationSpeed;
				animation(humanRotationSpeed);
				movmentZ(-1.0f);

				//test
				CameraMovedInZ -= speed;

			}


		}
		if (::GetAsyncKeyState(0x41) & 0x8000f) // Key(A)
		{
			if (movedInX >= -123.0f)
			{
				movmentX(-1.0f); // go left
				//test
				CameraMovedInX -= speed;
			}

		}
		if (::GetAsyncKeyState(0x44) & 0x8000f) // Key(D)
		{
			if (movedInX <= 123.0f)
			{
				movmentX(+1.0f); // go right
				//test
				CameraMovedInX += speed;
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
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00000000, 1.0f, 0);
		Device->BeginScene();
		flore->draw(&Worlds_sky_flore[0], &white_material, TexFlore);
		sky->draw(&Worlds_sky_flore[1], &white_material, TexSky);
		action_wall->draw(&world_action_wall, &white_material, Tex_action_wall);
		for (int i = 0; i < numOfObject; i++)
		{
			// set material and world matrix for ith object, then render
			// the ith object.
			if (i == 8 || i == 9)
				continue;
			

			
			Device->SetMaterial(&Mtrls[i]);
			Device->SetTransform(D3DTS_WORLD, &Worlds[i]);
			Objects[i]->DrawSubset(0);
		}
		RenderShadow();
		drawXFile(mesh_car, mtrls_car, textures_car, world_car);
		drawXFile(mesh_jet, mtrls_jet, textures_jet, world_jet);

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
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
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
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

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

void create_flore_sky()
{

	D3DXMATRIX pos_sky, pos_flore;
	D3DXMATRIX rotation_sky, rotation_flore;
	D3DXMatrixRotationX(&rotation_flore, D3DX_PI * 0.5);
	D3DXMatrixRotationX(&rotation_sky, 3 * D3DX_PI * 0.5);
	D3DXMatrixTranslation(&Worlds_sky_flore[0], 0.0f, 0.0f, 0.0f); //flore
	D3DXMatrixTranslation(&pos_flore, 0.0f, -4.2f, 0.0f);

	D3DXMatrixTranslation(&Worlds_sky_flore[1], 0.0f, 0.0f, 0.0f); //sky
	D3DXMatrixTranslation(&pos_sky, 0.0f, 28.0f, 0.0f);

	Worlds_sky_flore[0] = rotation_flore * pos_flore;
	Worlds_sky_flore[1] = rotation_sky * pos_sky;

	D3DXCreateTextureFromFile(
		Device,
		L"Flore_A.jpg",
		&TexFlore);
	D3DXCreateTextureFromFile(
		Device,
		L"sky3.jpg",
		&TexSky);

	flore = new PlaneCreator(Device);
	sky = new PlaneCreator(Device);

}

bool createMeshFromFile(ID3DXMesh*& Mesh, std::vector<D3DMATERIAL9>& Mtrls, std::vector<IDirect3DTexture9*>& Textures, LPCWSTR fileName)
{
	HRESULT hr = 0;

	//
	// Load the XFile data.
	//

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;


	hr = D3DXLoadMeshFromX(
		fileName,
		D3DXMESH_MANAGED,
		Device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&Mesh);

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}
	//
	// Extract the materials, and load textures.
	//

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back(mtrls[i].MatD3D);

			// check if the ith material has an associative texture
			if (mtrls[i].pTextureFilename != 0)
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					Device,
					(LPCWSTR)mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back(tex);
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back(0);
			}
		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer

	//
	// Optimize the mesh.
	//

	hr = Mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	d3d::Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	if (FAILED(hr))
	{
		::MessageBox(0, L"OptimizeInplace() - FAILED", 0, 0);
		return false;
	}
}



void drawXFile(ID3DXMesh* Mesh, std::vector<D3DMATERIAL9> Mtrls, std::vector<IDirect3DTexture9*> Textures, D3DXMATRIX World)
{
	for (int i = 0; i < Mtrls.size(); i++)
	{
		Device->SetMaterial(&Mtrls[i]);
		Device->SetTexture(0, Textures[i]);
		Device->SetTransform(D3DTS_WORLD, &World);
		Mesh->DrawSubset(i);
	}
}

void cleanupXFile(ID3DXMesh* Mesh, std::vector<IDirect3DTexture9*> Textures)
{
	d3d::Release<ID3DXMesh*>(Mesh);

	for (int i = 0; i < Textures.size(); i++)
		d3d::Release<IDirect3DTexture9*>(Textures[i]);
}
void posXFiles()
{
	D3DXMATRIX rotation_motor;
	D3DXMATRIX trans_motor;
	D3DXMatrixRotationY(&rotation_motor, D3DX_PI * 0.5);
	D3DXMatrixTranslation(&world_car, -40.0f, 0.0f, -100.0f);
	D3DXMatrixTranslation(&trans_motor, 40.0f, 2.00f, -100.0f);

	world_jet = trans_motor;
}
void set_action_wall()
{
	D3DXMATRIX pos_action_wall;
	D3DXMATRIX rotation_action_wall;
	D3DXMatrixRotationY(&rotation_action_wall, D3DX_PI);
	D3DXMatrixTranslation(&world_action_wall, 0.0f, 0.0f, 0.0f);// action_wall
	D3DXMatrixTranslation(&pos_action_wall, 0.0f, 0.0f, -90.0f);

	world_action_wall = rotation_action_wall * pos_action_wall;

	D3DXCreateTextureFromFile(
		Device,
		L"Wall_A.jpg",
		&Tex_action_wall);

	action_wall = new PlaneCreator(Device);
}
void RenderShadow()
{
	Device->SetRenderState(D3DRS_STENCILENABLE, true);
	Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	Device->SetRenderState(D3DRS_STENCILREF, 0x0);
	Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR); // increment to 1

	// position shadow

	D3DXVECTOR4 lightDirection(2.0f, 2.0f, 2.0f, 0.0f);
	D3DXPLANE groundPlane(0.0f, 1.0f, 0.0f, 0.0f);

	D3DXMATRIX S, translation, W;
	D3DXMatrixShadow(
		&S,
		&lightDirection,
		&groundPlane);
	D3DXMatrixTranslation(&translation, -4.2f, -4.2f, -4.2f); //1.85
	S *= translation;

	// alpha blend the shadow
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, 0.0f);
	mtrl.Diffuse.a = 0.01f; // 50% transparency.
	mtrl.Ambient.a = 0.5f;
	// Disable depth buffer so that z-fighting doesn't occur when we
	// render the shadow on top of the floor.
	Device->SetRenderState(D3DRS_ZENABLE, false);

	for (int i = 0; i < 8; i++)
	{
		W = Worlds[i] * S;
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&mtrl);
		Objects[i]->DrawSubset(0);
	}


	Device->SetRenderState(D3DRS_ZENABLE, true);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Device->SetRenderState(D3DRS_STENCILENABLE, false);
}