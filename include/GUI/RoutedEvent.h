namespace System
{
namespace Gui
{

class GUIEXT RoutedEvent : public Object
{
public:
	CTOR RoutedEvent(StringIn name, RoutingStrategy routingStrategy, const Type_Info& handlerType, ClassType* ownerType);

	String get_Name();
	RoutingStrategy get_RoutingStrategy();
	ClassType* get_HandlerType();
	ClassType* get_OwnerType();

	int GetIndex() const
	{
		return m_index;
	}

private:

	friend class EventManager;
	friend class UIElement;	// TODO remove
//	friend class ButtonBase;	// TODO remove

	int m_index;

	String m_name;
	const Type_Info& m_handlerType;
	ClassType* m_ownerType;
	RoutingStrategy m_routingStrategy;
};

}	// Gui
}	// System
