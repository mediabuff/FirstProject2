// DXFile.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "dxfile.h"

#pragma comment(lib, "LFC")
#pragma comment(lib, "Graphics")
#pragma comment(lib, "GUI")

#ifdef _DEBUG
#pragma comment(lib, "d3d10_1")
#pragma comment(lib, "d3dx10d")
#else
#pragma comment(lib, "d3d10_1")
#pragma comment(lib, "d3dx10")
#endif

#pragma comment(lib, "dxgi")

namespace System
{

namespace DX
{

//using namespace std;
using namespace IO;

class Instance;
class Template;
class Member;

class Dimension
{
public:
	Dimension()
	{
		name = nullptr;
		pMember = nullptr;
	}

	enum
	{
		Integer,
		Name
	}
	type;

	int num;
	String name;
	Member* pMember;
};

Dimension* Dimension_Integer(int num)
{
	Dimension* p = new Dimension;
	p->type = Dimension::Integer;
	p->num = num;
	p->name = nullptr;
	return p;
}

Dimension* Dimension_Name(String name)
{
	Dimension* p = new Dimension;
	p->type = Dimension::Name;
	p->name = name;
	return p;
}

/*
class DataType
{
public:

	DataType()
	{
		m_sizeof = 0;
	}
	enum
	{
		vector,
		Primitive,
		Name,
	}
	type;

	DWORD m_sizeof;

	union
	{
		int primitive;
		struct
		{
			char* name;
			Template* pResolved;
		}
		template_name;

		struct
		{
			DataType* type;
			Dimension* dim;
		}
		array;
	};
};
*/

DataType* DataType_Name(StringA* name)
{
	DataType* p = new DataType;
	p->type = DataType::Name;
	p->template_name.name = name;
	p->template_name.pResolved = NULL;

	p->m_sizeof = sizeof(Instance*);

	return p;
}

DataType* DataType_Primitive(int primitive);

DataType* DataType_Array(DataType* arraytype, Dimension* dim)
{
	DataType* p = new DataType;
	p->type = DataType::vector;
	p->array_.type = arraytype;
	p->array_.dim = dim;

	p->m_sizeof = sizeof(void*);

	return p;
}

/*
class Member
{
public:
	Member();

	DataType* m_dataType;
	StringA m_name;
	DWORD m_offset;
};
*/

Member::Member()
{
	m_dataType = NULL;
}

Member* x_Member(DataType* dataType, StringA* name)
{
	ASSERT(dataType);

	Member* p = new Member;
	p->m_dataType = dataType;
	p->m_name = name;
	return p;
}

/*
Member* Member_Array(DataType* datatype)
{
	return NULL;
}
*/

/*
class Template
{
public:

	StringA GetName();

	StringA m_name;
	std::vector<Member*> m_members;
	std::map<StringA, Member*> m_membersByName;

	DWORD m_sizeof;
};
*/

String Template::GetName()
{
	return m_name;
}

Template* x_Template(String name)
{
	Template* p = new Template;
	p->m_name = name;
	return p;
}

/*
class xfileparser
{
public:
	std::map<StringA, Template*> m_templates;
	std::vector<Member*> m_members;

	std::vector<Instance*> m_objects;

	int ParseX(ISequentialByteStream* stream);

	void AddTemplate(Template* pTemplate);

};
*/

void xfileparser::AddTemplate(Template* pTemplate)
{
	m_templates[pTemplate->m_name] = pTemplate;

	uint offset = 0;
	for (uint i = 0; i < pTemplate->m_members.GetSize(); i++)
	{
		Member* pMember = pTemplate->m_members[i];
		pTemplate->m_membersByName[pMember->m_name] = pMember;

		pMember->m_offset = offset;

		if (pMember->m_dataType->type == DataType::vector)
		{
			if (pMember->m_dataType->array_.dim->name)
			{
				pMember->m_dataType->array_.dim->pMember = pTemplate->m_membersByName[pMember->m_dataType->array_.dim->name];
				/*
				Member* pMember2 = pTemplate->m_membersByName[pMember->m_dataType->array.dim->name];
				if (pMember2 == NULL)
					throw -1;

				if (pMember2->m_dataType->type == Primitive)
				{
					if (pMember2->m_dataType->primitive == TOKEN_DWORD)
					{
						pMember->m_dataType->array.dim->num = 
					}
					else
						throw -1;
				}
				else
					throw -1;
					*/
			}
		}
#if 0		
		else if (pMember->m_dataType->type == DataType::Name)
		{
			Template* pRefTemplate = m_templates[pMember->m_dataType->template_name.name];
			if (pRefTemplate == NULL) throw -1;
			pMember->m_dataType->template_name.pResolved = pRefTemplate;

		//	pMember->m_dataType->m_sizeof = pRefTemplate->m_sizeof;
		}	
#endif

		offset += pMember->m_dataType->m_sizeof;
	}
	pTemplate->m_sizeof = offset;
}

xfileparser* parser;

extern "C" Instance* parse_object(DataType* name) throw(Exception*);

extern "C"
{
#include "xfile_parse.tab.c"

#include "lex.xfile_.c"
}

extern "C" void xfile_error (/*YYLTYPE *llocp,*/ char const *s)
{
	printf(s);
}

extern "C" int xfile_wrap()
{
	return 1;	// terminate lex scanner
}

DataType* DataType_Primitive(int primitive)
{
	DataType* p = new DataType;
	p->type = DataType::Primitive;
	p->primitive = primitive;

	switch (primitive)
	{
	case TOKEN_CHAR:
	case TOKEN_UCHAR:
		p->m_sizeof = 1;
		break;

	case TOKEN_WORD:
	case TOKEN_SWORD:
		p->m_sizeof = 2;
		break;

	case TOKEN_FLOAT:
	case TOKEN_DWORD:
	case TOKEN_SDWORD:
		p->m_sizeof = 4;
		break;

	case TOKEN_DOUBLE:
		p->m_sizeof = 8;
		break;

	case TOKEN_CSTRING:
		p->m_sizeof = sizeof(StringA*);
		break;

	default:
		ASSERT(0);
	}
	return p;
}

#if 0
const char* m_p;
const char* m_buffer;
int m_size;

int xfile_lex2()
{
	const char* &p = m_p;

	if (m_p - m_buffer >= m_size)
		return 0;

	char c = *p;

	YYSTYPE& token = xfile_lval;
	//token.m_name = StringA();

	if (isalpha(c) || c == '_')
	{
		String str;

		str += c;
		p++;

		c = *p;
		while (isalnum(c) || c == '_')
		{
			p++;
			str += c;
			c = *p;
		}

		//String str = buffer.DetachToString();

		if (str == "template")
		{
			return TOKEN_TEMPLATE;
		}
		else if (str == "CHAR")
		{
			return TOKEN_CHAR;
		}
		else if (str == "UCHAR")
		{
			return TOKEN_UCHAR;
		}
		else if (str == "WORD")
		{
			return TOKEN_WORD;
		}
		else if (str == "DWORD")
		{
			return TOKEN_DWORD;
		}
		else if (str == "SWORD")
		{
			return TOKEN_SWORD;
		}
		else if (str == "SDWORD")
		{
			return TOKEN_SDWORD;
		}
		else if (str == "FLOAT")
		{
			return TOKEN_FLOAT;
		}
		else if (str == "DOUBLE")
		{
			return TOKEN_DOUBLE;
		}
		else if (str == "STRING")
		{
			return TOKEN_CSTRING;
		}
		else if (str == "array")
		{
			return TOKEN_ARRAY;
		}
		else
		{
			token.pName = dynamic_cast<StringA*>(str.m_stringObject);
			return TOKEN_NAME;
		}
	}
	else if (c == '{')
	{
		p++;
		return TOKEN_OBRACE;
	}
	else if (c == '}')
	{
		p++;
		return TOKEN_CBRACE;
	}
	else if (c == '[')
	{
		p++;
		return TOKEN_OBRACKET;
	}
	else if (c == ']')
	{
		p++;
		return TOKEN_CBRACKET;
	}
	else if (c == '<')
	{
		// TODO
	//	token.m_type = TOKEN_OANGLE;
		p++;
		while (*p != '>')
		{
			p++;
		}
		p++;

		return TOKEN_GUID;
	}
	else if (c == '>')
	{
		//token.m_type = tok_cbrace;
	}
	else if (c == ';')
	{
		p++;
		return TOKEN_SEMICOLON;
	}
	else if (c == ',')
	{
		p++;
		return TOKEN_COMMA;
	}
	else if (c == '.')
	{
		p++;
		return TOKEN_DOT;
	}
	else if (c == '\"')
	{
		String str;
		p++;
		while (*p != '\"')
		{
			str += *p;
			p++;
		}
		p++;

		token.pName = dynamic_cast<StringA*>(str.m_stringObject);

		return TOKEN_STRING;
	}
	else if (isdigit(c) || c == '-')
	{
	// get sign
		int sign = 1;

		if ((c == '-') || (c == L'+'))
		{
			if (c == L'-') sign = -1;
			p++;
		}

	// get number
		long integer = _getnumber(&p);

	// get fraction
		double fraction;
		if (*p == '.')
		{
			p++;
			fraction = getfrac(&p);

			int exp = 0;
			/*
		// get exp
			exp = lookexp(&p);
			*/

			double value = makeflt(integer, fraction, exp);
			if (sign == -1) value = -value;

			token.floatValue = value;
			return TOKEN_FNUM;
		}
		else
		{
			token.intValue = integer;
			return TOKEN_INTEGER;

	//		fraction = 0;

		//	double specifiedValue = makeflt(integer, fraction, exp);
		//	if (sign == -1) specifiedValue = -specifiedValue;
		}
	}

	throw -1;

	return 0;
}
#endif

#if 0
extern "C" int xfile_lex()
{
	int token = xfile_lex2();

	while (isspace(*m_p))
	{
		if (*m_p == '\n')
		{
			xfile_lloc.first_line++;
		}

		m_p++;
	}

	return token;
}
#endif

class Instance;

/*
union DataValue
{
	long m_intValue;
	float m_floatValue;
	double m_doubleValue;
	Instance* m_object;
	_StringA* m_stringValue;
	DataValue* m_arrayElements;
};
*/

/*
class MemberData
{
public:

	Instance* m_pInstance;
	Member* m_pMember;
	DataValue* m_pValue;
};
*/

class Reference
{
public:
	String m_name;
};

#if 0
class vector
{
public:
	uint8* m_data;

