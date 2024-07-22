#include "Animation.h"

void Animation::Update()
{
    if (!isPlaying)
    {
        return;
    }

    // �e�[�u���A�j���[�V�����c���߂�ꂽ���ԂōĐ������邽�߂̃f�[�^
    // ���e�[�u���c�����̖ړI�̂��߂ɍ��ꂽ�f�[�^�̏W�܂�
    int animTable[][32] =
    {
        // �G�t�F�N�g�p�O�`�P�P�܂ōĐ�
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, -2 },
    };

    // �\��������R�}ID���擾
    int animID = animTable[animPattern][(int)animCounter];

    // �\��������R�}��UV���v�Z
    uv.x = animID % 3 * 0.33f;
    uv.y = animID / 3 * 0.25f;

    // �A�j���[�V�����̃J�E���^�[��i�߂�
    animCounter += animSpeed;

    // �ŏ��ɖ߂鏈��
    if (animTable[animPattern][(int)animCounter] == -1)
    {
        animCounter = 0.0f;
    }
    else if (animTable[animPattern][(int)animCounter] == -2)
    {
        animCounter = 0.0f;
        //animSpeed = 0.0f;
        isPlaying = false;
    }
}

FLOAT_XY Animation::GetUV()
{
    return uv;
}

void Animation::SetPattern(int pattern)
{
    animPattern = pattern;
}
