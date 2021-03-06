#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

IMP_DEPENDENCY_PROPERTY_T(T, T, Vector4<T>, X, T(0))
IMP_DEPENDENCY_PROPERTY_T(T, T, Vector4<T>, Y, T(0))
IMP_DEPENDENCY_PROPERTY_T(T, T, Vector4<T>, Z, T(0))
IMP_DEPENDENCY_PROPERTY_T(T, T, Vector4<T>, W, T(1))

template<class T>
DependencyClass* Vector4<T>::get_Class()
{
	static DependencyClass depclass(typeid(Vector4<T>), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_XProperty(),
		get_YProperty(),
		get_ZProperty(),
		get_WProperty(),
	};

	return &depclass;
}

template<class T> DependencyClass* Vector4<T>::pClass(get_Class());

template class GUIEXT Vector4<unsigned char>;
template class GUIEXT Vector4<int>;
template class GUIEXT Vector4<long long>;
template class GUIEXT Vector4<float>;
template class GUIEXT Vector4<double>;

}
}
