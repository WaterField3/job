#include "Animation.h"

void Animation::Update()
{
    if (!isPlaying)
    {
        return;
    }

    // テーブルアニメーション…決められた順番で再生させるためのデータ
    // ※テーブル…何かの目的のために作られたデータの集まり
    int animTable[][32] =
    {
        // エフェクト用０～１１まで再生
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, -2 },
    };

    // 表示させるコマIDを取得
    int animID = animTable[animPattern][(int)animCounter];

    // 表示させるコマのUVを計算
    uv.x = animID % 3 * 0.33f;
    uv.y = animID / 3 * 0.25f;

    // アニメーションのカウンターを進める
    animCounter += animSpeed;

    // 最初に戻る処理
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
