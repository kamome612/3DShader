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
    
    //�摜�̈ꕔ�̐F��ς���
    //if (inData.uv.y > 0.5)
    //{
    //    output = float4(1, 0, 0, 0);
    //}
    //else
    //{
    //    output = g_texture.Sample(g_sampler, inData.uv);
    //}
    
    //�h�C�c�̍����\���p
    //if (inData.uv.y < 1 / 3.0f)
    //{
    //    output = float4(0, 0, 0, 0);
    //}
    //else if (inData.uv.y < 2 / 3.0f)
    //{
    //    output = float4(211 / 255.0f, 0, 0, 0);
    //}
    //else
    //{
    //    output = float4(1, 206 / 255.0f, 0, 0);
    //}
    
    float2 size;
    g_texture.GetDimensions(size.x, size.y);
    if ((int) (inData.uv.y * 600) % 2 == 1)
    {
        if ((int) (inData.uv.x * 800) % 2 == 1)
        {
            output = g_texture.Sample(g_sampler, inData.uv);
            output = float4((output.x + output.y + output.z) / 3,
                            (output.x + output.y + output.z) / 3,
                            (output.x + output.y + output.z) / 3, 0);
        }
        else
        {
            output = float4(0, 0, 0, 0);
        }
    }
    else
    {
        output = float4(0, 0, 0, 0);
    }
    
    //output = g_texture.Sample(g_sampler, inData.uv);
    return output;

}