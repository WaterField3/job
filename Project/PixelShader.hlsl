// �s�N�Z���V�F�[�_�[

// �s�N�Z���̏��̍\���́i�󂯎��p�j
struct PS_IN
{
    // float4�^�@���@float�^���S�̍\����
    float4 pos : SV_POSITION; // �s�N�Z���̉�ʏ�̍��W
    float2 tex : TEXCOORD;  // �s�N�Z���ɑΉ�����e�N�X�`�����W
    float4 normal : NORMAL; // �s�N�Z���ʂ̖@���x�N�g��
};

// �O���[�o���ϐ��̐錾
// ���V�F�[�_�[�̃O���[�o���ϐ��́AC����v���O��������n���ꂽ
// �@�f�[�^���󂯎�邽�߂Ɏg���B
Texture2D myTexture : register(t0); //�e�N�X�`���[
SamplerState mySampler : register(s0); //�T���v���[

// �萔�o�b�t�@�󂯎��p
cbuffer ConstBuffer : register(b0)
{
    // ���[���h�ϊ��s��
    matrix matrixWorld;
    // �@���x�N�g������]������s��
    matrix matrixWorldNormal;
    // UV�A�j���[�V�����p�s��
    matrix matrixUV;
    // �}�e���A���F
    float4 materialDiffuse;
    
    unsigned int isLighting;
    int dummy2, dummy3, dummy4;
}
 

// �s�N�Z���V�F�[�_�[�̃G���g���|�C���g
float4 ps_main(PS_IN input) : SV_Target
{
    if(!isLighting)
    {
        // Sample�֐����e�N�X�`������w�肵��UV�����Ƀs�N�Z���F������ė���
        float4 color = myTexture.Sample(mySampler, input.tex);    
        // �e�N�X�`���F�ƃ}�e���A���F��������
        return color * materialDiffuse;
    }
    
    // ���̌�����\���x�N�g��
    float3 lightVector = float3(1, -1, 1);
    // �s�N�Z���ʂ̖@���x�N�g��
    float3 pixelNormal = input.normal.xyz;
    
    // �����i�A���r�G���g�j
    float4 ambientLight = float4(0.1f, 0.1f, 0.1f, 0.0f);
    
    // �x�N�g���𐳋K�����遁�P�ʃx�N�g���ɂ���
    lightVector = normalize(lightVector);
    pixelNormal = normalize(pixelNormal);
    
    // ���x�N�g���Ɩ@���x�N�g������ς���
    float diffusePower = dot(lightVector, pixelNormal);
    
    // diffusePower��0.0f�`1.0f�͈̔͂Ɋۂ߂�
    diffusePower = saturate(diffusePower);
    
    // ���̓����鋭�������ȉ��ɂȂ�Ȃ��悤�ɂ���
    diffusePower = max(diffusePower, 0.2f);
    
    // �e�N�X�`���F���擾����
    float4 textureColor = myTexture.Sample(mySampler, input.tex);
    
    // ���̓����鋭���ƃe�N�X�`���F���|����
    float4 pixelColor = textureColor * diffusePower;
    // �ω����Ă��܂����A���t�@�l��߂�
    pixelColor.a = textureColor.a;
    
    // ������K�p����
    pixelColor = pixelColor + ambientLight;
    
    return pixelColor;
}