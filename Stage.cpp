#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Image.h"
#include "Engine/Sprite.h"

namespace
{
	XMFLOAT4 lpos_backup[POINT_LIGHT_MAX];
	bool isRotateLight = false;
}

void Stage::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	CONSTBUFFER_STAGE m;
	cb.ByteWidth = sizeof(m) + (sizeof(m) % 16 == 0 ? 0 : 16 - sizeof(m) % 16);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	HRESULT hr;

	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pCBStage_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"コンスタントバッファの作成に失敗", NULL, MB_OK);
	}
}

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"),pCBStage_(nullptr)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	Camera::SetPosition(XMFLOAT3{ 0, 0.8, -2.8});
	Camera::SetTarget(XMFLOAT3{ 0,0.8,0 });

	sptlight_ =
	{
		Direct3D::GetLightPos(),
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0, -1, 0, 0.0 },
		40.0f,
		50.0f,
		0.1f,
		1.0f
	};

	ptlight_[0].lightPosition = { -0.5,  0.2, 0, 1.0 };
	ptlight_[1].lightPosition = { 0.5,  0.2, 0, 1.0 };
	ptlight_[2].lightPosition = { 0.0,  0.2, -0.2, 1.0 };
	ptlight_[3].lightPosition = { 0, 0, 0, 1.0 };
	ptlight_[4].lightPosition = { 0, 0, 0, 1.0 };
	ptlight_[0].pointLightColor = { 1,0, 0, 1.0 };
	ptlight_[1].pointLightColor = { 0, 1, 0, 1.0 };
	ptlight_[2].pointLightColor = { 0, 0, 1, 1.0 };
	ptlight_[3].pointLightColor = { 1, 1, 1, 1.0 };
	ptlight_[4].pointLightColor = { 1, 1, 1, 1.0 };
	ptlight_[0].kTerm = { 0.2f, 0.2f, 1.0f, 1.0f };
	ptlight_[1].kTerm = { 0.2f, 0.2f, 1.0f, 1.0f };
	ptlight_[2].kTerm = { 0.2f, 0.2f, 1.0f, 1.0f };
	ptlight_[3].kTerm = { 0.2f, 0.2f, 1.0f, 1.0f };
	ptlight_[4].kTerm = { 0.2f, 0.2f, 1.0f, 1.0f };
	ptlight_[0].sw = 1;
	ptlight_[1].sw = 1;
	ptlight_[2].sw = 1;
	ptlight_[3].sw = 0;
	ptlight_[4].sw = 0;


	hImage_ = new Sprite("Assets/dice.png");
	hImage_->Initialize();

	for (int i = 0; i < POINT_LIGHT_MAX; i++)
	{
		lpos_backup[i] = ptlight_[i].lightPosition;
	}

	InitConstantBuffer();
}

void Stage::Update()
{
	/*transform_.rotate_.y += 0.5f;
	if (Input::IsKey(DIK_A)) {
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x - 0.05f,p.y,p.z,p.w };
		Direct3D::SetLightPos(p);
	}
	if (Input::IsKey(DIK_D)) {
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x + 0.05f,p.y,p.z,p.w };
		Direct3D::SetLightPos(p);
	}
	if (Input::IsKey(DIK_S)) {
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x,p.y,p.z - 0.05f,p.w };
		Direct3D::SetLightPos(p);
	}
	if (Input::IsKey(DIK_W)) {
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x,p.y,p.z + 0.05f,p.w };
		Direct3D::SetLightPos(p);
	}

	if (Input::IsKey(DIK_UP))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x,p.y + 0.05f, p.z,p.w };
		Direct3D::SetLightPos(p);
	}
	if (Input::IsKey(DIK_DOWN))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x ,p.y - 0.05f, p.z,p.w };
		Direct3D::SetLightPos(p);
	}*/

	//コンスタントバッファのシェーへのコンスタントバッファのセットを書くよ
	CONSTBUFFER_STAGE cb;
	//spotlight
	cb.sptLightPosition = sptlight_.LightPosition;
	XMStoreFloat4(&cb.eyePosition, Camera::GetPosition());
	//cb.pLightPosition = { -1.0, 1.0, 2.0, 1.0 };
	//XMStoreFloat4(&cb.eyePosition, Camera::GetPosition());
	//cb.pLightPosition = sptlight_.pLightPosition;
	cb.sptLightColor = sptlight_.color;
	cb.direction = sptlight_.direction;
	cb.sptLightparam = { sptlight_.theta,
						 sptlight_.phi,
						 sptlight_.att,
						 sptlight_.toff };
	//pointlight
	for (int i = 0; i < POINT_LIGHT_MAX; i++)
	{
		cb.pointlightPosition[i] = ptlight_[i].lightPosition;
		cb.pointLightColor[i] = ptlight_[i].pointLightColor;
		cb.kTerm[i] = ptlight_[i].kTerm;
		cb.pointListSW[i] = { ptlight_[i].sw, 0, 0,0 };
	}

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pCBStage_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	ULONG cbsize = sizeof(cb);

	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), cbsize);	// データを値を送る
	Direct3D::pContext->Unmap(pCBStage_, 0);	//再開

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(1, 1, &pCBStage_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(1, 1, &pCBStage_);	//ピクセルシェーダー用
	
}

void Stage::Draw()
{
	hImage_->Draw(transform_);
}

void Stage::Release()
{
}
