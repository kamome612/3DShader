#include "Image.h"
#include "Direct3D.h"

namespace Image
{
	std::vector<ImageData*>imageList;
}

int Image::Load(std::string filename)
{
	ImageData* pData = new ImageData;
	pData->filename_ = filename;
	pData->pSprite_ = nullptr;

	for (auto& e : imageList) {
		if (e->filename_ == filename) {//読み込み済み
			pData->pSprite_ = e->pSprite_;
			break;//同名のファイルが読み込み済み
		}
	}
	if (pData->pSprite_ == nullptr) {//同名のファイルはリストにない
		pData->pSprite_ = new Sprite;
		pData->pSprite_->Load(filename);
	}

	imageList.push_back(pData);
	return (imageList.size() - 1);
}

void Image::SetTransform(int hPict, Transform transform)
{
	imageList[hPict]->transform_ = transform;
}

void Image::Draw(int hPict)
{
	imageList[hPict]->pSprite_->Draw(imageList[hPict]->transform_);
}

void Image::Release()
{
	bool isReferred = false;
	for (int j = 0; j < imageList.size(); j++) {
		for (int i = j + 1; i < imageList.size(); i++) {
			if (imageList[j]->pSprite_ == imageList[i]->pSprite_) {
				isReferred = true;
				break;
			}
		}
		if (isReferred == false) {
			SAFE_DELETE(imageList[j]->pSprite_);
		}
		SAFE_DELETE(imageList[j]);
	}
	imageList.clear();
}
