#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{
namespace Media3D
{

IMP_DEPENDENCY_PROPERTY(Color*, Light, Color, new Color(1, 1, 1))
IMP_DEPENDENCY_PROPERTY(float, Light, Intensity, 1.0f)
IMP_DEPENDENCY_PROPERTY(float, Light, AmbientIntensity, 0.0f)

DependencyClass* Light::get_Class()
{
	static DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_ColorProperty(),
		get_IntensityProperty(),
		get_AmbientIntensityProperty(),
	};

	return &depclass;
}

IMP_DEPENDENCY_PROPERTY(Vector3_F*, PointLight, Location, new Vector3_F(0, 0, 0))
IMP_DEPENDENCY_PROPERTY(Vector3_F*, PointLight, Attenuation, new Vector3_F(1, 0, 0))

Light::Light(DependencyClass* depClass) : Visual3D(depClass)
{
}

DependencyClass* PointLight::get_Class()
{
	static DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_LocationProperty(),
		get_AttenuationProperty(),
	};

	return &depclass;
}

DependencyClass* PointLight::pClass(get_Class());

PointLight::PointLight() : Light(get_Class())
{
}

void PointLight::SetLight(ImmediateRenderContext* renderContext)
{
	D3DXMATRIX modelView = renderContext->modelViewMatrix();

	D3DXVECTOR3 location(get_Location()->get_XYZ());
	D3DXVECTOR4 v;

	D3DXVec3Transform(&v, &location, &modelView);

	float ambientIntensity = get_AmbientIntensity();
	float intensity = get_Intensity();
	Vec3f color = get_Color()->getVec3f();
	Vec3f attenuation = get_Attenuation()->get_XYZ();

	Graphics::Light light;
	light.m_type = 2;
	light.m_ambient = Vec4f(ambientIntensity, ambientIntensity, ambientIntensity, 1.0f);
	light.m_diffuse = Vec4f(color[0]*intensity, color[1]*intensity, color[2]*intensity, 1.0f);
	light.m_position = Vec4f(v.x, v.y, v.z, 1/*positional*/);
	light.m_constant_attenuation = attenuation[0];
	light.m_linear_attenuation = attenuation[1];
	light.m_quadratic_attenuation = attenuation[2];

	renderContext->m_lights.push_back(light);
}

}	// Media3D
}	// Gui
}	// System
