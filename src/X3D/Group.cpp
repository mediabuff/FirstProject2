#include "stdafx.h"
#include "X3D2.h"
#include "Group.h"

namespace System
{
namespace x3d
{

NodeType* Group::GetNodeType()
{
	static NodeType nodeType(S("Group"), typeid(Group), nullptr, 0, baseClass::GetNodeType());
	return &nodeType;
}

NodeType* Group::nodeType(GetNodeType());

Group::Group() : X3DGroupingNode(GetNodeType())
{
}

// virtual
int Group::HitTest(const D3DXMATRIX& projection, const D3DXMATRIX& view, const D3DXMATRIX& model, const D3D10_VIEWPORT& vp, int type, Vec2f mousepos, Vec3f pos, IHitTest* hitTest)
{
	return HitTestChildren(getChildrenField(), projection, view, model, vp, type, mousepos, pos, hitTest);
}

// virtual
void Group::DrawGlobalLights(X3DDrawContext* pDC)
{
	DrawGlobalLightsChildren(pDC, getChildrenField());
}

// virtual
void Group::Draw(X3DDrawContext* pDC)
{
	DrawChildren(pDC, getChildrenField());
}

// virtual
void Group::DrawOverlay(X3DDrawContext* pDC)
{
	DrawOverlayChildren(pDC, getChildrenField());
}

}	// x3d
}
