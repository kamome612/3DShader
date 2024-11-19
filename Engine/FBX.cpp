#include "FBX.h"
#include "Camera.h"
#include <filesystem>
#include "Direct3D.h"
#include "Texture.h"

namespace fs = std::filesystem;

FBX::FBX()
	:vertexCount_(0),polygonCount_(0),materialCount_(0),
	pVertexBuffer_(nullptr),pIndexBuffer_(nullptr),pConstantBuffer_(nullptr)
{
}

//���[�h���Ă��낢�돉����
HRESULT FBX::Load(std::string fileName)
{
	//�}�l�[�W���𐶐�
	FbxManager* pFbxManager = FbxManager::Create();

	//�C���|�[�^�[�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//�V�[���I�u�W�F�N�g��FBX�t�@�C���̏��𗬂�����
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//���b�V�������擾
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	//�e���̌����擾
	vertexCount_ = mesh->GetControlPointsCount();	//���_�̐�
	polygonCount_ = mesh->GetPolygonCount();	//�|���S���̐�
	materialCount_ = pNode->GetMaterialCount();

	//���݂̃J�����g�f�B���N�g�����o���Ă���
	fs::path cPath, basePath;
	cPath = fs::current_path();
	basePath = cPath;

	//������fileName����f�B���N�g���������擾
	//char dir[MAX_PATH];
	string subDir("Assets");
	fs::path subPath(cPath.string() +"\\" +  subDir);
	assert(fs::exists(subPath));
	//�J�����g�f�B���N�g���̕ύX
	fs::current_path(subPath);

	InitVertex(mesh);     //���_�o�b�t�@����
	InitIndex(mesh);      //�C���f�b�N�X�o�b�t�@����
	InitConstantBuffer(); //�R���X�^���g�o�b�t�@����
	InitMaterial(pNode);

	//�J�����g�f�B���N�g�������Ƃɖ߂�
	fs::current_path(basePath);

	//�}�l�[�W�����
	pFbxManager->Destroy();
	return S_OK;
}

void FBX::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//���_��������z��
	//VERTEX* vertices = new VERTEX[vertexCount_];
	std::vector<VERTEX> vertices(vertexCount_);
	//�S�|���S��
	for (DWORD poly = 0; poly < (DWORD)polygonCount_; poly++)
	{
		//3���_��
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//���ׂ钸�_�̔ԍ�
			int index = mesh->GetPolygonVertex(poly, vertex);

			//���_�̈ʒu
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);
		
			//���_��UV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices[index].uv = XMVectorSet((float)(uv.mData[0]), (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);
	
			//���_�̖@��
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//���Ԗڂ̃|���S���́A���Ԗڂ̒��_�̖@�����Q�b�g
			vertices[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}

	// ���_�o�b�t�@�쐬
	HRESULT hr;
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices.data();
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���_�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
	}
}

void FBX::InitIndex(fbxsdk::FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer* [materialCount_];
	//indexCount_ = std::vector<int>(materialCount_);
	//int* index = new int[polygonCount_ * 3];
	indexCount_ = std::vector<int>(materialCount_);
	std::vector<int> index(polygonCount_ * 3);

	for (int i = 0; i < materialCount_; i++) {
		int count = 0;
		//�S�|���S��
		for (DWORD poly = 0; poly < (DWORD)polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i) {
				//3���_��
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					//���_�̈ʒu
					index[count] = mesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}
		}
		indexCount_[i] = count;

		HRESULT hr;
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index.data();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
		}
	}
}

void FBX::InitConstantBuffer()
{
	//�R���X�^���g�o�b�t�@�쐬
	HRESULT hr;
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s", NULL, MB_OK);
	}
}

void FBX::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = std::vector<MATERIAL>(materialCount_);
	//pMaterialList_ = new MATERIAL[materialCount_];

	for (int i = 0; i < materialCount_; i++) {
		//i�Ԗڂ̃}�e���A�������擾
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);

		//�e�N�X�`�����
		FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//�e�N�X�`���̐���
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

		//�e�N�X�`������
		if (fileTextureCount > 0) {
			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();
			fs::path texFile(textureFilePath);
			//�����ő��݃`�F�b�N���K�v
			if (fs::is_regular_file(texFile)) {
				pMaterialList_[i].pTexture = new Texture;
				pMaterialList_[i].pTexture->Load(texFile.string());
			}
			else {
				//Error must be handled here
			}

			////�t�@�C����+�g�������ɂ���
			//char name[_MAX_FNAME]; //�t�@�C����
			//char ext[_MAX_EXT];    //�g���q
			//_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
			//wsprintf(name, "%s%s", name, ext);

			//�t�@�C������e�N�X�`���쐬
			/*pMaterialList_[i].pTexture = new Texture;
			HRESULT hr = pMaterialList_[i].pTexture->Load(name);
			assert(hr == S_OK);*/
			FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(i);
			FbxDouble diffuse = pMaterial->DiffuseFactor;
			pMaterialList_[i].factor = XMFLOAT2((float)diffuse, (float)diffuse);
		}
		//�e�N�X�`������
		else {
			pMaterialList_[i].pTexture = nullptr;
			//this part are witten after
			//�}�e���A���̐F �g�U�̎ЂƁA�A���r�G���g�݂̂̃V�F�[�f�B���O���f��
			FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(i);
			FbxDouble3  diffuse = pMaterial->Diffuse;
			pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
			FbxDouble factor = pMaterial->DiffuseFactor;
			pMaterialList_[i].factor = XMFLOAT2((float)factor, (float)factor);
		}
	}
}

void FBX::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_3D);
	transform.Calculation();//�g�����X�t�H�[�����v�Z

	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	for (int i = 0; i < materialCount_; i++) {
		CONSTANT_BUFFER cb;
		cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix()); //view*proj���J��������Ƃ��Ă���
		cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix()); //MATRIX�̊|���Z�̂�����DirectX�ƈႤ�̂œ]�u���Ƃ�i�Ȃ񂻂�j
		cb.diffuseColor = pMaterialList_[i].diffuse;
		cb.diffuseFactor = pMaterialList_[i].factor;
		cb.globalLightVec = Direct3D::GetGlobalLightVec();
		//cb.isTextured = pMaterialList_[i].pTexture != nullptr;
		if (pMaterialList_[i].pTexture == nullptr)
			cb.isTextured = false;
		else
			cb.isTextured = true;

		D3D11_MAPPED_SUBRESOURCE pdata;
		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); // GPU����̃f�[�^�A�N�Z�X���~�߂�
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb)); // �f�[�^��l�𑗂�
		Direct3D::pContext->Unmap(pConstantBuffer_, 0); //�ĊJ

		//���_�o�b�t�@
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//�R���X�^���g�o�b�t�@
		Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_); //���_�V�F�[�_�[�p 
		Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_); //�s�N�Z���V�F�[�_�[�p

		if (!(pMaterialList_[i].pTexture == nullptr)) {
			//�T���v���[�ƃV�F�[�_�[���\�[�X�r���[���V�F�[�_�ɃZ�b�g
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
		}
		//�`��
		/*Direct3D::pContext->DrawIndexed(polygonCount_ * 3, 0, 0);*/
		Direct3D::pContext->DrawIndexed(indexCount_[i], 0, 0);
	}
}

void FBX::Release()
{
}