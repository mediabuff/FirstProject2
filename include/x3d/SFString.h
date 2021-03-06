#ifndef x3d_SFString_h
#define x3d_SFString_h

namespace System
{
namespace x3d
{

class X3DEXT SFString : public X3DField
{
public:
	CTOR SFString(X3DNode* ownerNode) : X3DField(ownerNode)
	{
		m_value = nullptr;
	}

	CTOR SFString(TypedX3DFieldDefinition<SFString>* fieldDef, X3DNode* ownerNode);

	virtual FieldType getFieldType() const override
	{
		return FieldType_SFString;
	}

	virtual IO::TextWriter& WriteValueToStream(IO::TextWriter& stream) override
	{
		stream << '"' << m_value << '"';
		return stream;
	}

	virtual void CopyFrom(X3DField* pFrom)
	{
		m_value = static_cast<SFString*>(pFrom)->m_value;
	}

	String getValue() const
	{
		return m_value;
	}
	void setValue(StringIn value)
	{
		m_value = value;
		getOwnerNode()->OnFieldChanged(this);
	}

	static const FieldType fieldtype = FieldType_SFString;
	typedef String value_t;

public:

	String m_value;
};

}	// x3d
}	// System

#endif // x3d_SFString_h
