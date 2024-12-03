#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
	hModel_[0] = -1;
	hModel_[1] = -1;
	hModel_[2] = -1;
	Model_ = -1;
	rModel_ = -1;
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
	rModel_ = Model::Load("Assets/roadb.fbx");
	assert(rModel_ >= 0);
	trs = transform_;
}

void Stage::Update()
{
	transform_.rotate_.y += 0.5f;
	if (Input::IsKey(DIK_A)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p = { p.x - 0.05f,p.y,p.z,p.w };
		Direct3D::SetGlobalLightVec(p);
	}
	if (Input::IsKey(DIK_D)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p = { p.x + 0.05f,p.y,p.z,p.w };
		Direct3D::SetGlobalLightVec(p);
	}
	if (Input::IsKey(DIK_S)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p = { p.x,p.y - 0.05f,p.z,p.w };
		Direct3D::SetGlobalLightVec(p);
	}
	if (Input::IsKey(DIK_W)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p = { p.x,p.y + 0.05f,p.z,p.w };
		Direct3D::SetGlobalLightVec(p);
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
	XMFLOAT4 p = Direct3D::GetGlobalLightVec();
	transform_.position_ = { p.x,p.y,p.z };
	//Šm‚©‚ß‚é‚æ‚¤
	Model::SetTransform(rModel_, trs);
	Model::Draw(rModel_);
	Model::SetTransform(Model_, transform_);
	Model::Draw(Model_);
}

void Stage::Release()
{
}
