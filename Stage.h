#pragma once
#include "Engine/GameObject.h"
#include "Engine/Quad.h"

class Stage :
    public GameObject
{
public:

	//コンストラクタ
	Stage(GameObject* parent);

	//デストラクタ
	~Stage();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

private:
	//int hModel_;
	int hModel_[3];
	int Model_;
	XMFLOAT4 lv;
};

