#ifndef Web_CDATASection_h
#define Web_CDATASection_h

namespace System
{
namespace Web
{

class WEBEXT CDATASection : public CharacterData
{
public:
	CTOR CDATASection();

	Text* splitText(int offset)
	{
		VERIFY(0);
		return nullptr;
	}
	String get_wholeText()
	{
		VERIFY(0);
		return nullptr;
	}

	NodeType get_nodeType() const;
	String get_nodeName() const;

	Node* cloneNodeExport(Document* ownerDocument, bool deep) const;

protected:

	static ImmutableString<wchar_t> s_nodeName;
};

}	// Web
}

#endif // Web_CDATASection_h
