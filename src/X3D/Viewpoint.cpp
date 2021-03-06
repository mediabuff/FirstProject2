#include "stdafx.h"
#include "X3D2.h"
#include "Viewpoint.h"

namespace System
{
namespace x3d
{

void DrawPerspectiveCamera(X3DDrawContext* pDC, gm::SizeF windowSize, float _fovy)
{
	double fovy = 45;
	double zNear = 0.5;
//	double zFar = 100.0;
	double zFar = zNear + 1.0;
	double aspect = windowSize.Width / windowSize.Height;

	double ntop = zNear * tan( fovy * M_PI / 360.0 );
	double nbottom = -ntop;
	double nleft = nbottom * aspect;
	double nright = ntop * aspect;

	double ftop = zFar * tan( fovy * M_PI / 360.0 );
	double fbottom = -ftop;
	double fleft = fbottom * aspect;
	double fright = ftop * aspect;

	/*
	ntop = -1+0.001;
	nbottom = 1-0.001;
	nleft = -1+0.001;
	nright = 1-0.001;
	*/
	/*
	nleft = -actualSize.Width/2;
	nright = actualSize.Width/2;
	nbottom = -actualSize.Height/2;
	ntop = actualSize.Height/2;
	*/

	/*
	nleft *= actualSize.Width/2;
	nright *= actualSize.Width/2;
	nbottom *= actualSize.Height/2;
	ntop *= actualSize.Height/2;

	fleft *= actualSize.Width/2;
	fright *= actualSize.Width/2;
	fbottom *= actualSize.Height/2;
	ftop *= actualSize.Height/2;
	*/

//	zFar -= zNear;
//	zNear -= zNear;
	{

		Vec3f points[] =
		{
			Vec3f(nleft, nbottom, -zNear),
			Vec3f(nleft, ntop, -zNear),
			Vec3f(nright, ntop, -zNear),
			Vec3f(nright, nbottom, -zNear),

			Vec3f(fleft, fbottom, -zFar),
			Vec3f(fleft, ftop, -zFar),
			Vec3f(fright, ftop, -zFar),
			Vec3f(fright, fbottom, -zFar),
		};

		uint indices[] =
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0,

			4, 5,
			5, 6,
			6, 7,
			7, 4,

			0, 4, 
			1, 5, 
			2, 6, 
			3, 7, 
		};

		Vec4f color(0.2f, 0.2f, 0.2f, 1.0f);

		Gui::Media3D::IndexedLines lines;
		lines.Create(pDC->m_renderContext, points, _countof(points), indices, _countof(indices), color);

		lines.Draw(pDC->m_renderContext);

		/*
		pG->glBegin(GL_LINES);

		pG->glVertexf(nleft, nbottom, -zNear);
		pG->glVertexf(nleft, ntop, -zNear);

		pG->glVertexf(nleft, ntop, -zNear);
		pG->glVertexf(nright, ntop, -zNear);

		pG->glVertexf(nright, ntop, -zNear);
		pG->glVertexf(nright, nbottom, -zNear);

		pG->glVertexf(nright, nbottom, -zNear);
		pG->glVertexf(nleft, nbottom, -zNear);

		pG->glEnd();
		*/
	}

#if 0
	{
		pG->glBegin(GL_LINES);

		pG->glVertexf(nleft, nbottom, -zNear);
		pG->glVertexf(fleft, fbottom, -zFar);

		pG->glVertexf(nleft, ntop, -zNear);
		pG->glVertexf(fleft, ftop, -zFar);

		pG->glVertexf(nright, ntop, -zNear);
		pG->glVertexf(fright, ftop, -zFar);

		pG->glVertexf(nright, nbottom, -zNear);
		pG->glVertexf(fright, fbottom, -zFar);

		pG->glEnd();
	}

