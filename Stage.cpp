#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
	hModel_[0] = -1;
	hModel_[1] = -1;
	hModel_[2] = -1;
	//Model_ = -1;
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	/*hModel_[0] = Model::Load("Assets\\ball.fbx");
	assert(hModel_[0] >= 0);
	hModel_[1] = Model::Load("Asstes\\balldark.fbx");
	assert(hModel_[1] >= 0);
	hModel_[2] = Model::Load("Assets\\balllight.fbx");
	assert(hModel_[2] >= 0);*/
	Direct3D::SetGlobalLightVec(lv);

	//Šm‚©‚ß‚é‚æ‚¤
	Model_ = Model::Load("Assets/balllight.fbx");
	assert(Model_ >= 0);
}

void Stage::Update()
{
	transform_.rotate_.y += 0.5;
	if (Input::IsKey(DIK_X)) {
		lv.x = lv.x + 0.1;
	}
	if (Input::IsKey(DIK_Z)) {
		lv.y = lv.y + 0.1;
	}
	Direct3D::SetGlobalLightVec(lv);
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

	//Šm‚©‚ß‚é‚æ‚¤
	Model::SetTransform(Model_, transform_);
	Model::Draw(Model_);
}

void Stage::Release()
{
}
