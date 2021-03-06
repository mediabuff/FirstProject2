#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

BindingExpression::BindingExpression(IBinding* binding) : m_binding(binding), m_ownerBinding(NULL), m_setting(false)
{
	m_pType = binding->GetOutputType();
}

int BindingExpression::SetInput(IBinding* caller, const Variant&)
{
	if (m_setting == false)
	{
		m_setting = true;
		ExpressionBinding* expBinding = dynamic_cast<ExpressionBinding*>(m_ownerBinding);
		ASSERT(expBinding);

		Expressive::EvalContext context;
		context.m_userObject = m_ownerBinding;
		context.m_this = m_ownerBinding->m_this;

	//	m_parent->OnChildChanged(this);
		m_ownerBinding->m_exp->TraceCallback(&context, Expressive::ExpTrace(), NULL);

		m_ownerBinding->ReEval();
		m_setting = false;
	}

	return 0;
}

#if 0
int BindingExpression::SetInput(IBinding* caller, int)
{
	if (m_setting == false)
	{
		m_setting = true;
		ExpressionBinding* expBinding = dynamic_cast<ExpressionBinding*>(m_ownerBinding);
		ASSERT(expBinding);

		Expressive::EvalContext context;
		context.m_userObject = m_ownerBinding;
		context.m_this = m_ownerBinding->m_this;

	//	m_parent->OnChildChanged(this);
		m_ownerBinding->m_exp->TraceCallback(&context, Expressive::ExpTrace(), NULL);

		m_ownerBinding->ReEval();
		m_setting = false;
	}

	return 0;
}
#endif

//virtual Type* GetInputType();// override;	// IInputBinding

Variant BindingExpression::Evaluate(Expressive::EvalContext* context)
{
//	IBinding* binding = dynamic_cast<IBinding*>(context->m_userObject);

	return m_binding->GetOutput(nullptr);
}

void BindingExpression::TraceCallback(Expressive::EvalContext* context, Expressive::ExpTrace& trace, Expressive::ITraceCallback* callback)
{
	m_ownerBinding = dynamic_cast<ExpressionBinding*>(context->m_userObject);

	if (m_ptr == nullptr)
	{
//	ExpressionBinding* binding = dynamic_cast<ExpressionBinding*>(context->m_userObject);
//	void* ptr = m_binding->Connect(binding);
	m_ptr = m_binding->Connect(this);
//	binding->m_inBindings.push_back(m_binding);
	}
}

}	// Gui
}	// System