	operator [] (int index)
	{
	}
};
#endif

class DataValue
{
public:
	DataValue(const DataType* dataType, const uint8* data)
	{
		m_dataType = dataType;
		m_data = data;
	}

	operator uint32 () const
	{
		if (m_dataType->type == DataType::Primitive)
		{
			if (m_dataType->primitive == TOKEN_DWORD ||
				m_dataType->primitive == TOKEN_SDWORD)
			{
				return *(uint32*)m_data;
			}
		}

		raise(Exception("Cannot convert to (S)DWORD"));
	}

	operator float () const throw(Exception)
	{
		if (m_dataType->type == DataType::Primitive)
		{
			if (m_dataType->primitive == TOKEN_FLOAT)
			{
				return *(float*)m_data;
			}
		}

		raise(Exception(L"Cannot convert to float"));
	}

	operator String () const throw(Exception)
	{
		if (m_dataType->type == DataType::Primitive)
		{
			if (m_dataType->primitive == TOKEN_CSTRING)
			{
				return String(*(StringA**)m_data);
			}
		}

		raise(Exception(L"Cannot convert to String"));
	}

	/*
	operator Instance* () const
	{
		if (m_dataType->type == DataType::Primitive)
		{
			if (m_dataType->primitive == TOKEN_CSTRING)
			{
				return *(_StringA**)m_data;
			}
		}

		throw -1;
	}
	*/

