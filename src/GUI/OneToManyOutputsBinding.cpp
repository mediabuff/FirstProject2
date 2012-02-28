#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

OneToManyOutputsBinding::OneToManyOutputsBinding()
{
}

OneToManyOutputsBinding::OneToManyOutputsBinding(IBinding* inBinding)
{
	SetInputBinding(inBinding);
}

void OneToManyOutputsBinding::SetInputBinding(IBinding* inBinding)
{
	ASSERT(m_inBinding == NULL);

	if (inBinding == NULL) throw new ArgumentNullException();

	m_inBinding = inBinding;
	m_inBindingPtr = m_inBinding->Connect(this);

	/*
	if (m_localValue == &UnsetValue)
	{
		m_computedValue = &UnsetValue;
		OnComputedValueChanged();

		// TODO, update here, unless in init phase
	}
	*/
}

int OneToManyOutputsBinding::OutputValue(const Variant& value)
{
	int result = 0;

	for (auto it = m_outBindings.begin(); it != m_outBindings.end(); ++it)
	{
		IInputBinding* binding = *it;
	//	++it;

		int result2 = binding->SetInput(this, value);
		if (result2)
		{
			++result;
		}
	}

	return result;
}

Variant OneToManyOutputsBinding::GetOutput(IBinding* caller)
{
	if (caller != NULL)	// hmm..
	{
		ASSERT(m_outBindings.find(caller) != m_outBindings.end());
	}
	return GetValue();
}

int OneToManyOutputsBinding::SetOutput(IBinding* caller, const Variant& value)
{
	SetValue(value);
	return 0;
}

int OneToManyOutputsBinding::SetInput(IBinding* caller, const Variant& value)
{
	ASSERT(0);
	return 0;
}

void* OneToManyOutputsBinding::Connect(IInputBinding* binding)
{
	ASSERT(m_outBindings.find(binding) == m_outBindings.end());
	return m_outBindings.insert(m_outBindings.end(), binding).m_ptr;
}

}	// Gui
}	// System
