#include "nglCommon.h"
#include "nglSceneNode.h"
#include "nglTexture.h"
#include "nglSceneObserver.h"

#ifndef nglLight_h__
#define nglLight_h__



struct nglLightProperties {

	//shadow casting properties
	bool cast_shadows;
	float shadow_cast_range; //in world coordinates
	float shadow_alpha; //0 : shadow transparent; 1 : shadow black

	float power; //light power; 0 - none, 1- max

	nglRGB3F color;


};

class nglLight : public nglSceneNode, public nglSceneObserver {

public:
	nglLight();
	virtual ~nglLight();

	nglRGB3F GetColor();
	void SetColor(nglRGB3F color);

	void SetCashShadows(bool cast);
	bool GetCastShadows();

	void SetShadowCastRange(float range);
	float GetShadowCastRange();

	void SetShadowAlpha(float alpha);
	float GetShadowAlpha();

	void IncreaseLightPower(float delta);
	void DecreaseLightPower(float delta);

	float GetLightPower();

	nglTexture* GetDepthMap();

	virtual nglMat4 GetViewMatrix();

	virtual nglMat4 GetProjectionMatrix();

	virtual nglMat4 GetProjectionViewMatrix();

	virtual void LookAt(nglVec3 targetCoords);


private:
	nglLightProperties _light_properties;
	//depth map which this light causes towards scene
	//saved in a texture, because we can later sample this in a shader
	nglTexture* _shadow_depth_map;

	nglMat4 _projection;
	nglMat4 _view;
	nglMat4 _projection_view;



};


#endif // nglLight_h__