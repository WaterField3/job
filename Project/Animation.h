#pragma once

#include "direct3d.h"

// アニメーション機能のクラス
class Animation
{

public:
	// 列挙型
	// アニメーションパターン名
	enum PATTERN
	{
		NO_ANIMATION,
		FX_0_11 = 5  // エフェクト用０～１１再生
	};

	// メンバー変数
	DirectX::XMFLOAT2 uv; // アニメーションのUV移動量を保存

	float animCounter = 0.0f; // アニメテーブルのカウンター

	// 再生するパターンIDを持つ変数
	int animPattern = 1;

	

public:
	// 再生速度を設定する
	float animSpeed = 0.1f; // 再生速度

	bool isPlaying = true;

public:
	// メンバー関数
	void Update();

	// 再生するアニメーションを変更する

	// UV移動量を取得する
	DirectX::XMFLOAT2 GetUV();

	// 再生するパターンを変える
	void SetPattern(int pattern);

	bool IsPlaying() { return isPlaying; }

};

