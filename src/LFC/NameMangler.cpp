#include "stdafx.h"
#include "LFC.h"
#include "NameMangler.h"

namespace System
{

static const char* base36 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

Mangler::Mangler(bool bPreserveTypedefs) : m_bPreserveTypedefs(bPreserveTypedefs), m_dict(less<Object*>(), grow_allocator(m_buffer, 2048))
{
}

// static
void Mangler::Base36(IO::TextWriter& strbuilder, int num)
{
	if (num == 0)
	{
		strbuilder << "S_";

		//return new StringA("S_");
	}
	else
	{
		// TODO

		ASSERT((num-1) < 36);

		char s[32];
		s[0] = 'S';

		s[1] = base36[(num-1)];
		s[2] = '_';
		s[3] = 0;
		//return new StringA(s);
		strbuilder << s;
	}
}

void Mangler::MangleName(Scope* pScope, IO::TextWriter& strbuilder)
{
	map<Object*, int, less<Object*>, grow_allocator>::iterator it = m_dict.find(pScope);
	if (it != m_dict.end())
	{
		Base36(strbuilder, (*it).second);
		return;
	}

	//char component[512];
	//sprintf(component, "%d%s", pScope->m_name->Length(), pScope->m_name->c_str());

	if (pScope->m_pNamespace->AsClass())
	{
		if (pScope->m_pNamespace->AsClass()->m_pInstantiatedFromArgs)
		{
			ASSERT(pScope->m_pNamespace->AsClass()->m_pInstantiatedFromClass->m_pInstantiatedFromClass == nullptr);
			//ASSERT(pScope->m_pClass->m_pOwnerScope == pScope->m_pClass->m_pInstantiatedFromClass->m_pOwnerScope);

			MangleName(pScope->m_pNamespace->AsClass()->m_pInstantiatedFromClass->m_pScope, strbuilder);

			if (pScope->m_pNamespace->AsClass()->m_pInstantiatedFromClass->m_pTemplateParams)
			{
				/*
				map<Object*, int, less<Object*>, grow_allocator>::iterator it = m_dict.find(pScope);
				if (it != m_dict.end())
				{
					Base36(strbuilder, (*it).second);
					return;
				}
				*/

				strbuilder << "I";

				ClassType* pInstantiatedFromClass = pScope->m_pNamespace->AsClass()->m_pInstantiatedFromClass;
				TemplatedClassArgs* pInstantiatedFromArgs = pScope->m_pNamespace->AsClass()->m_pInstantiatedFromArgs;
				ASSERT(pInstantiatedFromClass->m_pTemplateParams->m_items.size() >= pInstantiatedFromArgs->m_items.size());

				for (size_t i = 0; i < pInstantiatedFromArgs->m_items.size(); i++)
				{
					TemplatedClassArg* pArg = pInstantiatedFromArgs->m_items[i];

					if (pInstantiatedFromClass->m_pTemplateParams->m_items[i]->m_kind == TemplateParameter::Param_Type)
					{
						MangleType(pArg->m_pType, strbuilder);
					}
					else
					{
						strbuilder << 'L';	// literal

						strbuilder << 'b';	// type (bool)

						int value = pArg->m_value;
						if (value < 0)
						{
							strbuilder << 'n';
							value = -value;
						}
						strbuilder << value;

						strbuilder << 'E';
					}
				}

				strbuilder << "E";

				m_dict.insert(map<Object*, int>::value_type(pScope, m_dict.size()));
			}
		}
		else
		{
			if (pScope->m_pParentScope && pScope->m_pParentScope->m_pNamespace->m_name)
			{
				MangleName(pScope->m_pParentScope, strbuilder);
			}

			m_dict.insert(map<Object*, int>::value_type(pScope, m_dict.size()));

			if (pScope->m_pNamespace->m_name)
			{
				strbuilder << pScope->m_pNamespace->m_name.GetLength();
				strbuilder << pScope->m_pNamespace->m_name;
			}
			else
			{
				strbuilder << 9;
				strbuilder << "__unnamed";
			}
		}
	}
	else
	{
		if (pScope->m_pParentScope && pScope->m_pParentScope->m_pNamespace->m_name)
		{
			MangleName(pScope->m_pParentScope, strbuilder);
		}

		m_dict.insert(map<Object*, int>::value_type(pScope, m_dict.size()));

		strbuilder << pScope->m_pNamespace->m_name.GetLength();
		strbuilder << pScope->m_pNamespace->m_name;
	}
}

void Mangler::MangleClassType(ClassType* pType, IO::TextWriter& strbuilder)
{
	if (pType->m_ownerScope && pType->m_ownerScope->m_pNamespace->m_name)
	{
		strbuilder << "N";
	}

	MangleName(pType->m_pScope, strbuilder);

	if (pType->m_ownerScope && pType->m_ownerScope->m_pNamespace->m_name)
	{
		strbuilder << "E";
	}
}

void Mangler::MangleEnumType(EnumType* pType, IO::TextWriter& strbuilder)
{
	if (pType->m_ownerScope && pType->m_ownerScope->m_pNamespace->m_name)
	{
		strbuilder << "N";

		MangleName(pType->m_ownerScope, strbuilder);

	//	char component[512];
	//	sprintf(component, "%d%s", pType->m_name->Length(), pType->m_name->c_str());
	//	strbuilder << component;
		strbuilder << pType->m_name.GetLength();
		strbuilder << pType->m_name;

		strbuilder << "E";
	}
	else
	{
//		char component[512];
//		sprintf(component, "%d%s", pType->m_name->Length(), pType->m_name->c_str());
//		strbuilder << component;
		strbuilder << pType->m_name.GetLength();
		strbuilder << pType->m_name;
	}
}

#if 0
void Mangler::MangleNamedType(__live_object_ptr<NamedType> pNamedType, StringBuilderA & strbuilder)
{
	if (pNamedType->m_pOwnerScope)
	{
		strbuilder << "N";

	//	char refname[512];
	//	refname[0] = 0;

	//	char name[512];
	//	name[0] = 0;
		MangleName( pNamedType->m_pOwnerScope, strbuilder);

//		name << pNamedType->m_name.Length();
//		name += pNamedType->m_name.c_str();

		char component[512];
		sprintf(component, "%d%s", pNamedType->m_name->Length(), pNamedType->m_name->c_str());

		strbuilder << component;

#if 0
		strcat(name, component);

		map<char*,int, cstr_less>::iterator it = m_dict.find(name);
		if (it != m_dict.end())
		{
			Base36(name, (*it).second);
			return;
		}

	//	printf("Inserting %s\n", name.c_str());
		m_dict.insert(map<StringA*,int, Ref_Less<StringA> >::value_type(name, m_dict.size()));

		refname = *refname + *component;

		return *("N" + *refname) + "E";
#endif
		strbuilder << "E";
	}
	else
	{
		char component[512];
		sprintf(component, "%d%s", pNamedType->m_name->Length(), pNamedType->m_name->c_str());

		strbuilder << component;

#if 0
		//__live_object_ptr<StringA> name;

		char* name = mname+strlen(mname);
		sprintf(name, "%d%s", pNamedType->m_name->Length(), , pNamedType->m_name->c_str());

		map<StringA*, int, Ref_Less<StringA> >::iterator it = m_dict.find(name);
		if (it != m_dict.end())
		{
			return Base36((*it).second);
		}

	//	printf("Inserting %s\n", name.c_str());
		m_dict.insert(map<StringA*, int, Ref_Less<StringA> >::value_type(name, m_dict.size()));

		return name;
#endif
	}
}
#endif

IO::TextWriter& Mangler::MangleType(Type* pType, IO::TextWriter& strbuilder)
{
	switch (pType->get_Kind())
	{
	case type_pointer:
		{
			map<Object*, int, less<Object*>, grow_allocator>::iterator it = m_dict.find(pType);
			if (it != m_dict.end())
			{
				Base36(strbuilder, (*it).second);
				return strbuilder;
			}

			PointerType* pPointer = (PointerType*)pType;
			
			strbuilder << "P";
			MangleType(pType->GetPointerTo(), strbuilder);

			m_dict.insert(map<Object*, int>::value_type(pType, m_dict.size()));
		}
		break;
		
	case type_reference:
		{
			map<Object*, int, less<Object*>, grow_allocator>::iterator it = m_dict.find(pType);
			if (it != m_dict.end())
			{
				Base36(strbuilder, (*it).second);
				return strbuilder;
			}

			ReferenceType* pPointer = (ReferenceType*)pType;
			
			strbuilder << "R";
			MangleType(pType->GetPointerTo(), strbuilder);

			m_dict.insert(map<Object*, int>::value_type(pType, m_dict.size()));
		}
		break;
		
	case type_cv:
		{
			map<Object*, int, less<Object*>, grow_allocator>::iterator it = m_dict.find(pType);
			if (it != m_dict.end())
			{
				Base36(strbuilder, (*it).second);
				return strbuilder;
			}

			ModifierType* pCV = (ModifierType*)pType;
			
			if (pCV->m_bVolatile) strbuilder << "V";
			if (pCV->m_bConst) strbuilder << "K";
			
			MangleType(pType->GetPointerTo(), strbuilder);

			m_dict.insert(map<Object*, int>::value_type(pType, m_dict.size()));
		}
		break;
		
	case type_array:
		{
			map<Object*, int, less<Object*>, grow_allocator>::iterator it = m_dict.find(pType);
			if (it != m_dict.end())
			{
				Base36(strbuilder, (*it).second);
				return strbuilder;
			}

			ArrayType* pArrayType = (ArrayType*)pType;

			strbuilder << "A";

			// positive dimension number
			strbuilder << pArrayType->m_nElemCount;

			strbuilder << "_";

			MangleType(pArrayType->m_pElemType, strbuilder);

			m_dict.insert(map<Object*, int>::value_type(pType, m_dict.size()));
		}
		break;
		
	case type_class:
		{
			MangleClassType((ClassType*)pType, strbuilder);
		}
		break;
		
	case type_enum:
		{
			MangleEnumType((EnumType*)pType, strbuilder);
		}
		break;
		
	case type_typedef:
		{
			if (m_bPreserveTypedefs)
			{
				NamedType* pType2 = (NamedType*)pType;

				if (pType2->m_ownerScope)
				{
					strbuilder << "N";

					MangleName(pType2->m_ownerScope, strbuilder);

				//	char component[512];
				//	sprintf(component, "%d%s", pType->m_name->Length(), pType->m_name->c_str());
				//	strbuilder << component;
					strbuilder << pType2->m_name.GetLength();
					strbuilder << pType2->m_name;

					strbuilder << "E";
				}
				else
				{
			//		char component[512];
			//		sprintf(component, "%d%s", pType->m_name->Length(), pType->m_name->c_str());
			//		strbuilder << component;
					strbuilder << pType2->m_name.GetLength();
					strbuilder << pType2->m_name;
				}
			}
			else
			{
				MangleType(((Typedef*)pType)->m_pType, strbuilder);
			}
		}
		break;
		
	case type_templatearg:
		{
			// ???
			strbuilder << "<arg>";
		}
		break;

		/*
	case type_templateinst:
		{
			// ???
			strbuilder << "<>";
		}
		break;
		*/

	case type_function:
		{
			// TODO
			strbuilder << "function";
		}
		break;

	default:
		{
			PrimitiveType* pPrimitive = (PrimitiveType*)pType;
			
			//mname = new StringA();
			
			switch (pType->get_Kind())
			{
			case type_void:						strbuilder << "v";	break;
			case type_bool:						strbuilder << "b";	break;
			case type_char:						strbuilder << "c";	break;
			case type_signed_char:				strbuilder << "a";	break;
			case type_unsigned_char:			strbuilder << "h";	break;
			case type_wchar:					strbuilder << "w";	break;
			case type_int:						strbuilder << "i";	break;
			case type_short:					strbuilder << "s";	break;
			case type_long:						strbuilder << "l";	break;
			case type_unsigned_int:				strbuilder << "j";	break;
			case type_unsigned_short:			strbuilder << "t";	break;
			case type_unsigned_long:			strbuilder << "m";	break;
			case type_long_long:				strbuilder << "x";	break;
			case type_unsigned_long_long:		strbuilder << "y";	break;
			case type_int128:					strbuilder << "n";	break;
			case type_unsigned_int128:			strbuilder << "o";	break;
			case type_float:					strbuilder << "f";	break;
			case type_double:					strbuilder << "d";	break;
			case type_long_double:				strbuilder << "e";	break;
			case type_float128:					strbuilder << "g";	break;
				
			default:
				DebugTrace("Unknown type: " << (int)pType->get_Kind() << '\n');
				ASSERT(0);
			}
		}
	}

	return strbuilder;
}

void Mangler::MangleFunctionParameters(const FunctionParameters& parameters, IO::TextWriter& strbuilder)
{
	if (parameters.m_parameters.size() == 0)
	{
		strbuilder << "v";
	}
	else
	{
		//StringA *mname = new StringA();
		for (uint i = 0; i < parameters.m_parameters.size(); i++)
		{
			MangleType(parameters.m_parameters[i].m_pType, strbuilder);
		}
	}
}

#if 0
/*
<mangled-name> ::= _Z <encoding>
    <encoding> ::= <function name> <bare-function-type>
	       ::= <data name>
	       ::= <special-name>

*/

#endif

//template class LFCEXT Mangler<char>;
//template class LFCEXT Mangler<WCHAR>;

/*
ReturnThis MyClass::test()
{
	return ReturnThis();
}

String MyClass::get_Name() const
{
	return String();
}

String MyClass::get_QName() const
{
	return String();
}
*/

}
