#include "stdafx.h"
#include "X3D2.h"
#include "SceneRenderFilter.h"

#include "OGLSceneRenderer.h"

namespace System
{
namespace x3d
{

#if 0
using namespace MediaShow;

SceneRenderFilter::SceneRenderFilter()
{
	m_pInput = new CInputPin;
	m_pInput->m_pFilter = this;
	m_pInput->m_id = WSTR("Input");
	AddPin(m_pInput);
}

ErrorCode_Bool SceneRenderFilter::CInputPin::Receive(IMediaSample *pSample)
{
	if (pSample)
	;//	GetFilter()->m_Control->SetScene(dynamic_cast<BIFSSample*>(pSample)->m_scene);

	ASSERT(0);
#if 0
	GetFilter()->m_Control->FireViewChange();
#endif

	return Success_True;
}
#endif

}	// x3d
}	// System
