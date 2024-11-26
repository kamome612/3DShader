#pragma once

//�C���N���[�h
#include <d3d11.h>
#include <DirectXMath.h>

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

enum SHADER_TYPE
{
	SHADER_2D,
	SHADER_3D,
	SHADER_CRT,
	SHADER_POINT,
	SHADER_MAX
};

namespace Direct3D
{
	//������
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	HRESULT InitShader();
	HRESULT InitShader3D(); //simple3D.hlsl�̂��߂̏�����
	HRESULT InitShader2D(); //simple2D.hlsl�̂��߂̏�����
	HRESULT InitShaderCRT();
	HRESULT InitPLShader();

	void SetShader(SHADER_TYPE type);

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();

	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;

	void SetGlobalLightVec(XMFLOAT4 lv);
	XMFLOAT4 GetGlobalLightVec();
};