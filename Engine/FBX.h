#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include <vector>

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

using std::vector;

class Texture;

class FBX
{

	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
		XMFLOAT2 factor;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX    matW;   //���[���h�ϊ��}�g���N�X
		XMMATRIX	matWVP; //�X�N���[���ϊ��}�g���N�X
		XMMATRIX	matNormal;   //�@�����[���h�ϊ��p�}�g���N�X
		XMFLOAT4    diffuseColor;//RGB�̊g�U���ˌW��(�F)
		XMFLOAT4    lightPosition;//�����ʒu
		XMFLOAT2    diffuseFactor;//�g�U���̔��ˌW��
		int         isTextured;
	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;//�m�[�}���ǉ�(�@���x�N�g��)
	};

	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�

	ID3D11Buffer* pVertexBuffer_;//���_�o�b�t�@�p������
	ID3D11Buffer** pIndexBuffer_; //�C���f�b�N�X�o�b�t�@�p������
	ID3D11Buffer* pConstantBuffer_;//�R���X�^���g�o�b�t�@�悤������
	vector<MATERIAL> pMaterialList_;//�}�e���A���̔z��
	vector<int> indexCount_;

	void InitVertex(fbxsdk::FbxMesh*mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);

public:
	FBX();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();
};

