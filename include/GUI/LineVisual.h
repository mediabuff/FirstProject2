namespace System
{
namespace Gui
{

class GUIEXT LineVisual : public UIElement
{
public:

	CTOR LineVisual();
	CTOR LineVisual(typed<float> x1, typed<float> y1, typed<float> x2, typed<float> y2);
	CTOR LineVisual(typed<float> x1, typed<float> y1, typed<float> x2, typed<float> y2, typed<Brush*> brush, typed<float> strokeWidth);

	virtual void Render(ManagedRenderContext renderContext) override;
	virtual void OnComputedPropertyValueChanged(PropertyValue* pPropertyValue, bool handled) override;
	virtual Variant GetLocalPropertyValue(PropertyValue* pPropertyVal) override;

	virtual geometry GetHitGeometry() override;

	gm::PointF get_Start();
	gm::PointF get_End();

	DEPENDENCY_PROPERTY(float, X1)
	DEPENDENCY_PROPERTY(float, Y1)
	DEPENDENCY_PROPERTY(float, X2)
	DEPENDENCY_PROPERTY(float, Y2)
	DEPENDENCY_PROPERTY(Brush*, Brush)
	DEPENDENCY_PROPERTY(float, Width)
	DEPENDENCY_PROPERTY(StrokeStyle*, Style)

	DEPENDENCY_CLASS(LineVisual, UIElement)

private:

	pathgeometry m_hitGeometry;
};

}	// Gui
}	// System
