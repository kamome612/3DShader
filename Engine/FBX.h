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

	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
		XMFLOAT2 factor;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX    matW;   //ワールド変換マトリクス
		XMMATRIX	matWVP; //スクリーン変換マトリクス
		XMMATRIX	matNormal;   //法線ワールド変換用マトリクス
		XMFLOAT4    diffuseColor;//RGBの拡散反射係数(色)
		XMFLOAT4    lightPosition;//光源位置
		XMFLOAT2    diffuseFactor;//拡散光の反射係数
		int         isTextured;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;//ノーマル追加(法線ベクトル)
	};

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数

	ID3D11Buffer* pVertexBuffer_;//頂点バッファ用メモリ
	ID3D11Buffer** pIndexBuffer_; //インデックスバッファ用メモリ
	ID3D11Buffer* pConstantBuffer_;//コンスタントバッファようメモリ
	vector<MATERIAL> pMaterialList_;//マテリアルの配列
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

