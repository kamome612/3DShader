#pragma once
#include "Engine/GameObject.h"
#include "Engine/Direct3D.h"

class Stage :
    public GameObject
{
public:

	//�R���X�g���N�^
	Stage(GameObject* parent);

	//�f�X�g���N�^
	~Stage();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

private:
	//int hModel_;
	int hModel_[3];
	int Model_;
	int rModel_;
	XMFLOAT4 lv;
	Transform trs;
};

