namespace System
{
namespace Gui
{

class GUIEXT RectangleGeometry : public Geometry
{
public:

	CTOR RectangleGeometry();
	CTOR RectangleGeometry(gm::RectF rect);
	CTOR RectangleGeometry(gm::PointF topleft, gm::SizeF size);
	CTOR RectangleGeometry(float left, float top, float width, float height);

	virtual void RenderFill(ManagedRenderContext renderContext, Brush* brush) override;
	virtual void RenderStroke(ManagedRenderContext renderContext, Brush* brush, float width) override;
	virtual ID2D1RectangleGeometry* GetD2DGeometry() override;
	virtual HRGN CreateHRGN() override;

	virtual Geometry* CombineWithGeometry(Geometry* pgeometry, D2D1_COMBINE_MODE combineMode, const D2D1_MATRIX_3X2_F* inputGeometryTransform) override;

	virtual void OnComputedPropertyValueChanged(PropertyValue* pPropertyVal, bool handled) override;

	virtual gm::RectF GetBounds() override;
	virtual bool FillContainsPoint(gm::PointF point) override;

	gm::RectF GetRect();

	DEPENDENCY_CLASS(RectangleGeometry, Geometry)

	DEFINE_THIS(_RectangleGeometry)

	DEPENDENCY_PROPERTY(float, Left)
	DEPENDENCY_PROPERTY(float, Top)
	DEPENDENCY_PROPERTY(float, Width)
	DEPENDENCY_PROPERTY(float, Height)
	DEPENDENCY_PROPERTY(float, Right)	// Computed
	DEPENDENCY_PROPERTY(float, Bottom)	// Computed

protected:

	ComPtr<ID2D1RectangleGeometry> m_D2DGeometry;
};

class GUIEXT _RectangleGeometry : public _Geometry
{
public:

	CTOR _RectangleGeometry(const Expressive::expression& exp) : _Geometry(exp)
	{
	}

	Expressive::typed_expression<float> getLeft() const
	{
		return getProperty(RectangleGeometry::get_LeftProperty());
	}

	Expressive::typed_expression<float> getTop() const
	{
		return getProperty(RectangleGeometry::get_TopProperty());
	}

	Expressive::typed_expression<float> getWidth() const
	{
		return getProperty(RectangleGeometry::get_WidthProperty());
	}

	Expressive::typed_expression<float> getHeight() const
	{
		return getProperty(RectangleGeometry::get_HeightProperty());
	}

	Expressive::typed_expression<float> getRight() const
	{
		return getProperty(RectangleGeometry::get_RightProperty());
	}

	Expressive::typed_expression<float> getBottom() const
	{
		return getProperty(RectangleGeometry::get_BottomProperty());
	}
};

}	// Gui
}	// System
