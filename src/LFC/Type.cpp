#include "stdafx.h"
#include "LFC.h"
#include "Type.h"

#include "BitStream.h"

#include <Psapi.h>

namespace System
{

using namespace IO;

IO::TextWriter& GetQualifiedName(Scope* pScope, IO::TextWriter& strbuilder)
{
	if (pScope->m_pNamespace && pScope->m_pNamespace->AsClass())
	{
		pScope->m_pNamespace->AsClass()->Write(strbuilder);
	}
	else
	{
		if (pScope->m_pParentScope)
		{
			if (pScope->m_pParentScope->m_pNamespace == nullptr)	// template scope
			{
				ASSERT(pScope->m_pParentScope->m_pParentScope);

				strbuilder << "TemplateParam::";
				GetQualifiedName(pScope->m_pParentScope->m_pParentScope, strbuilder);
				strbuilder << "::";
			}
			else
			{
				if (pScope->m_pParentScope->m_pNamespace->m_name)
				{
					ASSERT(pScope->m_pParentScope->m_pNamespace);

					GetQualifiedName(pScope->m_pParentScope, strbuilder);
					strbuilder << "::";
				}
			}
		}

		if (pScope->m_pNamespace->m_name == nullptr)
			strbuilder << "(null)";
		else
			strbuilder << pScope->m_pNamespace->m_name;
	}

	/*
	strbuilder << pScope->m_name->c_str();

	if (pScope->m_pClass)
	{
		if (pScope->m_pClass->m_pInstantiatedFrom)
		{
			strbuilder << "<";

			for (int i = 0; i < pScope->m_pClass->m_pInstantiatedFrom->m_items.size(); i++)
			{
				pScope->m_pClass->m_pInstantiatedFrom->m_items[i]->Write(strbuilder);
			}
		}
	}
	*/

	return strbuilder;
}

NullType::NullType()
{
}

String NullType::ToString()
{
	return "NULL";
}

Type* NullType::Clone() const
{
	ASSERT(0);
	return NULL;
}

TextWriter& NullType::Write(TextWriter& stream) const
{
	stream << "decltype(nullptr)";
	return stream;
}

bool NullType::IsOfType(Type* pType) const
{
	return pType == this;
}

uint NullType::get_sizeof(uint sizeofptr) const
{
	return 0;
}

bool NullType::IsConst() const
{
	return false;
}

bool NullType::IsVolatile() const
{
	return false;
}

TypeSerializable::Class_Type NullType::GetSerializableType() const
{
	ASSERT(0);
	raise(Exception());
}

void NullType::Load(TypeArchive& stream)
{
	ASSERT(0);
}

void NullType::Store(TypeArchive& stream) const
{
	ASSERT(0);
}

NullType* Types::get_nullType()
{
	return &type_null;
}

// static
bool Type::IsInteger(const Type* pType)
{
	return System::IsInteger(pType->get_Kind());
}

String FunctionParameters::ToString() const
{
	ASSERT(0);
	return nullptr;

#if 0
	StringBuilderA str;

	str = "(";

	for (int i = 0; i < m_parameters.size(); i++)
	{
		__live_object_ptr<StringA> pstr = m_parameters[i]->ToString();
		if (i > 0)
			str.Append(",");
		str.Append(pstr->c_str());
	}

	str.Append(')');

	return str.DetachToString();
#endif
}

void TemplatedClassArgs::Load(TypeArchive& ar)
{
	unsigned short nargs;
	ar >> nargs;

	for (int i = 0; i < nargs; i++)
	{
		ActualTemplateArg* pArg = new ActualTemplateArg;
		ar >> pArg->m_pType;
		ar >> pArg->m_value;
		m_items.push_back(pArg);
	}
}

void TemplatedClassArgs::Store(TypeArchive& ar) const
{
#if 0
	ASSERT(m_pClass->m_pTemplateParams->m_items.size() >= m_items.size() ||
		m_pClass->m_pTemplateParams == 0);
#endif

	ar << (unsigned short)m_items.size();

	for (uint i = 0; i < m_items.size(); i++)
	{
		ar << m_items[i]->m_pType;
		ar << m_items[i]->m_value;
	}
}

#if 0
StringA* TemplatedClassType::ToString() const
{
	BufferImp<char> buffer;
	StringBuilderA strbuilder(&buffer);
	Write(strbuilder);
	return buffer.DetachToString();
//	__live_object_ptr<StringA> str = m_pClass->ToString();
//	str = *str + "<>";	// TODO
//	return str;
}

void TemplatedClassType::Write(StringBuilderA& stream) const
{
	m_pClass->Write(stream);

	if (m_pClass->m_items.size())
	{
		stream << "<";

		ASSERT(m_pClass->m_items.size() >= m_items.size() ||
			m_pClass->m_items.size() == 0);

		Type* pType = NULL;

		for (int i = 0; i < m_items.size(); i++)
		{
			if (i > 0) stream << ",";

			if (m_pClass->m_items[i]->m_kind == TemplateParameter::Param_Type)
			{
				pType = m_items[i]->m_pType;
				while (pType->GetKind() == type_typedef)
				{
					pType = ((Typedef*)pType)->m_pType;
				}

				pType->Write(stream);
			}
			else
			{
				pType = NULL;
				stream << m_items[i]->m_value;
			}
		}

		if (pType)
		{
			ASSERT(pType->GetKind() != type_templateinst);

			if (pType->GetKind() == type_class && pType->GetClass()->m_pInstantiatedFrom)
				stream << " >";
			else
				stream << ">";
		}
		else
		{
			stream << ">";
		}
	}
}
#endif

/*
Type* TemplatedClassType::Clone() const
{
	ASSERT(0);
	return NULL;
}
*/

IO::TextWriter& TemplatedClassArgs::Write(IO::TextWriter& stream) const
{
	ASSERT(0);
#if 0
	m_pClass->Write(stream);

	ASSERT(m_pClass->m_pTemplateParams);

	if (m_pClass->m_pTemplateParams)
	{
		stream << "<";

		ASSERT(m_pClass->m_pTemplateParams->m_items.size() >= m_items.size()/* ||
			m_pClass->m_items.size() == 0*/);

		Type* pType = NULL;

		for (int i = 0; i < m_items.size(); i++)
		{
			if (i > 0) stream << ",";

			if (m_pClass->m_pTemplateParams->m_items[i]->m_kind == TemplateParameter::Param_Type)
			{
				pType = m_items[i]->m_pType;
				while (pType->get_Kind() == type_typedef)
				{
					pType = ((Typedef*)pType)->m_pType;
				}

				pType->Write(stream);
			}
			else
			{
				pType = NULL;
				stream << m_items[i]->m_value;
			}
		}

		if (pType)
		{
			if (pType->get_Kind() == type_class && pType->GetClass()->m_pInstantiatedFrom)
				stream << " >";
			else
				stream << ">";
		}
		else
		{
			stream << ">";
		}
	}
#endif
	return stream;
}

///////////////////////////////////////
// Namespace

Namespace::Namespace()
{
}

TypeSerializable::Class_Type Namespace::GetSerializableType() const
{
	return Class_Namespace;
}

TypeSerializable::Class_Type TemplatedClassArg::GetSerializableType() const
{
	return Class_TemplatedClassArg;
}

// static
SExp* SExp::FromArchive(TypeArchive& ar, Kind kind)
{
	if (kind == SExp::SEXP_NULL)
	{
		return nullptr;
	}
	else if (kind == SExp::SEXP_INT)
	{
		int value;
		ar >> value;
		return new SValExp(typeid(int), value);
	}
	else if (kind == SExp::SEXP_ARG)
	{
		Declarator* decl = new Declarator;
		decl->Load(false, ar);
		return new SArgExp(decl);
	}
	else if (kind == SExp::SEXP_BINOP)
	{
		SExp* left = FromArchive(ar);
		SExp* right = FromArchive(ar);

		return new SBinopExp(left->m_pType, left, right);
	}
	else
	{
		ASSERT(0);
		return nullptr;
	}
}

void TemplatedClassArg::Load(TypeArchive& ar)
{
	SExp::Kind kind;
	ar >> *(ubyte*)&kind;
	if (kind == SExp::SEXP_TYPE)
	{
		ar >> m_pType;
	}
	else if (kind == SExp::SEXP_INT)
	{
		ar >> m_value;
	}
	else
	{
		m_exp = SExp::FromArchive(ar, kind);
	}
}

void TemplatedClassArg::Store(TypeArchive& ar) const
{
	if (m_exp)
	{
		ar << (byte)m_exp->GetKind();
		m_exp->Store(ar);
	}
	else if (m_pType)
	{
		ar << (byte)SExp::SEXP_TYPE;
		ar << m_pType;
	}
	else
	{
		ar << (byte)SExp::SEXP_INT;
		ar << m_value;
	}
}

_TemplateArgType::_TemplateArgType()
{
	m_pClass = nullptr;
	m_defaultArg = nullptr;
}

TypeSerializable::Class_Type _TemplateArgType::GetSerializableType() const
{
	return Class_TemplateArgType;
}

void _TemplateArgType::Load(TypeArchive& ar)
{
	NamedType::Load(ar);
	ar >> m_index;
	ar >> m_defaultArg;
}

void _TemplateArgType::Store(TypeArchive& ar) const
{
	NamedType::Store(ar);
	ar << m_index;
	ar << (Type*)m_defaultArg;
}

String _TemplateArgType::ToString()
{
	IO::StringWriter strbuilder;
	Write(strbuilder);
	return strbuilder.str();
}

IO::TextWriter& _TemplateArgType::Write(IO::TextWriter& stream) const
{
	stream << "<arg(";
	stream << m_name;
	stream << ")>";

	return stream;
}

Type* _TemplateArgType::Clone() const
{
	return nullptr;
}

bool TemplateParameter::Equals(const TemplateParameter& other) const
{
	if (m_kind != other.m_kind)
	{
		ASSERT(0);
		return false;
	}

	if (u.m_pType)
	{
		ASSERT(other.u.m_pType);

		ASSERT(u.m_pType == other.u.m_pType);

		//m_pType->Equals(other.m_pType);
	}
	else
		ASSERT(other.u.m_pType == nullptr);

	return true;
}

bool TemplateParams::Equals(const TemplateParams& other) const
{
	if (m_items.size() != other.m_items.size())
	{
		ASSERT(0);
		return false;
	}
	for (size_t i = 0; i < m_items.size(); ++i)
	{
		m_items[i]->Equals(*other.m_items[i]);
	}

	return true;
}

FunctionParameters::FunctionParameters()
{
	m_bVarArg = false;
}

///////////////////////////////////////////////////////////////
// _CVType

_CVType::_CVType(bool bConst, bool bVolatile) : m_bConst(bConst), m_bVolatile(bVolatile)
{
}

void _CVType::Load(TypeArchive& ar)
{
	uint8 flags;
	ar >> flags;

	m_bConst = (flags >> 0) & 1;
	m_bVolatile = (flags >> 1) & 1;
}

void _CVType::Store(TypeArchive& ar) const
{
	uint8 flags = 0;
	if (m_bConst) flags |= 1;
	if (m_bVolatile) flags |= 2;

	ar << flags;
}

/*
// static
TypeLib* TypeLib::CreateFromArchive(TypeArchive& ar, uint32 id)
{
	StringA* name = new StringA;
	ar >> *name;

	TypeLib* typelib;

	map<StringA*, TypeLib*, Ref_Less<StringA> >::iterator it = ar.m_typestuff->m_typelibs.find(name);
	if (it == ar.m_typestuff->m_typelibs.end())
	{
		typelib = new TypeLib;
		typelib->m_typeinfo_filename = name;
		ar.m_typestuff->m_typelibs.insert(map<StringA*, TypeLib*, Ref_Less<StringA> >::value_type(name, typelib));
	}
	else
	{
		typelib = (*it).second;
	}

	ar.m_objectmapLoad.insert(map<uint32, TypeSerializable*>::value_type(id, typelib));

	return typelib;
}
*/

Type* Type::GetPointerTo() const
{
	switch (get_Kind())
	{
	case type_pointer:
		return ((const PointerType*)this)->m_pPointerTo;

	case type_reference:
		return ((const ReferenceType*)this)->m_pPointerTo;

	case type_rvalue_reference:
		return ((const RValueReferenceType*)this)->m_pPointerTo;

//	case type_cv:
	//	return ((ModifierType*)this)->m_pPointerTo;

	default:
		return nullptr;
	}
}

FunctionType* Type::AsFunction()
{
	return nullptr;
}

_CVType::_CVType() :
	m_bConst(false),
	m_bVolatile(false)
{
}

Type::Type()
{
#if 0
	m_pTypeInfo = NULL;
#endif
//	m_pTypeInfoName = NULL;

	m_xsType = nullptr;
	m_javaType = nullptr;
	//m_comType = nullptr;
	m_readonly = false;

	m_pointerToThis = nullptr;
	m_referenceToThis = nullptr;
	m_constToThis = nullptr;
}

PointerType* Type::get_pointerToThis()
{
	if (m_pointerToThis == nullptr)
	{
		m_pointerToThis = new PointerType(this);
	}
	return m_pointerToThis;
}

ReferenceType* Type::get_referenceToThis()
{
	if (m_referenceToThis == nullptr)
	{
		m_referenceToThis = new ReferenceType(this);
	}
	return m_referenceToThis;
}

RValueReferenceType* Type::get_rvalueReferenceToThis()
{
	if (m_rvalueReferenceToThis == nullptr)
	{
		m_rvalueReferenceToThis = new RValueReferenceType(this);
	}
	return m_rvalueReferenceToThis;
}

ModifierType* Type::AddConst()
{
	if (m_constToThis == nullptr)
	{
		m_constToThis = new ModifierType(this, true, false);
	}
	return m_constToThis;
}

ModifierType* Type::AddVolatile()
{
	if (m_volatileToThis == nullptr)
	{
		m_volatileToThis = new ModifierType(this, false, true);
	}
	return m_volatileToThis;
}

bool Type::IsOfType(Type* pType) const
{
	if (this == pType) return true;

	if (get_Kind() != type_pointer) return false;
	if (pType->get_Kind() != type_pointer) return false;

	if (GetPointerTo()->get_Kind() != type_class) return false;
	if (pType->GetPointerTo()->get_Kind() != type_class) return false;

	return GetPointerTo()->AsClass()->IsOfType(pType->GetPointerTo()->AsClass());
}


#if 0
BOOL Type::IsCompatible(Type* pOther)
{
	if (m_type == type_pointer && pOther->m_type == type_pointer && pOther->m_pPointerTo->m_type == type_void)
	{
		return TRUE;
	}

	if (m_sizeof != pOther->m_sizeof)
		return FALSE;

	if (m_type != pOther->m_type)
		return FALSE;

	if (m_bConst != pOther->m_bConst)
		return FALSE;

	if (m_pClass == NULL && pOther->m_pClass != NULL)
		return FALSE;
	else if (m_pClass != NULL && pOther->m_pClass == NULL)
		return FALSE;

	if (m_pPointerTo == NULL && pOther->m_pPointerTo != NULL)
		return FALSE;
	else if (m_pPointerTo != NULL && pOther->m_pPointerTo == NULL)
		return FALSE;

	/* ??
	if (m_pClass)
	{
		if (!m_pClass->IsEqual(pOther->m_pClass))
			return FALSE;
	}
	*/

	if (m_pPointerTo)
	{
		if (!m_pPointerTo->IsCompatible(pOther->m_pPointerTo))
			return FALSE;
	}

	return TRUE;	// They're equal
//	return IsCompatible(pOther);
//	return IsEqual(pOther);
}
#endif

#if 0
BOOL Type::IsEqual(Type* pOther, BOOL bCheckQualification /*= TRUE */)
{
	if (m_type == type_typedef)
		return m_pType->IsEqual(pOther);

	if (pOther->m_type == type_typedef)
		return IsEqual(pOther->m_pType);

//////////////////////

	if (m_type != pOther->m_type)
		return FALSE;

	if (bCheckQualification)
	{
		if (m_bConst != pOther->m_bConst)
			return FALSE;
	}

	if (m_type == type_class)
	{
		if (!m_pClass->IsEqual(pOther->m_pClass))
			return FALSE;
	}
	else if (m_type == type_pointer)
	{
		if (!m_pPointerTo->IsEqual(pOther->m_pPointerTo))
			return FALSE;
	}

	if (m_sizeof != pOther->m_sizeof)
	{
		pPP->jerror("INTERNAL ERROR, sizeof differs");
	}
	return TRUE;	// They're equal
}
#endif

Type* Type::Normalized()
{
	return this;
}

//bool Type::Equals(Type* pOther, bool bConst/* = FALSE*/, bool bOtherConst/* = FALSE*/) const
bool Type::Equals(const Type& other) const
{
	VERIFY(this != NULL);
	VERIFY(&other != NULL);

	/*
	if (m_bConst)
		bConst = true;

	if (pOther->m_bConst)
		bOtherConst = true;
		*/
	Type_type kind = get_Kind();
	Type_type otherkind = other.get_Kind();

	if (kind == type_cv)
		return GetPointerTo()->Equals(other/*, bConst, bOtherConst*/);

	if (otherkind == type_cv)
		return Equals(*(Type*)other.GetPointerTo()/*, bConst, bOtherConst*/);

	if (kind == type_typedef)
		return ((Typedef*)this)->m_pType->Equals(other/*, bConst, bOtherConst*/);

	if (otherkind == type_typedef)
		return Equals(*(Type*)((Typedef*)&other)->m_pType/*, bConst, bOtherConst*/);

//////////////////////

	if (kind != otherkind)
		return false;

	/*
	//if (bCheckQualification)
	{
		if (bConst != bOtherConst)
			return false;
	}
	*/

	if (kind == type_class)
	{
		if (!((ClassType*)this)->Equals(*(ClassType*)&other))
			return false;
	}
	else if (kind == type_pointer)
	{
		if (!((PointerType*)this)->Equals(*((PointerType*)&other)))
			return false;
	}
	else if (kind == type_reference)
	{
		if (!((ReferenceType*)this)->Equals(*((ReferenceType*)&other)))
			return false;
	}
	else if (kind == type_function)
	{
		ASSERT(0);	// Done in override
//		if (!((FunctionType*)this)->Equals(*(FunctionType*)&other))
	//		return false;
	}
	else if (kind == type_enum)
	{
		if (!((EnumType*)this)->Equals((EnumType*)&other))
			return false;
	}

	// TODO array

	return true;	// They're equal
}

/*
Type* Type::Normalized()
{
	if (m_type == type_cv)
	{
		if (!m_bConst && !m_bVolatile)
			return GetPointerTo()->Normalized();
	}

	return this;
}
*/

// virtual
Type* Type::GetBaseType()	// overriden in EnumType
{
	return GetStripped();
}

// virtual
Type* Type::GetStripped()
{
	return this;
}

// virtual
ClassType* Type::AsClass()
{
	Type_type kind = get_Kind();

//	if (kind == type_typedef) return static_cast<Typedef*>(this)->m_pType->AsClass();
	if (kind == type_cv) return static_cast<ModifierType*>(this)->m_pPointerTo->AsClass();

	return nullptr;
}

// virtual
uint _TemplateArgType::get_sizeof(uint sizeofptr) const
{
	return ~0;
}

/*
uint TemplatedClassType::get_sizeof() const
{
	ASSERT(0);
	return -1;
}
*/

LFCEXT ImmutableString<char> operator_gt("operator>");
LFCEXT ImmutableString<char> operator_lt("operator<");
LFCEXT ImmutableString<char> operator_eq("operator==");
LFCEXT ImmutableString<char> operator_neq("operator!=");
LFCEXT ImmutableString<char> operator_geq("operator>=");
LFCEXT ImmutableString<char> operator_leq("operator<=");
LFCEXT ImmutableString<char> operator_plus("operator+");
LFCEXT ImmutableString<char> operator_minus("operator-");
LFCEXT ImmutableString<char> operator_assign("operator=");
LFCEXT ImmutableString<char> operator_plus_assign("operator+=");
LFCEXT ImmutableString<char> operator_minus_assign("operator-=");
LFCEXT ImmutableString<char> operator_and_assign("operator&=");
LFCEXT ImmutableString<char> operator_or_assign("operator|=");
LFCEXT ImmutableString<char> operator_xor_assign("operator^=");

////////////////////////////////////////////////
// Predefined Types

// Primitive types

// static
PrimitiveType* Types::get_void()
{
	return &type_void;
}

// static
PrimitiveType* Types::get_bool()
{
	return &type_bool;
}

// static
PrimitiveType* Types::get_char()
{
	return &type_char;
}

// static
PrimitiveType* Types::get_signed_char()
{
	return &type_signed_char;
}

// static
PrimitiveType* Types::get_unsigned_char()
{
	return &type_unsigned_char;
}

// static
PrimitiveType* Types::get_wchar()
{
	return &type_wchar;
}

// static
PrimitiveType* Types::get_short()
{
	return &type_short;
}

// static
PrimitiveType* Types::get_unsigned_short()
{
	return &type_unsigned_short;
}

// static
PrimitiveType* Types::get_long()
{
	return &type_long;
}

// static
PrimitiveType* Types::get_unsigned_long()
{
	return &type_unsigned_long;
}

// static
PrimitiveType* Types::get_int()
{
	return &type_int;
}

// static
PrimitiveType* Types::get_unsigned_int()
{
	return &type_unsigned_int;
}

// static
PrimitiveType* Types::get_long_long()
{
	return &type_long_long;
}

// static
PrimitiveType* Types::get_unsigned_long_long()
{
	return &type_unsigned_long_long;
}

// static
PrimitiveType* Types::get_float()
{
	return &type_float;
}

// static
PrimitiveType* Types::get_double()
{
	return &type_double;
}

// static
PrimitiveType* Types::get_long_double()
{
	return &type_long_double;
}

NullType Types::type_null;
PrimitiveType Types::type_void(System::type_void);
PrimitiveType Types::type_bool(System::type_bool);
PrimitiveType Types::type_char(System::type_char);
PrimitiveType Types::type_wchar(System::type_wchar);
PrimitiveType Types::type_signed_char(System::type_signed_char);
PrimitiveType Types::type_unsigned_char(System::type_unsigned_char);
PrimitiveType Types::type_int(System::type_int);
PrimitiveType Types::type_unsigned_int(System::type_unsigned_int);
PrimitiveType Types::type_short(System::type_short);
PrimitiveType Types::type_unsigned_short(System::type_unsigned_short);
PrimitiveType Types::type_long(System::type_long);
PrimitiveType Types::type_unsigned_long(System::type_unsigned_long);
PrimitiveType Types::type_long_long(System::type_long_long);
PrimitiveType Types::type_unsigned_long_long(System::type_unsigned_long_long);
PrimitiveType Types::type_float(System::type_float);
PrimitiveType Types::type_double(System::type_double);
PrimitiveType Types::type_long_double(System::type_long_double);
PrimitiveType Types::type_float128(System::type_float128);

void CDefine::Load(TypeArchive& ar)
{
	m_name = ar.LoadIdentifier();

	ar >> m_bFunctionLike;

	uint nargs;
	ar >> nargs;
	m_argNames.resize(nargs);
	for (uint i = 0; i < nargs; ++i)
	{
		m_argNames[i] = ar.LoadIdentifier();
	}

	uint n;
	ar >> n;
	m_replacementList.resize(n);
	for (uint i = 0; i < n; ++i)
	{
		CToken* token;
		int kind;
		ar >> kind;
		switch (kind)
		{
		case CToken::tok_space:
			{
				token = new SpaceToken;
			}
			break;

		case CToken::tok_integer:
			{
				token = new IntegerToken;
				ar >> token->intval;
			}
			break;

		case CToken::tok_double:
			{
				token = new DoubleToken;
				ar >> token->double64;
			}
			break;

		case CToken::tok_string_literal:
			{
				token = new StringToken;
				String str;
				ar >> str;
				token->m_astr = dynamic_cast<StringA*>(str.m_stringObject);
				ASSERT(token->m_astr);
				token->m_astr->IncRef();
			}
			break;

		case CToken::tok_ident:
			{
				token = new IdentToken;
				token->m_astr = dynamic_cast<StringA*>(ar.LoadIdentifier().m_stringObject);
				token->m_astr->IncRef();
			}
			break;

		case CToken::tok_symbol:
			{
				token = new SymbolToken;
				ar >> token->symbol;
			}
			break;

		case CToken::tok_char_literal:
			{
				token = new CharLiteralToken;
				ar >> token->intval;
			}
			break;

		case CToken::tok_char:
			{
				token = new CharToken;
				ar >> token->symbol;
			}
			break;

		default:
			VERIFY(0);
		}

		m_replacementList[i] = token;
	}
}

void CDefine::Store(TypeArchive& ar) const
{
	ar.StoreIdentifier(m_name);

	ar << m_bFunctionLike;

	ar << m_argNames.size();
	for (uint i = 0; i < m_argNames.size(); i++)
	{
		ar.StoreIdentifier(m_argNames[i]);
	}

	ar << m_replacementList.size();
	for (uint i = 0; i < m_replacementList.size(); i++)
	{
		m_replacementList[i]->Store(ar);
	}
}

bool CDefine::Equals(CDefine* pOther)
{
	ASSERT(m_name == pOther->m_name);	// Should already have been checked against this

	if (m_bFunctionLike != pOther->m_bFunctionLike)
		return false;

	if (m_argNames.size() != pOther->m_argNames.size())
		return false;

	if (m_replacementList.size() != pOther->m_replacementList.size())
		return false;

	for (uint i = 0; i < m_replacementList.size(); i++)
	{
		if (m_replacementList[i]->GetKind() != pOther->m_replacementList[i]->GetKind())
			return false;

		// TODO CToken::Equals ??

		if (m_replacementList[i]->GetKind() == CToken::tok_ident || pOther->m_replacementList[i]->GetKind() == CToken::tok_string_literal)
		{
			if (String(m_replacementList[i]->m_astr) != String(pOther->m_replacementList[i]->m_astr))
				return false;
		}
	}

	return true;	// They're equal
}

void CToken::Store(TypeArchive& ar)
{
	ar << GetKind();

	switch (GetKind())
	{
	case tok_space:
		break;

	case tok_char_literal:
		ar << intval;
		break;

	case tok_ltstring:
	case tok_string_literal:
		ASSERT(0);
		//ar << *m_astr;
		break;

	case tok_ident:
		ar.StoreIdentifier(m_astr);
		break;

	case tok_integer:
		ar << intval;
		break;

	case tok_char:
	case tok_symbol:
		ar << symbol;
		break;

	case tok_double:
		ar << double64;
		break;

	default:
		VERIFY(0);
	}
}

String CToken::toString() const
{
	int type = GetKind();

	if (type == tok_space)
	{
		return " ";
	}
	else if (type == tok_string_literal)
	{
		return m_astr;
	}
	else if (type == tok_char_literal)
	{
		return m_astr;
	}
	else if (type == tok_ident)
	{
		return m_astr;
	}
	else if (type == tok_integer)
	{
		char buf[64];
		sprintf_s(buf, "%d", intval);
		return string_copy(buf);
	}
	else if (type == tok_symbol)
	{
		if (symbol > 255)
		{
			if (symbol > 65535)
			{
				IO::StringWriter strbuilder;

				{
					char c2[2] = {(symbol>>16)&0xff, 0};
					strbuilder << c2;
				}

				{
					char c2[2] = {(symbol>>8)&0xff, 0};
					strbuilder << c2;
				}

				{
					char c2[2] = {(symbol)&0xff, 0};
					strbuilder << c2;
				}

				return strbuilder;
			}
			else
			{
				IO::StringWriter strbuilder;

				{
					char c2[2] = {(symbol>>8)&0xff, 0};
					strbuilder << c2;
				}

				{
					char c2[2] = {(symbol)&0xff, 0};
					strbuilder << c2;
				}

				return strbuilder;
			}
		}
		else
		{
			char c2[2] = {(char)symbol, 0};
			return string_copy(c2);
		}
	}
	else if (type == tok_char)
	{
		char c2[2] = {(char)symbol, 0};
		return string_copy(c2);
	}
	else
	{
		ASSERT(0);
	}
	return nullptr;
}

IO::TextWriter& CToken::Write(IO::TextWriter& strbuilder) const
{
	int type = GetKind();

	if (type == tok_space)
	{
		strbuilder << " ";
	}
	else if (type == tok_string_literal)
	{
		strbuilder << String(m_astr);
	}
	else if (type == tok_char_literal)
	{
		strbuilder << String(m_astr);
	}
	else if (type == tok_ident)
	{
		strbuilder << String(m_astr);
	}
	else if (type == tok_integer)
	{
	//	char buf[64];
	//	sprintf(buf, "%d", int32);
	//	return new System::StringA(buf);
		strbuilder << (unsigned int)intval;
	}
	else if (type == tok_symbol)
	{
		if (symbol > 255)
		{
			if (symbol > 65535)
			{
				//System::StringBuilderA strbuilder;

				{
					char c2[2] = {(symbol>>16)&0xff, 0};
					strbuilder << c2;
				}

				{
					char c2[2] = {(symbol>>8)&0xff, 0};
					strbuilder << c2;
				}

				{
					char c2[2] = {(symbol)&0xff, 0};
					strbuilder << c2;
				}

				//return strbuilder.DetachToString();
			}
			else
			{
				//System::StringBuilderA strbuilder;//__live_object_ptr<System::StringA> str;

				{
					char c2[2] = {(symbol>>8)&0xff, 0};
					strbuilder << c2;
				}

				{
					char c2[2] = {(symbol)&0xff, 0};
					strbuilder << c2;
				}

				//return strbuilder.DetachToString();
			}
		}
		else
		{
		//	char c2[2] = {(char)symbol, 0};
		//	return new System::StringA(c2);
			strbuilder << (char)symbol;
		}
	}
	else if (type == tok_char)
	{
	//	char c2[2] = {(char)symbol, 0};
	//	return new System::StringA(c2);
		strbuilder << (char)symbol;
	}
	else
	{
		char buf[512];

		sprintf_s(buf, "%f", double64);
		strbuilder << buf;

		//ASSERT(0);
	}
//	return new System::StringA();

	return strbuilder;
}

/*
uint8 TypeLibImport::GetSerializableType() const
{
	return Class_TypeLibImport;
}
*/
/*
void TypeLibImport::Load(TypeArchive& ar)
{
}
*/

#if 0
void TypeLibImport::Load(TypeArchive& ar)
//void TypeLibImport::CreateFromArchive(TypeArchive& ar, uint32 id)
{
	m_typeinfo_filename = new StringA;
	ar >> *m_typeinfo_filename;

	map<StringA*, TypeLib*, Ref_Less<StringA> >::iterator it = ar.m_typestuff->m_typelibs.find(m_typeinfo_filename);
	if (it == ar.m_typestuff->m_typelibs.end())
	{
		m_typelib = new TypeLib;
		m_typelib->m_typeinfo_filename = m_typeinfo_filename;
		ar.m_typestuff->m_typelibs.insert(map<StringA*, TypeLib*, Ref_Less<StringA> >::value_type(m_typeinfo_filename, m_typelib));

		char fullname[260];
		_makepath(fullname, "C:", "\\mmstudio\\Win32\\bin\\debug", m_typeinfo_filename->c_str(), "typeinfo");

		IO::FileByteStream file(new StringA(fullname), IO::FileMode_Read);
		TypeArchive ar2(TypeArchive::Mode_Load, &file);
		ar2.m_pGlobalNamespace = ar.m_pGlobalNamespace;
		ar2.m_bSortedDecls = ar.m_bSortedDecls;
		ar2.m_typestuff = ar.m_typestuff;

		ar2.m_typelib = this;//new TypeLibImportm_typelib;
		ar2.MapObject(ar2.m_typelib);
		m_typelib->Load(ar2);
	}
	else
	{
		m_typelib = (*it).second;
	}
}

void TypeLibImport::Store(TypeArchive& ar) const
{
	VERIFY(m_typeinfo_filename);
	ar << *m_typeinfo_filename;
}
#endif

/*
FilePart* TypeStuff::GetFilePart(StringIn parentname)
{
	map<String, FilePart*>::iterator it = ar.m_typestuff->m_files.find(parentname);
	ASSERT(it != ar.m_typestuff->m_files.end());

	part->m_parent = it->second->m_children.insert(it->second->m_children.end(), part);
	part->m_fullname = (*part->m_parent)->m_name + "/" + part->m_name;

	return part;
}
*/

/*
FilePart* TypeLib::LoadFilePart(TypeArchive& ar)
{
	String name;
	ar >> name;

	FilePart* part = new FilePart;
	ar.MapObject(part);
	part->m_name = name;

	String parentname;
	ar >> parentname;

	if (parentname != NULL)
	{
		part->m_parent = ar.m_typestuff->GetFilePart(parentname);
	}

	return part;
}
*/

int CppSourceFile::Generate()
{
	return 0;
}

LFCEXT IO::TextWriter& WriteToStream(IO::TextWriter& stream, CppSourceFile* file)
{
	stream << file->ToString();
	return stream;
}

LFCEXT IO::TextWriter& WriteToStream(IO::TextWriter& stream, ClassKey classKey)
{
	switch (classKey)
	{
	case ClassKey_class:	stream << "class"; break;
	case ClassKey_struct:	stream << "struct"; break;
	case ClassKey_union:	stream << "union"; break;
	case ClassKey_interface: stream << "interface"; break;
	}

	return stream;
}

}	// System
