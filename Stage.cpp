#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/Quad.h"
#include "Engine/Model.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hModel_(-1)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hModel_ = Model::Load("Assets/ball.fbx");
	assert(hModel_ >= 0);
}

void Stage::Update()
{
	transform_.rotate_.y += 1;
}

void Stage::Draw()
{
	Model::SetTransform(hModel_,transform_);
	Model::Draw(hModel_);
}

void Stage::Release()
{
}
