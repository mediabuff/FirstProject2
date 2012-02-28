#include "stdafx.h"
#include "X3D2.h"

namespace System
{
namespace x3d
{

NodeType* X3DNBodyCollidableNode::GetNodeType()
{
	static X3DFieldDefinition enabled("enabled", FieldType_SFBool, SAIFieldAccess_inputOutput, offsetof(X3DNBodyCollidableNode, m_enabled));
	static X3DFieldDefinition rotation("rotation", FieldType_SFRotation, SAIFieldAccess_inputOutput, offsetof(X3DNBodyCollidableNode, m_rotation));
	static X3DFieldDefinition translation("translation", FieldType_SFVec3f, SAIFieldAccess_inputOutput, offsetof(X3DNBodyCollidableNode, m_translation));

	static X3DFieldDefinition* fields[] =
	{
		&enabled,
		&rotation,
		&translation,
//		X3DFieldDefinition(WSTR("bboxSize"), FieldType_SFVec3f, SAIFieldAccess_initializeOnly,-1),
//		X3DFieldDefinition(WSTR("children"), FieldType_MFNode, SAIFieldAccess_inputOutput,-1),
	};

	static NodeType nodeType(nullptr, typeid(X3DNBodyCollidableNode), fields, _countof(fields), X3DChildNode::GetNodeType());
	return &nodeType;
}

X3DNBodyCollidableNode::X3DNBodyCollidableNode(NodeType* nodeType) : X3DChildNode(nodeType)
{
}

}	// x3d
}	// System

