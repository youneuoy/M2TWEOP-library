#pragma once
#include"headersSTD.h"
#include "headersMEM.h"
#include "headersGraphics.h"
#include "globals.h"
#include "techFuncs.h"

#include "exportHeader.h"

class testD3D
{
public:
	HRESULT InitGeometry(LPDIRECT3DDEVICE9 device)
	{
		g_pd3dDevice = device;
		LPD3DXBUFFER pD3DXMtrlBuffer;

		// Load the mesh from the specified file
		if (FAILED(D3DXLoadMeshFromX(L"Tiger.x", D3DXMESH_SYSTEMMEM,
			g_pd3dDevice, NULL,
			&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
			&g_pMesh)))
		{
			// If model is not in current folder, try parent folder
			if (FAILED(D3DXLoadMeshFromX(L"..\\Tiger.x", D3DXMESH_SYSTEMMEM,
				g_pd3dDevice, NULL,
				&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
				&g_pMesh)))
			{
				MessageBox(NULL, L"Could not find tiger.x", L"Meshes.exe", MB_OK);
				return E_FAIL;
			}
		}

		// We need to extract the material properties and texture names from the 
		// pD3DXMtrlBuffer
		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
		g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
		if (g_pMeshMaterials == NULL)
			return E_OUTOFMEMORY;
		g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
		if (g_pMeshTextures == NULL)
			return E_OUTOFMEMORY;

		for (DWORD i = 0; i < g_dwNumMaterials; i++)
		{
			// Copy the material
			g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

			// Set the ambient color for the material (D3DX does not do this)
			g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

			g_pMeshTextures[i] = NULL;
			if (d3dxMaterials[i].pTextureFilename != NULL &&
				lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
			{
				// Create the texture
				if (FAILED(D3DXCreateTextureFromFileA(g_pd3dDevice,
					d3dxMaterials[i].pTextureFilename,
					&g_pMeshTextures[i])))
				{
					// If texture is not in current folder, try parent folder
					const CHAR* strPrefix = "..\\";
					CHAR strTexture[MAX_PATH];
					strcpy_s(strTexture, MAX_PATH, strPrefix);
					strcat_s(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
					// If texture is not in current folder, try parent folder
					if (FAILED(D3DXCreateTextureFromFileA(g_pd3dDevice,
						strTexture,
						&g_pMeshTextures[i])))
					{
						MessageBox(NULL, L"Could not find texture map", L"Meshes.exe", MB_OK);
					}
				}
			}
		}

		// Done with the material buffer
		pD3DXMtrlBuffer->Release();

		return S_OK;
	}

	int draw()
	{
		// Backup the DX9 state
		IDirect3DStateBlock9* d3d9_state_block = NULL;
		if (g_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
			return 0;
		if (d3d9_state_block->Capture() < 0)
		{
			d3d9_state_block->Release();
			return 0;
		}
		// Backup the DX9 transform (DX9 documentation suggests that it is included in the StateBlock but it doesn't appear to)
		D3DMATRIX last_world, last_view, last_projection;
		g_pd3dDevice->GetTransform(D3DTS_WORLD, &last_world);
		g_pd3dDevice->GetTransform(D3DTS_VIEW, &last_view);
		g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &last_projection);


		pStateBlock->Apply();


		// Turn on the zbuffer
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

		// Turn on ambient lighting 
		g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
		SetupMatrices();
		for (DWORD i = 0; i < g_dwNumMaterials; i++)
		{
			// Set the material and texture for this subset
		    g_pd3dDevice->SetMaterial(&g_pMeshMaterials[i]);
			g_pd3dDevice->SetTexture(0, g_pMeshTextures[i]);

			// Draw the mesh subset
			g_pMesh->DrawSubset(i);
		}

		// Restore the DX9 transform
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &last_world);
		g_pd3dDevice->SetTransform(D3DTS_VIEW, &last_view);
		g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &last_projection);

		// Restore the DX9 state
		d3d9_state_block->Apply();
		d3d9_state_block->Release();
	}

