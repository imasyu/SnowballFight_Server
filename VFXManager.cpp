#include "VFXManager.h"
#include "Engine/VFX.h"

namespace VFXManager {
	EmitterData hit;
	EmitterData snow;

	void Initialize()
	{
		//ê·ã 
		snow.textureFileName = "circle_Y.png";
		snow.position = XMFLOAT3();
		snow.delay = 0;
		snow.number = 10;
		snow.lifeTime = 60;
		snow.positionRnd = XMFLOAT3(0.5, 0, 0.5);
		snow.direction = XMFLOAT3(0, 0, 1);
		snow.directionRnd = XMFLOAT3(90, 90, 90);
		snow.speed = 0.3f;
		snow.speedRnd = 0;
		snow.accel = 0.93;
		snow.size = XMFLOAT2(0.1, 0.1);
		snow.sizeRnd = XMFLOAT2(0.4, 0.4);
		snow.color = XMFLOAT4(0, 0, 1, 1);
		snow.deltaColor = XMFLOAT4(0, 1, 1, 0);

		//ìñÇΩÇ¡ÇΩéûÇÃVFX
		hit.textureFileName = "defaultParticle.png";
		hit.position = XMFLOAT3();
		hit.delay = 0;
		hit.number = 40;
		hit.lifeTime = 30;
		hit.positionRnd = XMFLOAT3(0.5, 0, 0.5);
		hit.direction = XMFLOAT3(0, 1, 0);
		hit.directionRnd = XMFLOAT3(90, 90, 90);
		hit.speed = 0.25f;
		hit.speedRnd = 1;
		hit.accel = 0.93;
		hit.size = XMFLOAT2(0.4, 0.4);
		hit.sizeRnd = XMFLOAT2(0.4, 0.4);
		hit.scale = XMFLOAT2(0.99, 0.99);
		hit.color = XMFLOAT4(1, 1, 1, 1);
		hit.deltaColor = XMFLOAT4(0, 0, 0, 0);
		hit.gravity = 0.003f;
	}

}

void VFXManager::CreateHitParticle(XMFLOAT3 pos)
{
	hit.position = pos;
	VFX::Start(hit);
}

void VFXManager::CreateSnowBallParticle(XMFLOAT3 pos)
{
	snow.position = pos;
	VFX::Start(snow);
}
