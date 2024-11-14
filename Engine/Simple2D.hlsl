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
    float4x4 matW; //���[���h�s��
    float4x4 matN;
    int2     winSize;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matW);
    outData.uv = uv;

    //float4 light = float4(-1, 0.5, -0.7, 0); //�����x�N�g���̋t�x�N�g��
 
	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 output;
    //float2 nowUV = inData.uv;
    //float neiUV = float2(inData.uv.x + 1.0f / 800, inData.uv.y);
    //float neineiUV = float2(inData.uv.x + 2.0f / 800, inData.uv.y);
    //float4 now, nei, neinei;
    
    float2 n1,n2,n3,n4,n5,n6,n7,n8,n9; //���W
    float4 c1,c2,c3,c4,c5,c6,c7,c8,c9; //��̍��W�̐F
    n1 = float2(inData.uv.x - 1.0f / 800, inData.uv.y - 1.0f / 600);
    n2 = float2(inData.uv.x, inData.uv.y - 1.0f / 600);
    n3 = float2(inData.uv.x + 1.0f / 800, inData.uv.y - 1.0f / 600);
    n4 = float2(inData.uv.x - 1.0f / 800, inData.uv.y);
    n5 = inData.uv;
    n6 = float2(inData.uv.x + 1.0f / 800, inData.uv.y);
    n7 = float2(inData.uv.x - 1.0f / 800, inData.uv.y + 1.0f / 600);
    n8 = float2(inData.uv.x, inData.uv.y + 1.0f / 600);
    n9 = float2(inData.uv.x + 1.0f / 800, inData.uv.y + 1.0f / 600);
    
    c1 = g_texture.Sample(g_sampler, n1);
    c2 = g_texture.Sample(g_sampler, n2);
    c3 = g_texture.Sample(g_sampler, n3);
    c4 = g_texture.Sample(g_sampler, n4);
    c5 = g_texture.Sample(g_sampler, n5);
    c6 = g_texture.Sample(g_sampler, n6);
    c7 = g_texture.Sample(g_sampler, n7);
    c8 = g_texture.Sample(g_sampler, n8);
    c9 = g_texture.Sample(g_sampler, n9);
    
    return ((c1+c2+c3+c4+c5+c6+c7+c8+c9) / 9.0);
    
    //now = g_texture.Sample(g_sampler, inData.uv);
    //nei = g_texture.Sample(g_sampler, neiUV);
    //neinei = g_texture.Sample(g_sampler, neineiUV);
    //return ((now + nei + neinei) / 3.0);
    //output = g_texture.Sample(g_sampler, inData.uv);
    //return output;

}