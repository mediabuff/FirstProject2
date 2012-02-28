#include "stdafx.h"
#include "X3D2.h"

namespace System
{
namespace x3d
{

NodeType* X3DShaderNode::GetNodeType()
{
	static X3DFieldDefinition activate("activate", FieldType_SFBool, SAIFieldAccess_inputOnly, offsetof(X3DShaderNode, m_activate));
	static X3DFieldDefinition isSelected("isSelected", FieldType_SFBool, SAIFieldAccess_outputOnly, offsetof(X3DShaderNode, m_isSelected));
	static X3DFieldDefinition isValid("isValid", FieldType_SFBool, SAIFieldAccess_outputOnly, offsetof(X3DShaderNode, m_isValid));
	static X3DFieldDefinition language("language", FieldType_SFString, SAIFieldAccess_initializeOnly, offsetof(X3DShaderNode, m_language));

	static X3DFieldDefinition* fields[] =
	{
		// X3DShaderNode
		&activate,
		&isSelected,
		&isValid,
		&language,
	};

	static NodeType nodeType(nullptr, typeid(X3DShaderNode), fields, _countof(fields), X3DAppearanceChildNode::GetNodeType());
	return &nodeType;
}

X3DShaderNode::X3DShaderNode(NodeType* nodeType) : X3DAppearanceChildNode(nodeType)
{
}

/*
X3DField* X3DShaderNode::createField(System::StringW* name, FieldType fieldType)
{
	X3DField* x3dfield = CreateFieldOfType(fieldType, this);

	// TODO, insert sorted
	m_fields.Add(x3dfield);

	m_addfields.Add(x3dfield);

	return x3dfield;
}
*/

void X3DShaderNode::AddField(X3DField* x3dfield)
{
//	// TODO, insert sorted
//	m_fields.Add(x3dfield);

	m_addfields.Add(x3dfield);
}

NodeType* X3DGeometricPropertyNode::GetNodeType()
{
	/*
	X3DFieldDefinition* fields[] =
	{
		&X3DFieldDefinition(WSTR("name"), SAIFieldAccess_inputOutput, offsetof(X3DVertexAttributeNode, m_name)),
	};
	static NodeType nodeType(NULL, typeid(X3DVertexAttributeNode), fields, _countof(fields), &X3DNode::GetNodeType());
	*/

	static NodeType nodeType(nullptr, typeid(X3DVertexAttributeNode), nullptr, 0, X3DNode::GetNodeType());
	return &nodeType;
}

NodeType* X3DVertexAttributeNode::GetNodeType()
{
	static X3DFieldDefinition* fields[] =
	{
		&TypedX3DFieldDefinition<SFString>("name", SAIFieldAccess_inputOutput, offsetof(X3DVertexAttributeNode, m_name)),
	};

	static NodeType nodeType(nullptr, typeid(X3DVertexAttributeNode), fields, _countof(fields), X3DGeometricPropertyNode::GetNodeType());
	return &nodeType;
}

X3DVertexAttributeNode::X3DVertexAttributeNode(NodeType* nodeType) : X3DGeometricPropertyNode(nodeType)
{
}

///

X3DFieldDefinition* Matrix4VertexAttributeFields[] =
{
	&X3DFieldDefinition("name", FieldType_SFString, SAIFieldAccess_initializeOnly,-1),
	&X3DFieldDefinition("value", FieldType_MFMatrix4f, SAIFieldAccess_inputOutput,-1),

	/*

	SFNode   [in,out] metadata NULL [X3DMetadataObject]
	SFString []       name     "" 

  MFMatrix4f [in,out] value    []   (-?,?)
	*/
};

NodeType Matrix4VertexAttribute::s_nodeType("Matrix4VertexAttribute", typeid(Matrix4VertexAttribute), Matrix4VertexAttributeFields, _countof(Matrix4VertexAttributeFields));

Matrix4VertexAttribute::Matrix4VertexAttribute() : X3DVertexAttributeNode(&s_nodeType)
{
}

}	// x3d
}	// System