	Instance* operator -> ()
	{
		if (m_dataType->type == DataType::Name)
		{
			return *(Instance**)m_data;
		}
		raise(Exception(L"Is not instance"));
	}

	operator Instance* ()
	{
		if (m_dataType->type == DataType::Name)
		{
			return *(Instance**)m_data;
		}
		raise(Exception(L"Is not instance"));
	}

	operator const uint32* () const
	{
		if (m_dataType->type == DataType::vector)
		{
			if (m_dataType->array_.type->primitive == TOKEN_DWORD)
			{
				return *(uint32**)m_data;
			}
		}

		raise(Exception(L"Is not array of DWORD"));
	}

	operator const float* () const
	{
		if (m_dataType->type == DataType::vector)
		{
			if (m_dataType->array_.type->primitive == TOKEN_FLOAT)
			{
				return *(float**)m_data;
			}
		}

		raise(Exception(L"Is not array of float"));
	}

	const DataType* m_dataType;
	const uint8* m_data;
};

class Instance : public Object
{
public:
	Instance()
	{
		m_pTemplate = nullptr;
		m_data = nullptr;
		m_name = nullptr;
	}

	virtual ~Instance()
	{
	}

	Template* GetTemplate() throw()
	{
		return m_pTemplate;
	}

	String GetName() throw()
	{
		return m_name;
	}

	Instance* FindDataOfType(StringIn type);

	/*
	int GetIntValue(StringA name)
	{
		MemberData* data = m_members[name];

		if (m_pTemplate->m_members[data->m_index]->type == DataType::Primitiveprimitive
	}
*/

	ULONG GetOffset(StringIn name) const;
	void* GetData(StringIn name) const;
	DataValue GetValue(StringIn name) const;

