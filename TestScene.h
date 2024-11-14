#pragma once
#include "Engine/GameObject.h"
#include "Engine/Sprite.h"
#include "Engine/Image.h"

class TestScene :
    public GameObject
{
public:
	//コンストラクタ
	//引数 : parent  親オブジェクト(SceneManager)
	TestScene(GameObject* parent);

	//初期化
	void Initialize() override;
	
	//更新
	void Update() override;
	
	//描画
	void Draw() override;
	
	//開放
	void Release() override;
	
private:
	int hPict_;
};

