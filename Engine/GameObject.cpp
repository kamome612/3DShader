#include "GameObject.h"
#include "Direct3D.h"
#include "SphereCollider.h"

GameObject::GameObject()
	:pParent_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	:pParent_(parent),objectName_(name),isDead_(false),pCollider_(nullptr)
{
	if (parent != nullptr) {
		this->transform_.pParent_ = &(parent->transform_);
	}
}

GameObject::~GameObject()
{
}

void GameObject::UpdateSub()
{
	Update();
	RoundRobin(GetRootJob());

	for (auto itr : childList_) {
		itr->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();) {
		if ((*itr)->isDead_) {
			(*itr)->ReleaseSub();
			//(*itr)->Release();
			SAFE_DELETE(*itr);
			itr = childList_.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr : childList_) {
		itr->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	Release();
	for (auto itr : childList_) {
		itr->ReleaseSub();
	}
}

void GameObject::KillMe()
{
	isDead_ = true;
}

GameObject* GameObject::FindObject(string objName)
{
	GameObject* rootjob = GetRootJob();
	GameObject* result = rootjob->FindChildObject(objName);
	return result;
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr) {
		return this;
	}
	return pParent_->GetRootJob();
}

GameObject* GameObject::FindChildObject(string objName)
{
	if (this->objectName_ == objName) {
		return this;
	}
	else {
		for (auto itr : childList_) {
			GameObject* obj = itr->FindChildObject(objName);
			if (obj != nullptr) { 
				return obj; 
			}
		}
	}
	return nullptr;
}

void GameObject::AddCollider(SphereCollider* pColl)
{
	pCollider_ = pColl;
}

void GameObject::Collision(GameObject* pTarget)
{
	if (this->pCollider_ == nullptr || pTarget->pCollider_ == nullptr || this == pTarget) {
		return;
	}
	//自分とターゲットのコライダー同士の当たり判定を書く！
	//this->positionとpTarget->positionの距離 <= this->pCollider.radius_ + pTarget->pCollider.radius_
	XMVECTOR P1 = XMLoadFloat3(&(transform_.position_));
	XMVECTOR P2 = XMLoadFloat3(&(pTarget->transform_.position_));

	float dist = XMVectorGetX(XMVector3Length(P1 - P2)); //thisとTargetの距離
	float rdist = this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius(); //thisとTargetのコライダーの半径の和
	if(dist <= rdist)//２点間の距離 <= 半径の足し算
	{
		//onCollision()を呼ぶ=当たってる時の処理
		OnCollision(pTarget);
	}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	//自分とターゲットの当たり判定
	if (this->pCollider_ == nullptr && pTarget->pCollider_ == nullptr) {
		return;
	}
	Collision(pTarget);
	//自分とターゲットの子オブジェクト全部の当たり判定(再帰)
	for (auto& itr : pTarget->childList_) {
		RoundRobin(itr);
	}
}
