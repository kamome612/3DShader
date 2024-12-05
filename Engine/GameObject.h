#pragma once

#include <string>
#include <list>
#include "Transform.h"
#include "Direct3D.h"

using std::string;
using std::list;

class SphereCollider;


class GameObject
{
	bool isDead_;//生き死に
protected:
	std::list<GameObject*> childList_;
	Transform	           transform_;
	GameObject* pParent_;
	std::string	           objectName_;//オブジェクトの名前
	SphereCollider* pCollider_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();

	void KillMe();

	//inline定義
	void SetPosition(XMFLOAT3 position)
	{
		transform_.position_ = position;
	}

	//インライン定義
	void SetPosition(float x, float y, float z)
	{
		SetPosition(XMFLOAT3(x, y, z));
	}

	void SetScale(float sx, float sy, float sz) { transform_.scale_ = { sx,sy,sz }; }
	void SetRotateX(float xrotate) { transform_.rotate_.x = xrotate; }
	void SetRotateY(float yrotate) { transform_.rotate_.y = yrotate; }
	void SetRotateZ(float zrotate) { transform_.rotate_.z = zrotate; }

	GameObject* FindObject(string objName);
	GameObject* GetRootJob();
	GameObject* FindChildObject(string objName);
	void AddCollider(SphereCollider* pColl);
	void Collision(GameObject* pTarget);
	void RoundRobin(GameObject* pTarget);
	virtual void OnCollision(GameObject* pTarget) {};

	//template <typename T> と同じだよ
	template <class T>
	T* Instantiate(GameObject* pParent)
	{
		T* pTmp = new T(pParent);
		if (pTmp != nullptr) {
			pTmp->Initialize();
			childList_.push_back(pTmp);
		}
		return pTmp;
	}
};

