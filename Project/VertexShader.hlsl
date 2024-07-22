// ���_�V�F�[�_�[

// ���_�̃f�[�^��\���\���́i�󂯎��p�j
struct VS_IN
{
    float4 pos : POSITION; // �ʒu���W������
    float2 tex : TEX;       // UV���W������
    float4 normal : NORMAL; // ���_�̖@���x�N�g��������
};

// ���_�̃f�[�^��\���\���́i���M�p�j 
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
    float4 normal : NORMAL; // ���_�̖@���x�N�g��������
};

// �O���[�o���ϐ��̐錾
// �萔�o�b�t�@�󂯎��p
cbuffer ConstBuffer : register(b0)
{
   // ���[���h�ϊ��s��~�r���[�ϊ��s��~���e�s��
    matrix matrixWorld;
    // �@���x�N�g������]������s��
    matrix matrixWorldNormal;
    // UV�A�j���[�V�����p�s��
    matrix matrixUV;
    // �}�e���A���F
    float4 materialDiffuse;
}
 

// ���_�V�F�[�_�[�̃G���g���|�C���g 
VS_OUT vs_main( VS_IN input )
{
    VS_OUT output;
 
    // ���[���h�ϊ��s��𒸓_�Ɋ|����
    output.pos = mul(input.pos, matrixWorld);
        
    // UV�A�j���[�V����
    float4 tex4;
    tex4.xy = input.tex;
    tex4.z = 0;
    tex4.w = 1;
    output.tex = mul(tex4, matrixUV).xy;
    
    // ���_�̖@���x�N�g���ɉ�]�s���K�p����
    output.normal = mul(input.normal, matrixWorldNormal);
        
    return output;
}