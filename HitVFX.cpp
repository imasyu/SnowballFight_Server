#include "HitVFX.h"

void HitVFX::CreatePosition(float x, float y, float z)
{
	//位置を設定
	data.position = { x,y,z };

	//VFXを生成
	VFX::Start(data);

}
