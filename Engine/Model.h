#pragma once
#include <string>
#include <vector>
#include "FBX.h"

namespace Model
{
	struct ModelData
	{
		//FBX
		FBX* pFbx_;
		//Transform
		Transform transform_;
		//FileName
		std::string filename_;
	};

	//モデルの一覧を保存する配列(Vector or List)
	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	void Release();
}
