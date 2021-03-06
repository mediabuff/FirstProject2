#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{
namespace Media3D
{

struct X3DMaterial
{
	float4 diffuseColor;
	float4 ambientColor;
	float4 specularColor;
	float4 emissiveColor;
	float power;
};

IMP_DEPENDENCY_PROPERTY1(Color*, Material, AmbientColor, new Color(0.0f, 0.0f, 0.0f), new PropertyMetadata(true));
IMP_DEPENDENCY_PROPERTY1(Color*, Material, DiffuseColor, new Color(0.8f, 0.8f, 0.8f), new PropertyMetadata(true));
IMP_DEPENDENCY_PROPERTY1(Color*, Material, SpecularColor, nullptr, new PropertyMetadata(true));

DependencyClass* Material::get_Class()
{
	static DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_DiffuseColorProperty(),
		get_AmbientColorProperty(),
		get_SpecularColorProperty(),
	};

	return &depclass;
}

DependencyClass* Material::pClass(get_Class());

Material::Material() : DependencyObject(get_Class())
{
}

void Material::Apply(RenderContext* renderContext)
{
	Graphics::Direct3D10_I* pD10 = renderContext->GetRT()->m_d3d10;
	Graphics::OpenGL_I* pGL = renderContext->GetRT()->m_opengl;

	if (pD10)
	{
		X3DMaterial material;
		memset(&material, 0, sizeof(material));
		material.diffuseColor = Vec4f(get_DiffuseColor()->getVec3f(), 1);
	//	material.ambientColor = Vec4f(getDiffuseColor(), 1);
		HRESULT hr = pD10->m_d3d10_materialVariable->SetRawValue(&material, 0, sizeof(X3DMaterial));
		ASSERT(SUCCEEDED(hr));
	}
}

}	// Media3D
}	// Gui
}	// System
