//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    //�ϊ��s��A���_�A����
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matNormal; //�@�������[���h���W�ɑΉ�������s�񁁉�]���X�P�[��
    float4 diffuseColor; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    float4 lightPosition;
    float2 factor; //�f�B�t���[�Y�t�@�N�^�[(diffuseFactor)
    bool isTextured; // �e�N�X�`���\���Ă��邩�ǂ���
    
    //float4 Attenuation;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 color : COLOR; //�F�i���邳�j
    
    //float4 norw : NORMALO;
    //float4 cos_alpha : COLOR; //�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;

    normal = mul(normal, matNormal);
    normal.w = 0;
    normal = normalize(normal);
    
    //outData.norw = mul(normal, matWVP);
    //light��_�����̕����x�N�g���ɒu��������
    float4 lightvec = lightPosition - outData.pos;
    float4 light = normalize(lightvec); //�P�ʃx�N�g����
    outData.color = clamp(dot(normal, light), 0, 1);
	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 ambentSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse;
    float4 ambient;
    if (isTextured == false)
    {
        diffuse = lightSource * diffuseColor * inData.color * factor.x;
        ambient = lightSource * diffuseColor * ambentSource * factor.x;
    }
    else
    {
        diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color * factor.x;
        ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambentSource * factor.x;
    }
    return diffuse + ambient;
    
    //float3 dir;
    //float len;
    //float colD;
    //float colA;
    //float col;
    
    ////�_�����̕���
    //dir = lightVec;
    
    ////�_�����̋���
    //len = length(dir);
    
    ////�_�����̕�����normalize
    //dir = dir / len;
    
    ////�g�U
    //colD = saturate(dot(normalize(inData.norw.xyz), dir));
    ////����
    //colA = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));
    
    //col = colD * colA;
    //return float4(col, col, col, 1.0f);
}
