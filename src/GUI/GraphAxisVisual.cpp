#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

/*
IMP_DEPENDENCY_PROPERTY(WaveformData*, WaveformVisual, Data, NULL)
IMP_DEPENDENCY_PROPERTY(int64, WaveformVisual, AtSample, 0)
IMP_DEPENDENCY_PROPERTY(int64, WaveformVisual, SelectionStart, -1)
IMP_DEPENDENCY_PROPERTY(int64, WaveformVisual, SelectionEnd, -1)
*/

DependencyClass* GraphAxisVisual::get_Class()
{
	static DependencyClass depclass(typeid(GraphAxisVisual), UIElement::get_Class());

	/*
	static DependencyProperty* properties[] =
	{
		get_DataProperty(),
		get_AtSampleProperty(),
		get_SelectionStartProperty(),
		get_SelectionEndProperty(),
	};
	*/

	return &depclass;
}

GraphAxisVisual::GraphAxisVisual() : UIElement(get_Class())
{
}

void GraphAxisVisual::Render(ManagedRenderContext renderContext)
{
	for (uint i = 0; i < m_points.size(); ++i)
	{
		Point pt = Point(m_points[i]->get_X(), m_points[i]->get_Y());

		renderContext.FillEllipse(pt.X, pt.Y, 5, 5, new SolidColorBrush(m_colors[i]));
	}
}

void GraphAxisVisual::OnMouseLeftButtonDown(MouseButtonEventArgs* args)
{
	m_oldpoint = args->GetPosition(this);

	CaptureMouse();
}

void GraphAxisVisual::OnMouseLeftButtonUp(MouseButtonEventArgs* args)
{
	ReleaseMouseCapture();
}

void GraphAxisVisual::OnMouseMove(MouseEventArgs* args)
{
	Point point = args->GetPosition(this);

	if (get_IsMouseCaptured())
	{
		Point delta = point - m_oldpoint;

		m_points[0]->set_X(m_points[0]->get_X() + delta.X);
		m_points[0]->set_Y(m_points[0]->get_Y() + delta.Y);

		m_oldpoint = point;
		InvalidateVisual();
	}
}

}
}
