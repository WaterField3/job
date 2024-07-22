#pragma once

#include "direct3d.h"

// カメラの視点を操作する機能を持つクラス
class Camera
{

public:
	// カメラが存在する座標を設定する関数
	void SetEye(DirectX::XMFLOAT3 eyePosition);

	// カメラが狙う座標を設定する関数
	void SetFocus(DirectX::XMFLOAT3 focusPosition);

	// ビュー変換行列を計算する関数
	void Update();

	// 計算済みビュー変換行列を取得する関数
	DirectX::XMMATRIX GetViewMatrix();

	DirectX::XMMATRIX GetProjectionMatrix();

	enum PROJECTION_MODE
	{
		_3D, _2D
	};

	void SetProjectionMode(PROJECTION_MODE mode) { projectionMode = mode; }

private:
	// メンバー変数
	// 計算済みビュー変換行列を保存する変数
	DirectX::XMMATRIX matrixView;

	//DirectX::XMMATRIX matrixProj;

	// Eyeを保存する変数
	DirectX::XMFLOAT3 eye = { 0,0,-1 };
	// Focusを保存する変数
	DirectX::XMFLOAT3 focus = { 0,0,0 };
	// Upを保存する変数
	DirectX::XMFLOAT3 up = { 0,1,0 };

	PROJECTION_MODE projectionMode = _3D; // デフォルトは3D → 透視投影

};

