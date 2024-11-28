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
    float4 lightVec; //���s�����̕����x�N�g��
    float2 factor; //�f�B�t���[�Y�t�@�N�^�[(diffuseFactor)
    bool isTextured; // �e�N�X�`���\���Ă��邩�ǂ���
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 color : COLOR; //�F�i���邳�j
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
    
    //light��_�����̕����x�N�g���ɒu��������
    float4 lightvec = lightVec - outData.pos;
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
    //float4 myUv = { 0, 125, 0.25, 0, 0 };
    //float4 Id = { 1.0, 1.0, 1.0, 0.0 };
    //float4 Kd = g_texture.Sample(g_sampler, inData.uv);
    //float cos_alpha = inData.cos_alpha;
    //float4 ambentSource = { 0.5, 0.5, 0.5, 0.0 };//�����̋���
    //if(isTextured == false)
    //{
    //    return Id *  diffuseColor  * cos_alpha + Id * diffuseColor * ambentSource;
    //}
    //else
    //{
    //    return Id * Kd * cos_alpha + Id * Kd * ambentSource;
    //}
    
    float4 ambentSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse;
    float4 ambient;
    if (isTextured == false)
    {
        diffuse = diffuseColor * inData.color * factor.x;
        ambient = diffuseColor * ambentSource * factor.x;
    }
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambentSource * factor.x;
    }
    return diffuse + ambient;
    //return g_texture.Sample(g_sampler, myUv);
}
