#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
	hModel_[0] = -1;
	hModel_[1] = -1;
	hModel_[2] = -1;
	Model_ = -1;
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
	//Šm‚©‚ß‚é‚æ‚¤
	Model_ = Model::Load("Assets/ball.fbx");
	assert(Model_ >= 0);
	hGround_ = Model::Load("Assets/roadb.fbx");
	assert(hGround_ >= 0);

	/*Camera::SetPosition(XMFLOAT3{ 0, 0.5, -1 });
	Camera::SetTarget(XMFLOAT3{ 0,0,0 });*/
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
}

void Stage::Draw()
{
	/*for (int i = 0; i < 3; i++) {
		Transform trs;
		trs.position_ = { transform_.position_.x + (float)i * 2.0f,
			              transform_.position_.y,transform_.position_.z };
		trs.scale_ = transform_.scale_;
		trs.rotate_ = transform_.rotate_;
		Model::SetTransform(hModel_[i], trs);
		Model::Draw(hModel_[i]);
	}*/
	XMFLOAT4 p = Direct3D::GetLightPos();
	transform_.position_ = { p.x,p.y,p.z };
	//Šm‚©‚ß‚é‚æ‚¤
	Transform ltr;
	ltr.position_ = { 0,0,0 };
	Model::SetTransform(hGround_, ltr);
	Model::Draw(hGround_);
	Model::SetTransform(Model_, transform_);
	Model::Draw(Model_);

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
