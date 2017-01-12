#include "nglLight.h"
#include "gtc\matrix_transform.hpp"




nglLight::nglLight()
{
	this->SetNodeType(NGL_SCENENODE_TYPE_LIGHT);
	_shadow_depth_map = new nglTexture();
	_shadow_depth_map->SetTextureCompareFunc(GL_LEQUAL);
	_shadow_depth_map->SetTextureCompareMode(GL_COMPARE_R_TO_TEXTURE);
	_shadow_depth_map->SetTextureDatatype(GL_FLOAT);
	_shadow_depth_map->SetTextureFormat(GL_DEPTH_COMPONENT);
	_shadow_depth_map->SetTextureInternalFormat(GL_DEPTH_COMPONENT16);
	_shadow_depth_map->SetSize(NGL_RENDER_SHADOWMAP_RES,NGL_RENDER_SHADOWMAP_RES);

	//_projection = glm::ortho<float>(-15.0f,15.0f,-15.0f,15.0f,-10.0f,100.0f);
	_projection = glm::ortho<float>(-10.0f,10.0f,-10.0f,10.0f,-5.0f,99.0f);
	//_projection = glm::perspective<float>(45.0f, 1.0f, 1.0f, 500.0f);
	_view = glm::lookAt(this->GetPosition(), nglVec3(0,0,0), nglVec3(0,1,0));

	_light_properties.color = nglVec3(1.0f,1.0f,1.0f);
	_light_properties.power = 1.0f;
}

nglLight::~nglLight()
{

}

nglRGB3F nglLight::GetColor()
{
	return _light_properties.color;
}

void nglLight::SetColor(nglRGB3F color)
{
	_light_properties.color = color;
}

void nglLight::SetCashShadows(bool cast)
{
	_light_properties.cast_shadows = cast;
}

bool nglLight::GetCastShadows()
{
	return _light_properties.cast_shadows;
}

void nglLight::SetShadowCastRange(float range)
{
	_light_properties.shadow_cast_range = range;
}

float nglLight::GetShadowCastRange()
{
	return _light_properties.shadow_cast_range;
}

void nglLight::SetShadowAlpha(float alpha)
{
	_light_properties.shadow_alpha = alpha;
}

float nglLight::GetShadowAlpha()
{
	return _light_properties.shadow_alpha;
}

nglTexture* nglLight::GetDepthMap()
{
	return _shadow_depth_map;
}

glm::mat4 nglLight::GetViewMatrix()
{
	return _view;
}

glm::mat4 nglLight::GetProjectionMatrix()
{
	return _projection;
}

glm::mat4 nglLight::GetProjectionViewMatrix()
{
	this->_projection_view = this->_projection * this->_view;
	return _projection_view;
}

void nglLight::LookAt(nglVec3 targetCoords)
{
	_view = glm::lookAt(GetPosition(), targetCoords, glm::vec3(0.0f,1.0f,0.0f));
}


void nglLight::IncreaseLightPower(float delta)
{
	_light_properties.power += delta;
	if (_light_properties.power > 1.0f)
		_light_properties.power = 1.0f;
}

void nglLight::DecreaseLightPower(float delta)
{
	_light_properties.power -= delta;
	if (_light_properties.power < 0.0f)
		_light_properties.power = 0.0f;
}

float nglLight::GetLightPower()
{
	return _light_properties.power;
}