	{
		pG->glBegin(GL_LINE_STRIP);

		pG->glVertexf(fleft, fbottom, -zFar);
		pG->glVertexf(fleft, ftop, -zFar);
		pG->glVertexf(fright, ftop, -zFar);
		pG->glVertexf(fright, fbottom, -zFar);
		pG->glVertexf(fleft, fbottom, -zFar);

		pG->glEnd();
	}
#endif
}

IMP_X3DFIELD3(Viewpoint, SFFloat, fieldOfView, inputOutput, float(M_PI_f/4), 0, M_PI_f)

NodeType* Viewpoint::GetNodeType()
{
	static X3DFieldDefinition* fields[] =
	{
		get_fieldOfViewFieldDef(),
	};

	static NodeType nodeType(WSTR("Viewpoint"), typeid(Viewpoint), fields, _countof(fields), X3DViewpointNode::GetNodeType());
	return &nodeType;
}

NodeType* Viewpoint::nodeType(GetNodeType());

Viewpoint::Viewpoint() : X3DViewpointNode(GetNodeType()),
	m_fieldOfView(new SFFloat(get_fieldOfViewFieldDef(), this))
{
}

D3DXMATRIX Viewpoint::GetProjectionMatrixLH(float width, float height, float zNear, float zFar)
{
	if (height == 0)
	{
		raise(ArgumentException());
	}

	float fov = getFieldOfView();

	float aspect = width/height;

	D3DXMATRIX m;
	D3DXMatrixPerspectiveFovLH(&m, fov, aspect, zNear, zFar);
	return m;
}

D3DXMATRIX Viewpoint::GetProjectionMatrixRH(float width, float height, float zNear, float zFar)
{
	if (height == 0)
	{
		raise(ArgumentException());
	}

	float fov = getFieldOfView();

	float aspect = width/height;

	D3DXMATRIX m;
	D3DXMatrixPerspectiveFovRH(&m, fov, aspect, zNear, zFar);
	return m;
}

// virtual
void X3DViewpointNode::OnFieldChanged(X3DField* pField)
{
	if (pField == getSetBindField())
	{
		if (getSetBindField()->getValue())
		{
			if (m_ownerLayer->m_viewpointStack.size() == 0 || m_ownerLayer->m_viewpointStack.top() != this)
			{
				if (m_ownerLayer->m_viewpointStack.size())	// send isBound FALSE to current
				{
					X3DViewpointNode* currentTop = m_ownerLayer->m_viewpointStack.top();

					SFBool* isBound = currentTop->getIsBoundField();//static_cast<SFBool*>(currentTop->getField(WSTR("isBound")));
					isBound->m_value = false;
					currentTop->OnFieldChanged(isBound);
				}

			//	std::vector<X3DNode*>::iterator it = m_pScene->m_viewpointStack.begin();
			//	while (it != m_pScene->m_viewpointStack.end())

				m_ownerLayer->m_viewpointStack.cont.erase(m_ownerLayer->m_viewpointStack.cont.find(this));

				/*
				for (int i = 0; i < m_scene->m_viewpointStack.size(); ++i)
				{
					if (m_scene->m_viewpointStack.cont[i] == this)
					{
						m_scene->m_viewpointStack.cont.RemoveAt(i);
						break;
					}
					//it++;
				}
				*/

				m_ownerLayer->m_viewpointStack.push(this);

				getIsBoundField()->m_value = true;
				Change(getIsBoundField());
			}
		}
		else
		{
		}
	}

	X3DBindableNode::OnFieldChanged(pField);
}

void Viewpoint::DrawOverlay(X3DDrawContext* pDC)
{
	X3DViewpointNode::DrawOverlay(pDC);

	Gui::Window* container = dynamic_cast<Gui::Window*>(m_scene->m_defaultView->GetRoot());

	// TODO
	gm::SizeF winsize = container->GetSize();

	pDC->m_renderContext->PushModelView(pDC->m_renderContext->modelViewMatrix() * GetViewMatrix());

	DrawPerspectiveCamera(pDC, winsize, getFieldOfView());

	pDC->m_renderContext->PopMatrix();
}

}	// x3d
}