	map<String, Object*> m_userdata;
	Template* m_pTemplate;
	String m_name;
	ubyte* m_data;
	vector<Instance*> m_children;
	vector<Reference*> m_references;
};

/*
ULONG Instance::GetOffset(StringA name)
{
	const char* p = name.c_str();
	while (*p)
	{
		if (
	}
}

class Path
{
public:
	operator ULONG

	Path operator -> ()
	{
	}
};
*/

ULONG Instance::GetOffset(StringIn name) const
{
	// TODO, use find
	return m_pTemplate->m_membersByName[name]->m_offset;
}

void* Instance::GetData(StringIn name) const
{
	// TODO, use find and possibly return NULL
	return m_data + m_pTemplate->m_membersByName[name]->m_offset;
}

DataValue Instance::GetValue(StringIn name) const
{
	Member* pMember = m_pTemplate->m_membersByName.find(name)->second;
	if (pMember == NULL)
		raise(Exception(L"member not found"));

	return DataValue(pMember->m_dataType, m_data + pMember->m_offset);
}

Instance* Instance::FindDataOfType(StringIn type)
{
	for (int i = 0; i < m_children.GetSize(); i++)
	{
		Instance* pObject = m_children[i];
		if (pObject->m_pTemplate->m_name == type)
		{
			return pObject;
		}
	}

	return NULL;
}

uint8* ReadData(Instance* pInstance, DataType* dataType, ubyte* data, int& token);

void ReadInstanceData(Instance* pInstance, int& token)
{
	pInstance->m_data = new ubyte[pInstance->m_pTemplate->m_sizeof];

	ubyte* data = pInstance->m_data;

	size_t nmembers = pInstance->m_pTemplate->m_members.GetSize();
	for (size_t i = 0; i < nmembers; ++i)
	{
		Member* pMember = pInstance->m_pTemplate->m_members[i];

		data = ReadData(pInstance, pMember->m_dataType, data, token);

		//token = xfile_lex();
	}
}

ubyte* ReadData(Instance* pInstance, DataType* dataType, ubyte* data, int& token)
{
	switch (dataType->type)
	{
	case DataType::Primitive:
		{
			switch (dataType->primitive)
			{
			case TOKEN_CHAR:
			case TOKEN_UCHAR:
				{
					if (token != TOKEN_INTEGER)
					{
						raise(Exception());
					}

					*(char*)data = xfile_lval.intValue;
					data += 1;

					token = xfile_lex();
				}
				break;

			case TOKEN_WORD:
			case TOKEN_SWORD:
				{
					if (token != TOKEN_INTEGER)
					{
						raise(Exception());
					}

					*(short*)data = xfile_lval.intValue;
					data += 2;

					token = xfile_lex();
				}
				break;

			case TOKEN_DWORD:
			case TOKEN_SDWORD:
				{
					if (token != TOKEN_INTEGER)
					{
						throw -1;
					}

					*(long*)data = xfile_lval.intValue;
					data += 4;

					token = xfile_lex();
				}
				break;

			case TOKEN_FLOAT:
				{
					if (token != TOKEN_FNUM)
					{
						throw -1;
					}

					*(float*)data = xfile_lval.floatValue;
					data += 4;

					token = xfile_lex();
				}
				break;

			case TOKEN_DOUBLE:
				{
					if (token != TOKEN_FNUM)
					{
						throw -1;
					}

					*(double*)data = xfile_lval.floatValue;
					data += 8;

					token = xfile_lex();
				}
				break;

			case TOKEN_CSTRING:
				{
					if (token != TOKEN_STRING)
					{
						throw -1;
					}

					String str = xfile_lval.pName;
					//str->AddRef();
					*(StringA**)data = dynamic_cast<StringA*>(str.m_stringObject);
					data += sizeof(StringA*);

					token = xfile_lex();
				}
				break;

			default:
				ASSERT(0);
				return NULL;
			}
		}
		break;

	case DataType::vector:
		{
			int dimsize;
			if (dataType->array_.dim->type == Dimension::Integer)
			{

				dimsize = dataType->array_.dim->num;
			}
			else
			{
				dimsize = *(uint32*)(pInstance->m_data + dataType->array_.dim->pMember->m_offset);//pInstance->m_members[dataType->array.dim->name]->m_intValue;
			}

			//DataValue* pData;
		//	pData  = new DataValue;//[dimsize];
		//	pData->m_arrayElements = new DataValue[dimsize];
			uint8* arrayData = new uint8[dataType->array_.type->m_sizeof * dimsize];

			*(void**)data = arrayData;
			data += sizeof(void*);

			for (int i = 0; i < dimsize; ++i)
			{
				if (i > 0)
				{
					if (token != TOKEN_COMMA)
						raise(Exception("Expected comma"));

					token = xfile_lex();
				}

				/*pData->m_arrayElements[i] = * ReadData(pInstance, dataType->array.type, token); */
				arrayData = ReadData(NULL, dataType->array_.type, arrayData, token);
			}
		}
		break;

	case DataType::Name:
		{
			dataType->template_name.pResolved = parser->m_templates[dataType->template_name.name];
		//	Template* pTemplate2 = dataType->template_name.pResolved;//parser->m_templates[dataType->name];
			Template* pTemplate2 = dataType->template_name.pResolved;//parser->m_templates[dataType->name];
		//	if (pTemplate == NULL)
		//		throw -1;

		//	DataValue* pData = new DataValue;

			Instance* pInstance2 = new Instance;
			pInstance2->m_pTemplate = pTemplate2;
		//	pData->m_object = pInstance2;

//			ReadData(pInstance2, token);

			ReadInstanceData(pInstance2, token);

			*(void**)data = pInstance2;
			data += sizeof(void*);
		}
		break;

	default:
		ASSERT(0);
		return NULL;
	}

	if (token == TOKEN_SEMICOLON)
	{
		token = xfile_lex();
	}

	return data;
}

/*
uint8* ReadData(Template* pTemplate, uint8* data, int& token)
{
	int nmembers = pInstance->m_pTemplate->m_members.size();
	for (int i = 0; i < nmembers; i++)
	{
		Member* pMember = pInstance->m_pTemplate->m_members[i];

		DataValue* pData = ReadData(pInstance, pMember->m_dataType, token);
	//	pData->m_pInstance = pInstance;
	//	pData->m_pMember = pMember;
		pInstance->m_members[pMember->m_name] = pData;

		//token = xfile_lex();
	}
}
*/

// TODO, not global
map<String, Instance*> m_namedObjects;

//extern "C"
//{
Instance* parse_object(DataType* /*const char* */ name) throw(Exception*)
{
	ASSERT(name->type == DataType::Name);

	Template* pTemplate = parser->m_templates[name->template_name.name];

	if (pTemplate == NULL)
		raise(Exception(String(name->template_name.name) + " template not found"));

	Instance* pInstance = new Instance;
	pInstance->m_pTemplate = pTemplate;

	int token = xfile_lex();
	if (token == TOKEN_NAME)
	{
		pInstance->m_name = xfile_lval.pName;
		token = xfile_lex();

		m_namedObjects.insert(map<String, Instance*>::value_type(pInstance->m_name, pInstance));
	}

	if (token != TOKEN_OBRACE)
		throw -1;

	token = xfile_lex();

//	pInstance->m_data = new uint8[pTemplate->m_sizeof];
//	DataType dataType;
//	dataType.type == DataType::Name;
//	dataType.template_name.pResolved = pTemplate;
	ReadInstanceData(pInstance, token);
	/*
	while (token != 0)
	{
		if (token == TOKEN_CBRACE)
			break;

		token = xfile_lex();
	}
	*/
	while (token == TOKEN_NAME || token == TOKEN_OBRACE)
	{
		if (token == TOKEN_NAME)
		{
			Instance* pChildInstance = parse_object(DataType_Name(xfile_lval.pName));
			pInstance->m_children.Add(pChildInstance);
		}
		else
		{
			token = xfile_lex();

			if (token != TOKEN_NAME)
			{
				throw -1;
			}

			Reference* pRef = new Reference;
			pRef->m_name = xfile_lval.pName;

			pInstance->m_references.Add(pRef);

			token = xfile_lex();
			// TODO optional class_id

			if (token != TOKEN_CBRACE)
				throw -1;
		}

		token = xfile_lex();
	}

	if (token != TOKEN_CBRACE)
		throw -1;

	return pInstance;
}

//}

struct yy_buffer_state;
yy_buffer_state* xfile__scan_string(const char* str);

int xfileparser::ParseX(Stream* stream)
{
	size_t size = stream->GetSize();

	const char* m_p;
	const char* m_buffer;
	size_t m_size;

	char signature[16];
	stream->Read(signature, 16);
	if (memcmp(signature, "xof 0302txt 0064", 16) == 0 ||
		memcmp(signature, "xof 0303txt 0032)", 16) == 0)
	{
		m_size = size-16;
		char* buffer = new char[m_size];

		stream->Read(buffer, m_size);

		m_buffer = buffer;
		m_p = buffer;
		while (isspace(*m_p))
		{
			m_p++;
		}

	//struct yy_buffer_state;
		xfile__scan_string(m_p);

		parser = this;//new xfileparser;
		xfile_lloc.first_line = 0;
		int failed = xfile_parse();

		/*
		parser.m_buffer = buffer;
		parser.m_p = buffer;
		parser.m_size = size;

		parser.parse();
		*/

		delete[] buffer;

		return 0;
	}

	return -1;
}
}	// DX

namespace Gui
{
namespace Media3D
{
/*

IMesh* Document3D::CreateMesh(uint nVertices, uint nFaces)
{
		ID3DX10Mesh* pMesh;

		D3D10_INPUT_ELEMENT_DESC declaration[6] =
		{
		};

		D3DX10CreateMesh(GL::Direct10::get_D3DDevice(),
			declaration,
			1,
			"POSITION",
			nVertices,
			nFaces,
			0,
			&pMesh);

}

Group* Document3D::CreateGroup(String name)
{
	if (pObject->m_name != NULL)
	{
		document->m_namedObjects[pGroup->m_name] = pGroup;
	}
}
*/

void convert(Document3D* document, const vector<DX::Instance*>& objects, Group* children);

Visual3D* convert(Document3D* document, DX::Instance* pObject)
{
	if (pObject->m_pTemplate->m_name == "AnimationSet")
	{
		for (size_t i = 0; i < pObject->m_children.GetSize(); ++i)
		{
			DX::Instance* pAnimation = pObject->m_children[i];
			if (pAnimation->m_pTemplate->m_name == "Animation")
			{
				DX::Instance* pInstanceRef = DX::m_namedObjects[pAnimation->m_references[0]->m_name];

				for (size_t j = 0; j < pAnimation->m_children.GetSize(); ++j)
				{
					DX::Instance* pAnimationKey = pAnimation->m_children[j];

					if (pAnimationKey->m_pTemplate->m_name == "AnimationKey")
					{
						MatrixAnimation* pAnimation2 = new MatrixAnimation;

						pAnimation2->m_Target = (Object3D*)pInstanceRef->m_userdata[StringIn("Object3D")];
						if (pAnimation2->m_Target == NULL)
							throw -1;

					//	DWORD keyType = pAnimationKey->m_members["keyType"]->m_intValue;
					//	DWORD nKeys = pAnimationKey->m_members["nKeys"]->m_intValue;

						uint32 keyType = pAnimationKey->GetValue("keyType");
						if (keyType != 4)
							throw -1;

						uint32 nKeys = pAnimationKey->GetValue("nKeys");

						DX::Instance** keys = *(DX::Instance***)(pAnimationKey->GetData("keys"));

						for (uint k = 0; k < nKeys; ++k)
						{
							DX::Instance* pTimedFloatKeys = keys[k];

							uint32 keytime = pTimedFloatKeys->GetValue("time");
							DX::Instance* tfkeys = *(DX::Instance**)pTimedFloatKeys->GetData("tfkeys");
							uint32 nValues = tfkeys->GetValue("nValues");
							const float* values = tfkeys->GetValue("values");

							MatrixAnimationKey* pKey2 = new MatrixAnimationKey;

							pKey2->m_time = keytime * 10.0 / 4960;	// ??
							//pKey2->m_value = LDraw::Matrix4(values);

							VERIFY(0);
							// It's probably not like this
							pKey2->m_value = gm::matrix4f(
								values[0], values[4], values[8], values[12],
								values[1], values[5], values[9], values[13],
								values[2], values[6], values[10], values[14],
								values[3], values[7], values[11], values[15]);

						//	pKey2->m_value->m_matrix = pKey2->m_value->m_matrix.getTransposed();

							if	(pKey2->m_value.isSingular())
							{
								MessageBoxA(NULL, "dxfile", "MatrixAnimationKey is singular", MB_OK);
							}

							pAnimation2->m_keys.Add(pKey2);
						}

						document->AddAnimation(pAnimation2);
					}
				}
			}
		}
	}
	else if (pObject->m_pTemplate->m_name == "Frame")
	{
		Group* pGroup = new Group;//document->CreateGroup(pObject->m_name);
		pGroup->set_ID(pObject->m_name);

	//	Object3DCollection* children = new Object3DCollection;

		pObject->m_userdata[StringIn("Object3D")] = pGroup;

		for (uint i = 0; i < pObject->m_children.GetSize(); i++)
		{
			DX::Instance* pChild = pObject->m_children[i];
			if (pChild->m_pTemplate->GetName() == "FrameTransformMatrix")
			{
			//	Instance* frameMatrix = *(Instance**)(pChild->GetData("frameMatrix"));
			//	const float* values = frameMatrix->GetValue("matrix");

			//	Instance* frameMatrix = *(Instance**)(pChild->GetData("frameMatrix"));

				const float* values = pChild->GetValue("frameMatrix")->GetValue("matrix");

				ASSERT(0);
				/*
				pGroup->SetTransform(LDraw::matrix4f(
								values[0], values[4], values[8], values[12],
								values[1], values[5], values[9], values[13],
								values[2], values[6], values[10], values[14],
								values[3], values[7], values[11], values[15]));
								*/
#if 0
				{
			float m[16];/* = {1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1};
								*/


			pGroup->m_transform->Get(m);

			ASSERT(m[0] == 1);
			ASSERT(m[1] == 0);
			ASSERT(m[2] == 0);
			ASSERT(m[3] == 0);

			ASSERT(m[4] == 0);
			ASSERT(m[5] == 1);
			ASSERT(m[6] == 0);
			ASSERT(m[7] == 0);

			ASSERT(m[8] == 0);
			ASSERT(m[9] == 0);
			ASSERT(m[10] == 1);
			ASSERT(m[11] == 0);

			ASSERT(m[12] == 0);
			ASSERT(m[13] == 0);
			ASSERT(m[14] == 0);
			ASSERT(m[15] == 1);
				}
#endif
			//	pGroup->m_transform->m_matrix = pGroup->m_transform->m_matrix.getTransposed();

				/*
				if (pGroup->m_transform->m_matrix.isSingular())
				{
					MessageBoxA(NULL, "dxfile", "Group transform matrix is singular", MB_OK);
				}
				*/

				break;
			}
		}

		convert(document, pObject->m_children, pGroup);

	//	pGroup->set_Children(children);
		return pGroup;
	}
	else if (pObject->m_pTemplate->m_name == "Mesh")
	{


		uint32 nVertices = pObject->GetValue("nVertices");
		DX::Instance** vertices = *(DX::Instance***)(pObject->GetData("vertices"));

	//	String string_x = ASTR("x");
	//	String string_y = ASTR("y");
	//	String string_z = ASTR("z");

		uint32 nFaces = pObject->GetValue("nFaces");

		/*
		UI::Media3D::Shape* pShape = new UI::Media3D::Shape;
		pShape->m_name = pObject->m_name;

		if (pShape->m_name != NULL)
		{
			document->m_namedObjects[pShape->m_name] = pShape;
		}

		pObject->m_userdata["Object3D"] = pShape;

		UI::Media3D::IndexedFaceSet* faceset = new UI::Media3D::IndexedFaceSet;
		pShape->set_Geometry(faceset);

		UI::Media3D::Point3DfCollection* points = new UI::Media3D::Point3DfCollection;
		Int32Collection* pointIndices = new Int32Collection;
		faceset->set_Points(points);
		ASSERT(0);
	//	faceset->set_PointIndices(pointIndices);
	*/

		Mesh* pMesh = new Mesh;

		struct VERTEX
		{
			gm::vector3f position;
			gm::vector3f normal;
			gm::vector2f texCoord;
		};

		D3D10_INPUT_ELEMENT_DESC declaration[3] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};
		uint declarationCount = 3;

		ASSERT(0);
#if 0
		HRESULT hr = D3DX10CreateMesh(Graphics::Direct10::get_D3DDevice(),
			declaration,
			declarationCount,
			declaration[0].SemanticName,
			nVertices,
			nFaces,
			D3DX10_MESH_32_BIT,
			&pMesh->m_pMesh);
		ASSERT(SUCCEEDED(hr));
#endif
	//	LDraw::vector3f* vertexPositions = new LDraw::vector3f[nVertices];

		VERTEX* vertexData = new VERTEX[nVertices];

		for (uint i = 0; i < nVertices; i++)
		{
			float x = vertices[i]->GetValue("x");
			float y = vertices[i]->GetValue("y");
			float z = vertices[i]->GetValue("z");

	//		vertexPositions[i] = LDraw::vector3f(x, y, z);

			vertexData[i].position = gm::vector3f(x, y, z);

		//	points->Add(LDraw::vector3f(x, y, z));
		}

		/*
		ID3D10Device *pDevice,
		  CONST D3D10_INPUT_ELEMENT_DESC *pDeclaration,
		  UINT DeclCount,
		  LPCSTR pPositionSemantic,
		  UINT VertexCount,
		  UINT FaceCount,
		  UINT Options,
		  ID3DX10Mesh **ppMesh
		  */

		DX::Instance** faces = *(DX::Instance***)(pObject->GetData("faces"));

		uint nindices = 0;

		for (uint i = 0; i < nFaces; i++)
		{
			DX::Instance* meshFace = faces[i];
			uint32 nFaceVertexIndices = meshFace->GetValue("nFaceVertexIndices");

			if (nFaceVertexIndices == 3)
			{
				nindices += 3;
			}
			else if (nFaceVertexIndices == 4)
			{
				nindices += 6;
			}
		}

		vector<uint32> indices;
		indices.reserve(nindices);

		for (uint i = 0; i < nFaces; i++)
		{
			DX::Instance* meshFace = faces[i];
			uint32 nFaceVertexIndices = meshFace->GetValue("nFaceVertexIndices");

			//ASSERT(nFaceVertexIndices == 3);

			const uint32* indices2 = meshFace->GetValue("faceVertexIndices");

			if (nFaceVertexIndices == 3)
			{
			//	DWORD* indices = *(DWORD**)(meshFace->GetData("faceVertexIndices"));

				for (uint i = 0; i < nFaceVertexIndices; i++)
				{
					indices.Add(indices2[i]);
				}
			}
			else if (nFaceVertexIndices == 4)
			{
				indices.Add(indices2[0]);
				indices.Add(indices2[1]);
				indices.Add(indices2[2]);
				indices.Add(indices2[2]);
				indices.Add(indices2[3]);
				indices.Add(indices2[0]);
			}
		}

		for (uint i = 0; i < pObject->m_children.GetSize(); i++)
		{
			DX::Instance* pChild = pObject->m_children[i];
			if (pChild->m_pTemplate->GetName() == "MeshNormals")
			{
				uint32 nNormals = pChild->GetValue("nNormals");

				if (nNormals != nVertices)
				{
					raise(Exception("nNormals != nVertices"));
				}

				uint32 nFaceNormals = pChild->GetValue("nFaceNormals");
				if (nFaceNormals != nFaces)
				{
					raise(Exception("nFaceNormals != nFaces"));
				}

			//	ASSERT(0);
#if 0
				UI::Media3D::Point3DCollection* normals = new UI::Media3D::Point3DCollection;
				Int32Collection* normalIndices = new Int32Collection;
				faceset->set_Normals(normals);
				faceset->set_NormalIndices(normalIndices);
#endif
				DX::Instance** normals = *(DX::Instance***)(pChild->GetData("normals"));

				for (uint i = 0; i < nNormals; ++i)
				{
					float x = normals[i]->GetValue("x");
					float y = normals[i]->GetValue("y");
					float z = normals[i]->GetValue("z");

					vertexData[i].normal = gm::vector3f(x, y, z);
				}

#if 0
				DX::Instance** faces = *(DX::Instance***)(pChild->GetData("faceNormals"));

				for (i = 0; i < nFaces; i++)
				{
					DX::Instance* meshFace = faces[i];
					uint32 nFaceVertexIndices = meshFace->GetValue("nFaceVertexIndices");

					if (nFaceVertexIndices == 3)
					{
					//	DWORD* indices = *(DWORD**)(meshFace->GetData("faceVertexIndices"));
						const DWORD* indices = meshFace->GetValue("faceVertexIndices");

						for (int i = 0; i < nFaceVertexIndices; i++)
						{
							normalIndices->m_items.Add(indices[i]);
						}
					}
				}
#endif
			}
			else if (pChild->m_pTemplate->GetName() == "MeshMaterialList")
			{
				uint32 nFaceIndexes = pChild->GetValue("nFaceIndexes");

				const uint32* faceIndex = pChild->GetValue("faceIndexes");

				for (uint i = 0; i < nFaceIndexes; i++)
				{
				//	pMesh->m_materialFaceIndices.push_back(faceIndex[i]);
				}

				for (uint i = 0; i < pChild->m_children.GetSize(); i++)
				{
					DX::Instance* pMaterial = pChild->m_children[i];

					if (pMaterial->m_pTemplate->GetName() == "Material")
					{
						Material* material = new Material;
					//	material->m_pTechnique = Mesh::m_effect->GetTechniqueByName("NoSpecular");
					//	ASSERT(material->m_pTechnique);

						DX::Instance* faceColor = pMaterial->GetValue("faceColor");
						material->set_DiffuseColor(new Color(faceColor->GetValue("red"),
						faceColor->GetValue("green"),
						faceColor->GetValue("blue")));
					//	material->m_diffuseColor[3] = faceColor->GetValue("alpha");
						material->m_alpha = 1.0f;

						pMesh->m_subsetMaterial.push_back(material);

						DX::Instance* textureFilename = pMaterial->FindDataOfType("TextureFilename");
						if (textureFilename)
						{
							material->m_textureFilename = textureFilename->GetValue("filename");
#if 0
							char buf[512];
							sprintf_s(buf, "C:/Documents and Settings/All Users/Documents/My Pictures/Sample Pictures/%s", filename->c_str());

						//	filename = *new StringA("C:/dxsdk/samples/Media/") + *filename;
							filename = string_copy(buf);//*new StringA("C:/Documents and Settings/All Users/Documents/My Pictures/Sample Pictures/") + *filename;

							Imaging::BitmapLoader loader;// = new Imaging::BitmapLoader;
							LDraw::Bitmap* bitmap = loader.Load(filename);

							material->m_textureBitmap = bitmap;
#endif
						}
					}
				}
			}
			else if (pChild->m_pTemplate->GetName() == "MeshTextureCoords")
			{
				uint32 nTextureCoords = pChild->GetValue("nTextureCoords");

				// Since the X format doesn't support texcoord indices, it must be the same as the vertex indices
				if (nTextureCoords != nVertices)
					raise(Exception("nTextureCoords != nVertices"));

				DX::Instance** textureCoords = *(DX::Instance***)(pChild->GetData("textureCoords"));

			//	Point3DfCollection* texCoords = new Point3DfCollection;
			//	faceset->set_TexCoords(texCoords);

			//	Int32Collection* texCoordIndices = new Int32Collection;
			//	faceset->set_TexCoordIndices(texCoordIndices);

				for (uint i = 0; i < nTextureCoords; ++i)
				{
					DX::Instance* textureCoord = textureCoords[i];

					float u = textureCoord->GetValue("u");
					float v = textureCoord->GetValue("v");

					v = -v;

					ASSERT(u >= 0 && u <= 1);
					ASSERT(v >= 0 && v <= 1);

					vertexData[i].texCoord = gm::vector2f(u, v);
				//	texCoords->m_items.Add(new Point3D(u, 1-v, 0));
				}

				/*
				// texture coordinate indices defalt to the same as the vertex coordinate indices
				for (uint i = 0; i < pointIndices->GetCount(); i++)
				{
					texCoordIndices->Add(pointIndices->get_Item(i));
				}
				*/
			}
			else if (pChild->m_pTemplate->GetName() == "SkinWeights")
			{
#if 0
				String transformNodeName = pChild->GetValue("transformNodeName");

				DX::Instance* pTransformNode = DX::m_namedObjects[transformNodeName];
				ASSERT(pTransformNode);

				Bone* pBone = new Bone;

				pBone->m_TransformNode = new Reference;//pTransformNode;

				pBone->m_IndexedFaceSet = faceset;
				pBone->m_TransformNode->m_name = transformNodeName;// = (Group*)pTransformNode->m_userdata["Object3D"];
				//ASSERT(pBone->m_pTransformNode);

				document->m_references.Add(pBone->m_TransformNode);

				uint32 nWeights = pChild->GetValue("nWeights");
				const uint32* vertexIndices = pChild->GetValue("vertexIndices");
				const float* weights = pChild->GetValue("weights");

				for (uint j = 0; j < nWeights; j++)
				{
					pBone->m_vertexWeights.Add(VertexWeight(vertexIndices[j], weights[j]));
				}

				DX::Instance* matrixOffset = *(DX::Instance**)(pChild->GetData("matrixOffset"));

				{
					const float* values = matrixOffset->GetValue("matrix");

					VERIFY(0);
				//	pBone->m_matrixOffset = LDraw::Matrix4(values);

					// It's probably the above instead of this
					pBone->m_matrixOffset = new LDraw::Matrix4(
									values[0], values[4], values[8], values[12],
									values[1], values[5], values[9], values[13],
									values[2], values[6], values[10], values[14],
									values[3], values[7], values[11], values[15]);

				//	pBone->m_matrixOffset->m_matrix = pBone->m_matrixOffset->m_matrix.getTransposed();

					if (pBone->m_matrixOffset->m_matrix.isSingular())
					{
						MessageBoxA(NULL, "dxfile", "matrixOffset is singular", MB_OK);
					}
				}

				faceset->m_Bones.Add(pBone);
#endif
			}
		}

		uint iBuffer = 0;
		pMesh->SetVertexData(iBuffer, vertexData);
		delete[] vertexData;

		pMesh->SetIndexData(indices.GetData(), indices.GetSize());
		//delete[] indices;

		// Reorder the vertices according to subset and optimize the mesh for this graphics 
		// card's vertex cache. When rendering the mesh's triangle list the vertices will 
		// cache hit more often so it won't have to re-execute the vertex shader.
		pMesh->m_pMesh->GenerateAdjacencyAndPointReps(1e-6f);
		pMesh->m_pMesh->Optimize(D3DX10_MESHOPT_ATTR_SORT | D3DX10_MESHOPT_VERTEX_CACHE, NULL, NULL);

		/*
		pMesh->m_pMesh->GetAttributeTable( NULL, &m_NumAttribTableEntries );
		m_pAttribTable = new D3DX10_ATTRIBUTE_RANGE[m_NumAttribTableEntries];
		pMesh->GetAttributeTable( m_pAttribTable, &m_NumAttribTableEntries);
		*/

		pMesh->m_pMesh->CommitToDevice();

		MeshContainer* mc = new MeshContainer;
		mc->m_mesh = pMesh;

		return mc;
	}

	return NULL;
}

void convert(Document3D* document, const vector<DX::Instance*>& objects, Group* children)
{
	for (int i = 0; i < objects.GetSize(); i++)
	{
		DX::Instance* pObject = objects[i];

		Visual3D* p = convert(document, pObject);
		if (p)
		{
			children->AddChild(p);
		}
	}
}

}	// Media3D
}	// Gui
}	// System
