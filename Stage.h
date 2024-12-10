#pragma once
#include "Engine/GameObject.h"

struct CONSTBUFFER_STAGE
{
	XMFLOAT4 lightPosition; //光源位置
	XMFLOAT4 eyePosition;   //視点の位置
};

class Stage :
    public GameObject
{
private:
	int hModel_;
	int hRoom_;
	int hGround_;
	ID3D11Buffer* pConstantBuffer_;
	void InitConstantBuffer();

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
};

