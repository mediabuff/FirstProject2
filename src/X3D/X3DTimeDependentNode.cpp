#include "stdafx.h"
#include "X3D2.h"

namespace System
{
namespace x3d
{

X3DTimeDependentNode::X3DTimeDependentNode(NodeType* nodeType) : X3DChildNode(nodeType),
	m_loop(new SFBool(this, false))
{
}

NodeType* X3DTimeDependentNode::GetNodeType()
{
	static X3DFieldDefinition isActive("isActive", FieldType_SFBool, SAIFieldAccess_outputOnly, offsetof(TimeSensor, m_isActive));
	static X3DFieldDefinition loop("loop", FieldType_SFBool, SAIFieldAccess_inputOutput, offsetof(TimeSensor, m_loop));
	static X3DFieldDefinition startTime("startTime", FieldType_SFTime, SAIFieldAccess_inputOutput, offsetof(TimeSensor, m_startTime));
	static X3DFieldDefinition stopTime("stopTime", FieldType_SFTime, SAIFieldAccess_inputOutput, offsetof(TimeSensor, m_stopTime));
	static X3DFieldDefinition elapsedTime("elapsedTime", FieldType_SFTime, SAIFieldAccess_outputOnly, offsetof(TimeSensor, m_elapsedTime));

	static X3DFieldDefinition* fields[] =
	{
		&isActive,
		&loop,
		&startTime,
		&stopTime,
		&elapsedTime,
	};

	static NodeType nodeType(nullptr, typeid(X3DTimeDependentNode), fields, _countof(fields), X3DChildNode::GetNodeType());
	return &nodeType;
}

}	// x3d
}	// System
