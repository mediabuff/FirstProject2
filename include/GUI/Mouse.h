namespace System
{
namespace Gui
{

class GUIEXT Mouse
{
public:

//	static ValueBinding<Vector2_F*>* get_Position();
	static Vector2_F* get_Position();

	static ValueBinding<float> s_ScreenXBinding;
	static ValueBinding<float> s_ScreenYBinding;
	static ValueBinding<bool> s_LeftButtonDownBinding;
	static ValueBinding<bool> s_MiddleButtonDownBinding;
	static ValueBinding<bool> s_RightButtonDownBinding;

	static inline gm::PointI GetCursorPos() throw()
	{
		POINT pt;
		::GetCursorPos(&pt);
		return pt;
	}

private:

	CTOR Mouse();	// non-creatable
};

class GUIEXT _Mouse
{
public:
	static Expressive::typed_expression<float> screenposX();
	static Expressive::typed_expression<float> screenposY();

	static Expressive::typed_expression<bool> leftButtonDown();
	static Expressive::typed_expression<bool> middleButtonDown();
	static Expressive::typed_expression<bool> rightButtonDown();
};

}	// Gui
}	// System
