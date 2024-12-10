#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

void Stage::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTBUFFER_STAGE);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"コンスタントバッファの作成に失敗", NULL, MB_OK);
	}
}

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"),pConstantBuffer_(nullptr)
{
	hModel_ = -1;
	hGround_ = -1;
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	/*hModel_[0] = Model::Load("Assets/ball.fbx");
	assert(hModel_[0] >= 0);
	hModel_[1] = Model::Load("Asstes/balldark.fbx");
	assert(hModel_[1] >= 0);
	hModel_[2] = Model::Load("Assets/balllight.fbx");
	assert(hModel_[2] >= 0);*/
	//確かめるよう
	hModel_ = Model::Load("Assets\\ball.fbx");
	assert(hModel_ >= 0);
	hGround_ = Model::Load("Assets\\roadb.fbx");
	assert(hGround_ >= 0);
	hRoom_ = Model::Load("Assets\\room.fbx");
	assert(hRoom_ >= 0);
	Camera::SetPosition(XMFLOAT3{ 0, 0.8, -2.8});
	Camera::SetTarget(XMFLOAT3{ 0,0.8,0 });

	/*Camera::SetPosition(XMFLOAT3{ 0, 5, -5 });
	Camera::SetTarget(XMFLOAT3{ 0,1,0 });*/

	InitConstantBuffer();
}

void Stage::Update()
{
	transform_.rotate_.y += 0.5f;
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
	}

	//コンスタントバッファのシェーへのコンスタントバッファのセットを書くよ
	CONSTBUFFER_STAGE cb;
	cb.lightPosition = Direct3D::GetLightPos();
	XMStoreFloat4(&cb.eyePosition,Camera::GetPosition());

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); // GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb)); // データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_, 0); //再開

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(1, 1, &pConstantBuffer_); //頂点シェーダー用 
	Direct3D::pContext->PSSetConstantBuffers(1, 1, &pConstantBuffer_); //ピクセルシェーダー用
	
}

void Stage::Draw()
{
	Transform ltr;
	ltr.position_ = { Direct3D::GetLightPos().x,Direct3D::GetLightPos().y,
					 Direct3D::GetLightPos().z };
	ltr.scale_ = { 0.5,0.5,0.5 };
	Model::SetTransform(hModel_, ltr);
	Model::Draw(hModel_);

	Transform gTrs;
	gTrs.position_ = { 0,0,0 };
	gTrs.rotate_ = { 0,0,0 };
	Model::SetTransform(hRoom_, gTrs);
	Model::Draw(hRoom_);
	/*Model::SetTransform(hGround_, gTrs);
	Model::Draw(hGround_);*/


	//XMFLOAT4 p = Direct3D::GetLightPos();
	//transform_.position_ = { p.x,p.y,p.z };
	////確かめるよう
	//Transform ltr;
	//ltr.position_ = { 0,0,0 };
	//Model::SetTransform(hGround_, ltr);
	//Model::Draw(hGround_);
	//Model::SetTransform(hModel_, transform_);
	//Model::Draw(hModel_);

	/*Transform ltr;
	ltr.position_ = { Direct3D::GetLightPos().x,Direct3D::GetLightPos().y,Direct3D::GetLightPos().z };
	ltr.scale_ = { 0.1,0.1,0.1 };
	Model::SetTransform(hModel_[0], ltr);
	Model::Draw(hModel_[0]);*/

	//Transform ltr;
	//ltr.position_ = { Direct3D::GetLightPos().x,Direct3D::GetLightPos().y,Direct3D::GetLightPos().z };
	////ltr.scale_ = { 0.1,0.1,0.1 };
	//Model::SetTransform(Model_, ltr);
	//Model::Draw(Model_);

	//Transform tr;
	//tr.position_ = { 0,0,0 };
	//tr.rotate_ = { 0,0,0 };
	//Model::SetTransform(hGround_, tr);
	//Model::Draw(hGround_);
}

void Stage::Release()
{
}
