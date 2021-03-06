namespace System
{
namespace Gui
{

class GUIEXT Control : public FrameworkElement
{
protected:
	CTOR Control(DependencyClass* depClass);
	CTOR Control(DependencyClass* depClass, gm::SizeF size);

public:

	DEFINE_THIS(_Control)

	Template* get_Template();
	void set_Template(Template* pTemplate);

//	virtual Variant GetLocalPropertyValue(PropertyValue* pPropertyVal) override;

	virtual void RenderRetained(ManagedRetainedRenderContext renderContext) override;

	DEPENDENCY_PROPERTY(String, FontFamily)
	DEPENDENCY_PROPERTY(float, FontSize)
	DEPENDENCY_PROPERTY(int, FontWeight)
	DEPENDENCY_PROPERTY(FontStyle, FontStyle)

	DEPENDENCY_PROPERTY(Brush*, Foreground)
	DEPENDENCY_PROPERTY(Brush*, Background)
	DEPENDENCY_PROPERTY(Brush*, BorderBrush)

	DEPENDENCY_CLASS_ABSTRACT(Control, FrameworkElement)

protected:

	virtual void Render(ManagedRetainedRenderContext renderContext);

//	virtual UIElement* CreateShadowTree() = 0;

	Template* m_Template;
};

class GUIEXT _Control : public _FrameworkElement
{
public:
	CTOR _Control(const Expressive::expression& exp) : _FrameworkElement(exp)
	{
	}

	_UIElement getShadowTree() const
	{
		return getProperty(Control::get_ShadowTreeProperty());
	}
};

}	// Gui
}	// System