	VOID SetupMatrices()
	{
		// Set up world matrix
		D3DXMATRIXA16 matWorld;
		//D3DXMatrixRotationY(&matWorld, 1);
		D3DXMatrixIdentity(&matWorld);
		//g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		static float worldCoords[3]{100,3,100};
		matWorld[12] = worldCoords[0];
		matWorld[13] = worldCoords[1];
		matWorld[14] = worldCoords[2];

		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// Set up our view matrix. A view matrix can be defined given an eye point,
		// a point to lookat, and a direction for which way is up. Here, we set the
		// eye five units back along the z-axis and up three units, look at the 
		// origin, and define "up" to be in the y-direction.

		static float vEyePtF[3]{0,3,5};
		static float vLookatPtF[3]{0,0,0};
		static float vUpVecF[3]{0,1,0};



		ImGui::Begin("Matrixes settings	");

		ImGui::InputFloat3("modelCoordinates", (float*)&worldCoords, "%.1f");

		ImGui::InputFloat3("vEyePt", (float*)&vEyePtF, "%.1f");
		ImGui::InputFloat3("vLookatPt", (float*)&vLookatPtF, "%.1f");
		ImGui::InputFloat3("vUpVec", (float*)&vUpVecF, "%.1f");
		ImGui::End();

		D3DXVECTOR3 vEyePt(vEyePtF[0], vEyePtF[1], vEyePtF[2]);
		D3DXVECTOR3 vLookatPt(vLookatPtF[0], vLookatPtF[1], vLookatPtF[2]);
		D3DXVECTOR3 vUpVec(vUpVecF[0], vUpVecF[1], vUpVecF[2]);
		D3DXMATRIXA16 matView;
		//D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

		float camCoords[3];
		float camPitch;
		techFuncs::Read(0x0193D538, &camCoords, 4*3);
		techFuncs::Read(0x0193D628, &camPitch, 4);		
		
		D3DXMATRIXA16 camTransMat;

		D3DXMatrixTranslation(&camTransMat, -camCoords[0], -camCoords[1], -camCoords[2]);



		D3DXMATRIXA16 camRotMat;
		D3DXMatrixRotationX(&camRotMat, -camPitch / 2 * D3DX_PI);
		D3DXMatrixMultiply(&matView, &camRotMat, &camTransMat);

		techFuncs::Read(0x0193D604, &matView, 16 * 4);

		g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		// For the projection matrix, we set up a perspective transform (which
		// transforms geometry from 3D view space to 2D viewport space, with
		// a perspective divide making objects smaller in the distance). To build
		// a perpsective transform, we need the field of view (1/4 pi is common),
		// the aspect ratio, and the near and far clipping planes (which define at
		// what distances geometry should be no longer be rendered).
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		techFuncs::Read(0x02C9E0F8, &matProj, 16 * 4);
		g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	}

	void onResetDevice()
	{
		if (pStateBlock != NULL)
			pStateBlock->Release();
	}
	void afterResetDevice()
	{
		g_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &pStateBlock);
	}

private:
	LPDIRECT3DDEVICE9 g_pd3dDevice;

	IDirect3DStateBlock9* pStateBlock = NULL;

	LPD3DXMESH          g_pMesh = NULL; // Our mesh object in sysmem
	D3DMATERIAL9* g_pMeshMaterials = NULL; // Materials for our mesh
	LPDIRECT3DTEXTURE9* g_pMeshTextures = NULL; // Textures for our mesh
	DWORD               g_dwNumMaterials = 0L;   // Number of mesh materials
};




class graphicsD3D
{
public:
	static struct dataT
	{
		IDirect3DDevice9* pDevice = nullptr;

		HWND Window = nullptr;


		//if imgui initialized
		bool ImInitialized = false;
		bool ifMouseOrKeyBoardAtImgui = false;
		struct 
		{
			WNDPROC oWndProc = NULL;
			// Originals
			WNDPROC onewGameWndProc = NULL;
		}hookD;

		testD3D d3dT;
	}dataS;
	static NOINLINE void APIENTRY Draw(LPDIRECT3DDEVICE9 pDevice);



	static NOINLINE LRESULT APIENTRY hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static NOINLINE LRESULT APIENTRY hkWndProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static NOINLINE void APIENTRY initImgGui(IDirect3DDevice9* pDevice);
	static bool init();
	static DWORD WINAPI InitS();


};


namespace graphicsExport
{
	NOINLINE EOP_EXPORT LPDIRECT3DTEXTURE9 loadTexture(const char*path, int* x, int* y);
	NOINLINE EOP_EXPORT void unloadTexture(LPDIRECT3DTEXTURE9 texture);


	NOINLINE EOP_EXPORT void onCreateDevice(IDirect3DDevice9* pDevice);
	NOINLINE EOP_EXPORT void onEndScene(IDirect3DDevice9* pDevice);
	NOINLINE EOP_EXPORT void onReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	NOINLINE EOP_EXPORT void afterReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
};