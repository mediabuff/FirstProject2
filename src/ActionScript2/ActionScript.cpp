#include "stdafx.h"
#include "ActionScript2.h"

#include "ActionScript.h"
#include "ActionScriptFunction.h"

namespace System
{
namespace ActionScripting
{

ImmutableString<char> empty_str("");
ImmutableString<char> true_str("true");
ImmutableString<char> false_str("false");
ImmutableString<char> null_str("null");
ImmutableString<char> undefined_str("undefined");

ImmutableString<wchar_t> empty_str_w(L"");
ImmutableString<wchar_t> true_str_w(L"true");
ImmutableString<wchar_t> false_str_w(L"false");
ImmutableString<wchar_t> null_str_w(L"null");
ImmutableString<wchar_t> undefined_str_w(L"undefined");

bool StkElt::ToBool() const
{
	switch (m_variant.kind)
	{
	case System::VARKIND_BOOL:
		return m_variant.u.bool_val;

	case System::VARKIND_INT32:
		return m_variant.u.int32_val & 1;

	case System::VARKIND_FLOAT32:
		return (int32)m_variant.u.float32_val != 0;

	case System::VARKIND_FLOAT64:
		return (int32)m_variant.u.float64_val != 0;

		/*
	case System::VARKIND_STRINGA:
		ASSERT(0);
		return str2int(CString((System::StringA*)m_variant.m_pObject).c_str());

	case System::VARKIND_STRINGW:
		ASSERT(0);
		return str2int(CStringw((System::StringW*)m_variant.m_pObject).c_str());
		*/
	}
	return 0;
}

int32 StkElt::ToInt() const
{
	switch (m_variant.kind)
	{
	case System::VARKIND_BOOL:
		return m_variant.u.bool_val;

	case System::VARKIND_INT32:
		return m_variant.u.int32_val;

	case System::VARKIND_FLOAT32:
		return (int32)m_variant.u.float32_val;

	case System::VARKIND_FLOAT64:
		return (int32)m_variant.u.float64_val;

		/*
	case System::VARKIND_STRINGA:
		return str2int(CString((System::StringA*)m_variant.m_pObject).c_str());

	case System::VARKIND_STRINGW:
		return str2int(CStringw((System::StringW*)m_variant.m_pObject).c_str());
		*/
	}
	return 0;
}

float StkElt::ToFloat() const
{
	switch (m_variant.kind)
	{
	case System::VARKIND_BOOL:
		return (float)m_variant.u.bool_val;

	case System::VARKIND_INT32:
		return (float)m_variant.u.int32_val;

	case System::VARKIND_FLOAT32:
		return m_variant.u.float32_val;

	case System::VARKIND_FLOAT64:
		return (float)m_variant.u.float64_val;

		/*
	case System::VARKIND_STRINGA:
		return (float)getfnumber(CString((System::StringA*)m_variant.m_pObject).c_str());

	case System::VARKIND_STRINGW:
		return (float)getfnumber(CStringw((System::StringW*)m_variant.m_pObject).c_str());
		*/
	}
	return 0;
}

double StkElt::ToDouble() const
{
	switch (m_variant.kind)
	{
	case System::VARKIND_BOOL:
		return (double)m_variant.u.bool_val;

	case System::VARKIND_INT32:
		return (double)m_variant.u.int32_val;

	case System::VARKIND_FLOAT32:
		return m_variant.u.float32_val;

	case System::VARKIND_FLOAT64:
		return m_variant.u.float64_val;

		/*
	case System::VARKIND_STRINGA:
		return getfnumber(CString((System::StringA*)m_variant.m_pObject).c_str());

	case System::VARKIND_STRINGW:
		return getfnumber(CStringw((System::StringW*)m_variant.m_pObject).c_str());
		*/
	}
	return 0;
}

VM::VM()
{
	m_flags = 0;
	m_breakpoint = NULL;
	m_userobject = NULL;

	m_global = NULL;
	m_pc = NULL;
	m_pFrame = NULL;

	OnSetVariable = NULL;
}

StackFrame::StackFrame()
{
	m_instructionsData = NULL;
	m_pCaller = NULL;
	m_return_pc = NULL;
	m_pContext = NULL;
}

String StkElt::ToStringA() const
{
	ASSERT(0);
	return nullptr;
#if 0
	switch (m_variant.kind)
	{
	case VARKIND_NULL:
		{
			return &empty_str;
		//	return null_str;
		}
		break;

	case VARKIND_UNDEFINED:
		{
			return &undefined_str;
		}
		break;

	case VARKIND_BOOL:
		{
			return m_variant.u.bool_val? &true_str: &false_str;
		}
		break;

	case VARKIND_INT32:
		{
			char buf[32];
			sprintf_s(buf, "%d", m_variant.u.int32_val);
			return String(string_copy(buf));
		}
		break;

	case VARKIND_FLOAT32:
		{
			char buf[32];
			sprintf_s(buf, "%f", m_variant.u.float32_val);
			return String(string_copy(buf));
		}
		break;

	case VARKIND_FLOAT64:
		{
			char buf[32];
			sprintf_s(buf, "%f", m_variant.u.float64_val);
			return String(string_copy(buf));
		}
		break;

#if 0
	case VARKIND_STRINGA:
		{
		//	ASSERT(m_variant.m_pObject->GetType() == typeid(StringA).GetType());
			return String(dynamic_cast<StringA*>(m_variant.m_pObject));
		}

	case VARKIND_STRINGW:
		{
		//	ASSERT(m_variant.m_pObject->GetType() == typeid(StringW).GetType());
			return String(dynamic_cast<StringW*>(m_variant.m_pObject));
		}
#endif
	case VARKIND_OBJECT:
		{
			char buf[64];
			sprintf_s(buf, "(object=0x%x)", m_variant.m_pObject);
			return String(string_copy(buf));
		}
		break;

	case VARKIND_STRUCT:
		{
			IO::StringWriter stream;
			PrintValue(stream, m_variant.m_pType, (byte*)m_variant.m_structptr, ",");
			return stream.str();
		}
		break;

	default:
		ASSERT(0);
		return null;
	}
#endif
}

String StkElt::Identifier() const
{
	switch (m_variant.kind)
	{
		/*
	case VARKIND_STRINGA:
	case VARKIND_STRINGW:
		{
		return String((StringObject*)m_variant.m_pObject);
		}
		break;
		*/
	default:
		raise(Exception("not an identifier"));
	}
	return nullptr;
}

String StkElt::ToString() const
{
	return nullptr;
#if 0
	switch (m_variant.kind)
	{
	case VARKIND_NULL:
		{
		//	return &null_str_w;
			return &null_str_w;
		}
		break;

	case VARKIND_UNDEFINED:
		{
			return &undefined_str_w;
		}
		break;

	case VARKIND_BOOL:
		{
			return m_variant.u.bool_val? &true_str_w: &false_str_w;
		}
		break;

	case VARKIND_INT32:
		{
			if (m_variant.m_pType && m_variant.m_pType->get_Kind() == type_enum)
			{
				EnumType* pEnumType = static_cast<EnumType*>(m_variant.m_pType);

				EnumDef* pDef = pEnumType->GetItem(m_variant.u.int32_val);
				return pDef->m_name;
			}
			else
			{
				WCHAR buf[32];
				swprintf_s(buf, L"%d", m_variant.u.int32_val);
				return String(string_copy(buf));
			}
		}
		break;

	case VARKIND_FLOAT32:
		{
			WCHAR buf[64];
			swprintf_s(buf, L"%f", m_variant.u.float32_val);
			return String(string_copy(buf));
		}
		break;

	case VARKIND_FLOAT64:
		{
			WCHAR buf[64];
			swprintf_s(buf, L"%f", m_variant.u.float64_val);
			return String(string_copy(buf));
		}
		break;

		/*
	case VARKIND_STRINGA:
	case VARKIND_STRINGW:
		{
			return "\"" + String((StringObject*)m_variant.m_pObject) + "\"";
		}
		break;
		*/
	case VARKIND_OBJECT:
		{
			if (true)
			{
				return m_variant.m_pObject->ToString();
			}
			else
			{
				ClassType* pType = m_variant.m_pObject->GetType();
				WCHAR buf[64];
				swprintf_s(buf, L"([%S]=0x%x)", CString(pType->m_qname).c_str(), m_variant.m_pObject);
				return String(string_copy(buf));
			}
		}
		break;

	case VARKIND_STRUCT:
		{
			IO::StringWriter writer;
			PrintValue(writer, m_variant.m_pType, (byte*)m_variant.m_structptr, "\n");
			return writer.str();
		}
		break;

	default:
		ASSERT(0);
		return null;
	}
#endif
}

Variant MembersMap::GetMember(StringIn name)
{
	return m_members[name];
}

void MembersMap::SetMember(StringIn name, Variant value)
{
	m_members[name] = value;
}

void VM::push(StkElt value)
{
	m_stack.push_back(value);
}

StkElt VM::pop()
{
	size_t size = m_stack.GetSize();
	if (size == 0) raise(Exception(L"Couldn't pop from empty stack"));
	StkElt elt = m_stack[m_stack.GetSize()-1];
	//ASSERT(elt == m_stack[size-1]);
	m_stack.RemoveAt(m_stack.GetSize()-1);
	return elt;
}

void VM::as_notimpl(unsigned int length)
{
	ASSERT(0);
	raise(Exception(L"ActionScript: op not implemented"));
}

Declarator* FindDeclarator(Scope* scope, StringIn name)
{
	for (uint i = 0; i < scope->m_orderedDecls.size(); ++i)
	{
		if (scope->m_orderedDecls[i]->m_name.LeftOf(4) == "get_" &&
			scope->m_orderedDecls[i]->m_name.RightOf(4) == name)
		{
			return scope->m_orderedDecls[i];
		}
	}

	return NULL;
}

bool IsCompatible(const StkElt& arg, Type* argType)
{
#if 0
	argType = argType->GetStripped();

	if (arg.m_variant.kind == VARKIND_BOOL)
	{
		if (argType->get_Kind() == type_bool)
		{
			return true;
		}
	}
	else if (arg.m_variant.kind == VARKIND_INT32)
	{
		if (
			argType->get_Kind() == type_signed_char ||
			argType->get_Kind() == type_unsigned_char ||
			argType->get_Kind() == type_short ||
			argType->get_Kind() == type_unsigned_short ||
			argType->get_Kind() == type_int ||
			argType->get_Kind() == type_unsigned_int ||
			argType->get_Kind() == type_float ||
			argType->get_Kind() == type_double ||

			argType->get_Kind() == type_enum)

		{
			return true;
		}
	}
	else if (arg.m_variant.kind == VARKIND_FLOAT64)
	{
		if (
			argType->get_Kind() == type_signed_char ||
			argType->get_Kind() == type_unsigned_char ||
			argType->get_Kind() == type_short ||
			argType->get_Kind() == type_unsigned_short ||
			argType->get_Kind() == type_int ||
			argType->get_Kind() == type_unsigned_int ||
			argType->get_Kind() == type_float ||
			argType->get_Kind() == type_double)
		{
			return true;
		}
	}
#if 0
	else if (arg.m_variant.kind == VARKIND_STRINGA ||
			arg.m_variant.kind == VARKIND_STRINGW)
	{
		if (argType->get_Kind() == type_class)
		{
			// TODO
					return true;	// Derived from, ok, continue
		}
		else if (argType->get_Kind() == type_pointer &&
			(argType->GetPointerTo()->GetStripped()->get_Kind() == type_char ||
			argType->GetPointerTo()->GetStripped()->get_Kind() == type_wchar))
		{
			// char* or wchar_t*

			return true;
		}
	}
#endif
	else if (arg.m_variant.kind == VARKIND_STRUCT)
	{
		ClassType* pStructType = arg.m_variant.m_pType->AsClass();

		if (argType->get_Kind() == type_class)
		{
			if (pStructType == argType->GetClass())
			{
				return true;	// match, ok, continue
			}
		}
		else if (argType->get_Kind() == type_reference)
		{
			Type* refType = argType->GetPointerTo()->GetStripped();

			if (refType->get_Kind() == type_class)
			{
				if (pStructType->IsDerivedFrom(refType->AsClass()))
				{
					return true;	// Derived from, ok, continue
				}
			}
		}
	}
	else if (arg.m_variant.kind == VARKIND_OBJECT)
	{
		ClassType* pObjectType = arg.m_variant.m_pObject->GetType();

		if (argType->get_Kind() == type_pointer)
		{
			if (argType->GetPointerTo()->get_Kind() == type_class)
			{
				if (pObjectType->IsDerivedFrom(argType->GetPointerTo()->AsClass()))
				{
					return true;	// Derived from, ok, continue
				}
			}
		}
		else if (argType->get_Kind() == type_reference)	// Treat like pointer
		{
			if (argType->GetPointerTo()->get_Kind() == type_class)
			{
				if (pObjectType->IsDerivedFrom(argType->GetPointerTo()->GetClass()))
				{
					return true;	// Derived from, ok, continue
				}
			}
		}
	}
	else if (arg.m_variant.kind == VARKIND_NULL)
	{
		if (argType->get_Kind() == type_pointer)
		{
			// by commenting this, we can call a function like
			// func(int* iptr)
			// don't know if a function like above is useful from ecmascript though?

		//	if (pType->GetPointerTo()->get_Kind() == type_class)
			{
				return true;	// ok, continue
			}
		}
	}
	else
	{
		raise(Exception("TODO: conversion not implemented"));
		ASSERT(0);
	}
#endif
	return false;
}

byte* DoArg(Type* ArgType, const StkElt& elt, byte* p)
{
	switch (ArgType->get_Kind())
	{
	case type_bool:
		{
			*(bool*)p = elt.ToBool();
			p += sizeof(bool);
		}
		break;

	case type_int:
		{
			*(int*)p = elt.ToInt();
			p += sizeof(int);
		}
		break;

	case type_unsigned_int:
		{
			*(int*)p = elt.ToInt();
			p += sizeof(unsigned int);
		}
		break;

	case type_enum:
		{
			// TODO: Not 100% sure about size

			*(int*)p = elt.ToInt();
			p += sizeof(int);
		}
		break;

	case type_float:
		{
			*(float*)p = elt.ToFloat();
			p += sizeof(float);
		}
		break;

	case type_double:
		{
			*(double*)p = elt.ToDouble();
			p += sizeof(double);
		}
		break;

	default:
		{
			//ASSERT(elt.m_variant.kind

			//Type* ArgType = method->m_decl->m_pType->GetFunction()->m_parameters.m_parameters[0]->m_pType->GetStripped();
			if (ArgType->get_Kind() == type_class)
			{
				ClassType* pClassType = ArgType->GetClass();
				// TODO, constructor

				if (pClassType->m_qname == "System::StringVariant")
				{
					*(StringVariant*)p = StringVariant(elt.ToString());
					p += sizeof(StringVariant);
				}
				else// if (pClassType->get_sizeof() == 12)
				{
					Dispatch* pDispatch = GetDispatch(pClassType);

					Declarator* pMethod = NULL;

					auto it = pDispatch->m_methodnames.find(pClassType->m_name);
					if (it != pDispatch->m_methodnames.end())
					{
						do
						{
							Declarator* pMethod2 = it->second;
							if (pMethod2->m_pType->AsFunction()->m_parameters.m_parameters.size() == 1)
							{
								if (IsCompatible(elt, pMethod2->m_pType->AsFunction()->m_parameters.m_parameters[0].m_pType))
								{
									ASSERT(pMethod == nullptr);	// TODO
									pMethod = pMethod2;
								}
							}

							++it;
						}
						while (it->first == pClassType->m_name);

						if (pMethod)
						{
							// TODO, TODO

							if (elt.m_variant.kind == VARKIND_STRUCT)
							{
								Type* ArgType = pMethod->m_pType->AsFunction()->m_parameters.m_parameters[0].m_pType;
								if (ArgType->get_Kind() == type_reference)
								{
									void_invoke_method32(pMethod->m_offset, p, &elt.m_variant.m_structptr, sizeof(void*));
								}
								else
								{
							//	byte args[256];
							//	byte* endargs = DoArg(pMethod->m_decl->m_pType->GetFunction()->m_parameters.m_parameters[].m_pType, elt, args);
							//	void_invoke_method(pMethod->m_decl->m_offset, p, args, endargs-args);
									void_invoke_method32(pMethod->m_offset, p, elt.m_variant.m_structptr, elt.m_variant.m_pType->get_sizeof());
								}
							}
							else if (elt.m_variant.kind == VARKIND_OBJECT)
							{
									
								/*
								*(Object**)p = NULL;
								p += sizeof(Object*);
								*(Object**)p = NULL;
								p += sizeof(Object*);
								*(Object**)p = elt.m_variant.m_pObject;	// TODO IncRef
								p += sizeof(Object*);
								*/
								pDispatch->void_Invoke(p, pMethod, elt.m_variant.m_pObject, sizeof(Object*));
							}
							else
							{
								// TODO
								ASSERT(0);
							}
						}
						else
						{
							if (pClassType == elt.m_variant.m_pType->GetStripped())
							{
								// TODO, sub ??

								// just copy
								memcpy(p, elt.m_variant.m_structptr, elt.m_variant.m_pType->get_sizeof());
							}
							else
							{
								raise(Exception(L"no matching copy constructor found"));
							}
						}

						p += pClassType->get_sizeof();
					}
					else
					{
						ASSERT(0);
					}

					/*
					// TODO
					// typed
					*(Object**)p = NULL;
					p += sizeof(Object*);
					*(Object**)p = NULL;
					p += sizeof(Object*);
					*(Object**)p = elt.m_variant.m_pObject;	// TODO IncRef
					p += sizeof(Object*);
					*/
				}
			}
			else if (ArgType->get_Kind() == type_reference)
			{
				Type* ArgType2 = ArgType->GetPointerTo()->GetStripped();
				if (ArgType2->get_Kind() == type_class)
				{
					if (elt.m_variant.kind == VARKIND_OBJECT)
					{
						if (elt.m_variant.m_pObject && elt.m_variant.m_pObject->GetType()->IsDerivedFrom(ArgType2->AsClass()))
						{
							*(void**)p = elt.m_variant.m_pObject;
							p += sizeof(void*);
						}
						else
						{
							Dispatch* pDispatch = GetDispatch(ArgType2->GetClass());

						//	Convert(ArgType2, m_variant.m_pObject->GetType());

							// Try all constructors, if more than one constructor matches, there's an ambiguity
							auto it = pDispatch->m_methodnames.find(pDispatch->GetClass()->get_Name());
							if (it != pDispatch->m_methodnames.end())
							{
								String cname;
								Declarator* pMethod = NULL;
								do
								{
									Declarator* pMethod = it->second;
									if (pMethod->m_pType->AsFunction()->m_parameters.size() == 1)
									{
										Type* ArgType3 = pMethod->m_pType->AsFunction()->m_parameters[0].m_pType->GetStripped();
										if (ArgType3->get_Kind() == type_pointer)
										{
											ArgType3 = ArgType3->GetPointerTo()->GetStripped();

											if (ArgType3->get_Kind() == type_class)
											{
												if (elt.m_variant.m_pObject->GetType()->IsDerivedFrom(ArgType3->AsClass()))
												{
													// temp object
													byte* temp = (byte*)_alloca(ArgType3->get_sizeof());

													void_invoke_method32(pMethod->m_offset, temp, &elt.m_variant.m_pObject, sizeof(Object*));

													*(void**)p = temp;
													p += sizeof(void*);

													break;
												}
											}
											else
											{
												raise(Exception(L"not derived from.."));
											}
										}
									}

									++it;
								}
								while (it->first == pDispatch->GetClass()->get_Name());
							}
							else
							{
								raise(Exception(L"not derived from.."));
							}
						}
					}
					else
					{
						ASSERT(0);
#if 0
						if (elt.m_variant.kind == VARKIND_NULL)
						{
							raise(Exception(ArgType->ToString() + " cannot be set to null value"));
						}
						else
						{
							raise(Exception(L"Unsupported arg type"));
						}
#endif
					}
				}
				else
				{
					raise(Exception(L"ecmacript cannot call function with non-class reference as parameter"));
				}

				//	p += ArgType->get_sizeof();
			}
			else if (ArgType->get_Kind() == type_pointer)
			{
				ArgType = ArgType->GetPointerTo()->GetStripped();

				//		ASSERT(pSourceType->m_type == type_pointer);
				if (ArgType->get_Kind() == type_char)
				{
					*(char**)p = CString(elt.ToString()).Detach();
					p += sizeof(char*);
				}
				else if (ArgType->get_Kind() == type_wchar)
				{
					*(wchar_t**)p = CStringw(elt.ToString()).Detach();
					p += sizeof(wchar_t*);
				}
				else if (ArgType->get_Kind() == type_class)
				{
					void* properType;

					if (ArgType == typeid(System::StringA).GetType())	// TODO, remove ?
					{
						Object* pObject = elt.ToString();
						properType = pObject;
					}
					else if (ArgType == typeid(System::StringW).GetType())	// TODO, remove ?
					{
						Object* pObject = elt.ToString();
						properType = pObject;
					}
					else
					{
						ASSERT(0);
#if 0
						Object* pObject = elt.m_variant.kind == VARKIND_NULL? NULL: elt.m_variant.m_pObject;
						ClassType* pSourceType = pObject? pObject->GetType(): NULL;

						if (pObject)
						{
							properType = DynamicCast(pObject, pSourceType->GetClass()/*->GetPointerTo()*/, ArgType->GetClass());
							if (properType == NULL)
							{
								raise(Exception(ArgType->GetClass()->get_QName() + " is not derived from " + pSourceType->GetClass()->get_QName()));
							}
						}
						else
							properType = NULL;
#endif
					}

					*(void**)p = properType;
					p += sizeof(void*);
				}
				else
				{
					raise(Exception(L"Unsupported arg type"));
					ASSERT(0);
				}
			}
			else
			{
				raise(Exception(L"Unsupported arg type"));
				ASSERT(0);
			}
		}
	}

	return p;
}

void VM::as_GetMember(unsigned int length)
{
	StkElt member = pop();
	StkElt object = pop();

	TRACE("%s.%s", CString(object.ToStringA()).c_str(), CString(member.ToStringA()).c_str());

	if (object.m_variant.kind != VARKIND_OBJECT)
	{
		throw new Exception(L"Expected object");
	}

	if (true)
	{
		String propertyName = member.ToStringA();

		/*
	class type_info
	{
	public:
		_CRTIMP virtual ~type_info();

		inline System::Type* GetType() const;

		void *_m_data;
	}
	*/


		ClassType* pType = object.m_variant.m_pObject->GetType();
		if (NamespaceType* pName = dynamic_cast<NamespaceType*>(object.m_variant.m_pObject))
		{
			Dispatch* pDispatch = GetDispatch(pName);

			Property* pProperty = pDispatch->GetProperties().find(propertyName)->second;
			if (pProperty == NULL)
/*

			/
			Declarator* decl = FindDeclarator(pName->m_pScope, propertyName);
			if (decl == NULL)
			*/
			{
				raise(Exception(L"property not found"));
			}

			if (!pProperty->get_GetMethod()->get_IsStatic())

			//if (decl->m_offset == -1)
			{
				raise(Exception(L"property is not a static property"));
			}

		//	byte args[512];
		//	byte* p = GetArgs(decl->m_pType->GetFunction(), args, numargs)

			Object* result = (Object*)int32_invoke_function_cdecl(pProperty->get_GetMethod()->m_offset, NULL, 0);

			push(result);
		}
		else if (Typedef* pTypedef = dynamic_cast<Typedef*>(object.m_variant.m_pObject))
		{
			ASSERT(0);
		}
		else if (EnumType* pEnum = dynamic_cast<EnumType*>(object.m_variant.m_pObject))
		{
			EnumDef* pDef = pEnum->GetItem(propertyName);
			if (pDef)
			{
				push(Variant(pDef->m_value, pEnum));
			}
			else
			{
				raise(Exception(L"enum '" + propertyName + L"' not found"));
			}
		}
		else
		{
			Dispatch* pDispatch = GetDispatch(pType);
			Property* pProperty = pDispatch->GetProperty(propertyName);
			if (pProperty)
			{
				CallMethod(pDispatch, object.m_variant.m_pObject, pProperty->get_GetMethod(), 0, true);
			}
			else
			{
				raise(Exception(L"property '" + propertyName + L"' not found"));
				ASSERT(0);
				/*
				Gui::UIElement* expandable = dynamic_cast<Gui::UIElement*>(object.m_variant.m_pObject);
				Object* value = expandable->m_userdata[member.ToString()];

				push(StkElt(value));
				*/
			}
		}
	}
	else
	{
		IMembersMap* pob = dynamic_cast<IMembersMap*>(object.m_variant.m_pObject);

		StkElt value = pob->GetMember(member.ToStringA());

		TRACE(" = %s", CString(value.ToStringA()).c_str());

		push(value);
	}
}

void VM::as_SetMember(unsigned int length)
{
	StkElt value = pop();
	StkElt member = pop();
	StkElt object = pop();

	TRACE("%s.%s = %s", CString(object.ToStringA()).c_str(), CString(member.ToStringA()).c_str(), CString(value.ToStringA()).c_str());

	if (object.m_variant.kind != VARKIND_OBJECT)
	{
		raise(Exception(L"Expected object"));
		ASSERT(0);
	}

	if (true)
	{
		if (object.m_variant.m_pObject == NULL)
		{
			ASSERT(0);
		}

		if (NamespaceType* pName = dynamic_cast<NamespaceType*>(object.m_variant.m_pObject))
		{
			NamespaceType* pType = pName;

			Dispatch* pDispatch = GetDispatch(pType);
			Property* pProperty = pDispatch->GetProperty(member.ToStringA());
			if (pProperty == NULL)
			{
				raise(Exception(pType->get_QName() + L"." + member.ToString() + " does not not exist"));
			}

			if (pProperty->get_SetMethod() == NULL)
			{
				raise(Exception(pType->get_QName() + L"." + member.ToString() + " is read only"));
			}

			byte args[256];
			byte* p = DoArg(pProperty->get_SetMethod()->m_pType->GetFunction()->m_parameters.m_parameters[0].m_pType, value, args);
			void_invoke_function_cdecl(pProperty->get_SetMethod()->m_offset, args, p-args);
		}
		else
		{
			ClassType* pType = object.m_variant.m_pObject->GetType();
			Dispatch* pDispatch = GetDispatch(pType);
			Property* pProperty = pDispatch->GetProperty(member.ToStringA());
			if (pProperty == NULL)
			{
				raise(Exception(pType->get_QName() + L"." + member.ToString() + " does not not exist"));
			}

			if (pProperty->get_SetMethod() == NULL)
			{
				raise(Exception(pType->get_QName() + L"." + member.ToString() + " is read only"));
			}

			push(value);

			CallMethod(pDispatch, object.m_variant.m_pObject, pProperty->get_SetMethod(), 1, false);
		}
	}
	else
	{
		IMembersMap* pob = dynamic_cast<IMembersMap*>(object.m_variant.m_pObject);

		pob->SetMember(member.ToStringA(), value);
	}
}

void VM::as_With(unsigned int length)
{
	StkElt object = pop();

	DebugTrace(object.ToString());

	if (object.m_variant.kind != VARKIND_OBJECT)
	{
		ASSERT(0);
		throw new Exception(L"Expected object");
	}

	uint16 size = *m_pc++;
	size |= (*m_pc++)<<8;

//	StringA str = StringA((const char*)pc, length);
//	pc += length;

//	PrintActions(pc, length);

	StackFrame* pFrame = new StackFrame;
	pFrame->m_pContext = m_pFrame->m_pContext;
	pFrame->m_pCaller = m_pFrame;
	pFrame->m_return_pc = m_pc+size;
	pFrame->m_instructionsData = m_pc;
	pFrame->m_instructionsLength = size;
	m_pc = pFrame->m_instructionsData;
	m_pFrame = pFrame;

	pFrame->m_variables[String("this")] = object.m_variant.m_pObject;

//	TRACE(" %s - %s", object.ToStringA().c_str(), str.c_str());
}

void VM::as_DefineFunction(unsigned int length)
{
	//StringA* funName = new StringA();
	uint8* start = m_pc;

	//StkElt name = pop();

	while (*m_pc)
	{
		//funName = funName->Append(*pc++);
		m_pc++;
	}
	String funName = nullptr;
	if (start[0])
		funName = String(string_copy((const char*)start, m_pc - start));

	m_pc++;

	TRACE(" %s(", (const char*)start);//->c_str());

	uint16 numParams = *m_pc++;
	numParams |= (*m_pc++)<<8;

	ActionScriptFunction* pFunction = new ActionScriptFunction;

//	m_ctx->m_pContext->m_funmap[new StringA(funName)] = pFunction;
	if (funName)
		m_pFrame->m_variables[funName] = pFunction;

	for (size_t i = 0; i < numParams; ++i)
	{
		const char* paramName = (const char*)m_pc;
		while (*m_pc)
		{
			m_pc++;
		}

		pFunction->m_args.push_back(String(string_copy(paramName, (const char*)m_pc - paramName)));

		m_pc++;

		if (i > 0) TRACE(",");
		TRACE("%s", paramName);//->c_str());
	}

	TRACE(")");

	uint16 codeSize = *m_pc++;
	codeSize |= (*m_pc++)<<8;

	pFunction->m_pContext = m_pFrame->m_pContext;
	pFunction->m_codeData = m_pc;
	pFunction->m_codeLength = codeSize;

	m_pc += codeSize;

	if (funName == nullptr)	// anonymous
	{
		StkElt elt;
		elt.m_variant = Variant(pFunction);

		push(elt);
	}
}

// Create a new constant pool, erase the old one
void VM::as_ConstantPool(unsigned int length)
{
	uint16 count = *m_pc++;
	count |= (*m_pc++)<<8;

	m_pFrame->m_pContext->m_constantPool.clear();

//	m_constantPool.resize(MAX(m_constantPool.size(), count));

	m_pFrame->m_pContext->m_constantPool.reserve(count);

	for (int i = 0; i < count; i++)
	{
		uint8* start = m_pc;

		while (*m_pc)
		{
		//	str = str->Append(*pc++);
			m_pc++;
		}
		String str = String(string_copy((const char*)start, m_pc - start));

		m_pc++;
	//	TRACE("%s, ", str->c_str());

	//	m_constantPool[i] = (str);
		m_pFrame->m_pContext->m_constantPool.Add(StkElt(str));
	}
}

void VM::as_Push(unsigned int length)
{
	uint8* start = m_pc;

	while (m_pc < start+length)
	{
		uint8 type = *m_pc++;
		switch (type)
		{
			/*
		case 2:	// null
			{
				push(StkElt(VARKIND_NULL));
			}
			break;

		case 3:	// undefined
			{
				push(StkElt(VARKIND_UNDEFINED));
			}
			break;
			*/

		case 1:	// 32 bit float
			{
				uint32 value = *m_pc++;
				value |= (*m_pc++)<<8;
				value |= (*m_pc++)<<16;
				value |= (*m_pc++)<<24;

			//	TRACE(" %f", value);
				push(*(float*)&value);
			}
			break;

		case 5:	// boolean
			{
				ubyte value = *m_pc++;

				push((bool)(value & 1));
			}
			break;

		case 6:	// 64 bit float
			{
				uint64 value = *m_pc++;
				value |= (uint64(*m_pc++))<<8U;
				value |= (uint64(*m_pc++))<<16U;
				value |= (uint64(*m_pc++))<<24U;
				value |= (uint64(*m_pc++))<<32U;
				value |= (uint64(*m_pc++))<<40U;
				value |= (uint64(*m_pc++))<<48U;
				value |= (uint64(*m_pc++))<<56U;

			//	TRACE(" %f", value);
				push(*(double*)&value);
			}
			break;

		case 7:	// 32 bit integer
			{
				int32 value = *m_pc++;
				value |= (*m_pc++)<<8;
				value |= (*m_pc++)<<16;
				value |= (*m_pc++)<<24;

				TRACE(" %d", value);
				push(value);
			}
			break;

		case 8:	// 8-bit index into pool
			{
				uint8 index = *m_pc++;

				if (index < m_pFrame->m_pContext->m_constantPool.GetSize())
				{
					TRACE(" %s", CString(m_pFrame->m_pContext->m_constantPool[index].ToStringA()));

					push(m_pFrame->m_pContext->m_constantPool[index]);
				}
				else
				{
					DebugTrace(" out of bounds");
				//	push(StkElt());
				}
			}
			break;

		case 9:	// 16-bit index into pool
			{
				uint16 index = *m_pc++;
				index |= (*m_pc++)<<8;

				if (index < m_pFrame->m_pContext->m_constantPool.GetSize())
				{
					TRACE(" %s", CString(m_pFrame->m_pContext->m_constantPool[index].ToStringA()).c_str());

					push(m_pFrame->m_pContext->m_constantPool[index]);
				}
				else
				{
					DebugTrace(" out of bounds");
				//	push(StkElt());
				}
			}
			break;

		default:
			ASSERT(0);
		}
	}
}

void VM::as_Pop(unsigned int length)
{
	pop();
}

void VM::as_Less2(unsigned int length)
{
	StkElt arg1 = pop();
	StkElt arg2 = pop();

// TODO

	bool result = arg2.ToInt() < arg1.ToInt();

	push(StkElt(result));
}

void VM::as_Greater(unsigned int length)
{
	StkElt arg1 = pop();
	StkElt arg2 = pop();

// TODO

	bool result = arg2.ToInt() > arg1.ToInt();

	push(StkElt(result));
}

void VM::as_Add2(unsigned int length)
{
	StkElt arg1 = pop();
	StkElt arg2 = pop();

	ASSERT(0);
	DebugTrace(arg2.ToString() << "+" << arg1.ToString());

#if 0
	if ((arg1.m_variant.kind == VARKIND_STRINGW || arg1.m_variant.kind == VARKIND_NULL) ||
		(arg2.m_variant.kind == VARKIND_STRINGW || arg2.m_variant.kind == VARKIND_NULL))
	{
		String value = arg2.ToString() + arg1.ToString();
		/*
		StringW* value = new StringW(arg2.ToString()- + *arg1.ToString();
		*/

		push(StkElt(value));
	}
	else if (arg1.m_variant.kind == VARKIND_STRINGA && arg2.m_variant.kind == VARKIND_STRINGA)
	{
		ASSERT(0);
#if 0
		StringA* value = *arg2.ToString() + *arg1.ToString();

		push(StkElt(value));
#endif
	}
	else
#endif
	{
		if (arg1.m_variant.kind == VARKIND_FLOAT64 ||
			arg2.m_variant.kind == VARKIND_FLOAT64)
		{
			double value = arg2.ToDouble() + arg1.ToDouble();
			push(StkElt(value));
		}
		else if (arg1.m_variant.kind == VARKIND_FLOAT32 ||
			arg2.m_variant.kind == VARKIND_FLOAT32)
		{
			float value = arg2.ToFloat() + arg1.ToFloat();
			push(StkElt(value));
		}
		else
		{
			int32 value = arg2.ToInt() + arg1.ToInt();
			push(StkElt(value));
		}
	}
}

void VM::as_StringAdd(unsigned int length)
{
	StkElt arg1 = pop();
	StkElt arg2 = pop();

	push(arg2.ToString() + arg1.ToString());
}

void VM::as_If(unsigned int length)
{
	StkElt cond = pop();

	short offset = (*m_pc++);
	offset |= (*m_pc++)<<8;

	if (cond == true)
	{
		m_pc += offset;
	}
}

void VM::as_DefineLocal2(unsigned int length)
{
	StkElt name = pop();

	//pair<map<System::StringA*, StkElt, Ref_Less<System::StringA>, System::__gc_allocator>::iterator, bool> r;
	/*r =*/ m_pFrame->m_variables.insert(map<String, StkElt>::value_type(name.ToStringA(), StkElt()));

	DebugTrace("var " << name.ToString() << "\n");
}

//			void SetValue(Dispatch* dispatch, void* _this, Method* method, Object* pObject, Type* pSourceType);

byte* VM::GetArgs(FunctionType* pFun, byte* args, unsigned int numargs)
{
	byte* p = args;
	for (unsigned int i = 0; i < numargs; i++)
	{
		Type* argType = pFun->m_parameters.m_parameters[i].m_pType->GetStripped();

		StkElt elt = pop();

		p = DoArg(argType, elt, p);

		/*
		switch (elt.m_variant.kind)
		{
		case VARKIND_INT32:
			{
				*(int*)p = elt.toInt();
				p += 4;
			}
			break;

		case VARKIND_STRINGA:
		case VARKIND_STRINGW:
		case VARKIND_OBJECT:
			{
			*/


		// TODO, convert to appropriate type

	//	*(void**)p = elt.m_variant.m_pStringA;
		//*(int*)p = elt.toInt();

		//switch (elt
	}

	return p;
}

void VM::CallMethod(Dispatch* pDispatch, void* object, Declarator* pMethod, unsigned int numargs, bool bReturnValue)
{
	uint8 args[512];
	uint8* p = args;

	if (pMethod->m_pType->GetFunction()->m_parameters.m_parameters.size() != numargs)
	{
		raise(Exception(L"Wrong number of arguments"));
	}

	Type* pReturnType = pMethod->m_pType->GetFunction()->get_ReturnType()->GetStripped();

	p = GetArgs(pMethod->m_pType->GetFunction(), args, numargs);
	ptrdiff_t argssize = p - args;
	argssize = (argssize + 3) & ~3;

	DispatcherObject* dispatcherObject = dynamic_cast<DispatcherObject*>((Object*)object);
	if (dispatcherObject && dispatcherObject->m_threadId != GetCurrentThreadId())
	{
		DispatchMsg dmsg;
		dmsg.caller.m_hWaitEvent = CreateEventW(NULL, false, false, NULL);	// TODO, create once

		dmsg.m_dispatch = pDispatch;
		dmsg.m_object = object;
		dmsg.m_method = pMethod;
		dmsg.m_args = args;
		dmsg.m_args_nbytes = (uint)argssize;

		String str;

		if (bReturnValue)
		{
			if (pReturnType->get_Kind() == type_class)
			{
			// TODO, check that String

				dmsg.retval_struct = &str;
			}
		}

		PostThreadMessageW(dispatcherObject->m_threadId, WM_USER+1, (WPARAM)2, (LPARAM)&dmsg);
		WaitForSingleObject(dmsg.caller.m_hWaitEvent, INFINITE);
		CloseHandle(dmsg.caller.m_hWaitEvent);

		if (dmsg.m_exception)
		{
			throw dmsg.m_exception;
		}

		if (bReturnValue)
		{
			switch (pReturnType->get_Kind())
			{
			case type_void:
				{
					push(StkElt());	// undefined
				}
				break;

			case type_bool:
				{
					push(StkElt(dmsg.retval.bool_val));
				}
				break;

			case type_char:
				{
					push(StkElt(dmsg.retval.char_val));
				}
				break;

			case type_wchar:
				{
					push(StkElt(dmsg.retval.wchar_t_val));
				}
				break;

			case type_signed_char:
				{
					push(StkElt(dmsg.retval.int8_val));
				}
				break;

			case type_unsigned_char:
				{
					push(StkElt(dmsg.retval.uint8_val));
				}
				break;

			case type_short:
				{
					push(StkElt(dmsg.retval.int16_val));
				}
				break;

			case type_unsigned_short:
				{
					push(StkElt(dmsg.retval.uint16_val));
				}
				break;

			case type_int:
		//	case type_long_int:
				{
					push(StkElt(dmsg.retval.int32_val));
				}
				break;

			case type_enum:
				{
					push(StkElt(dmsg.retval.int32_val));
				}
				break;

				/*
			case type_unsigned_int:
		//	case type_unsigned_long_int:
				{
					push(StkElt(dmsg.retval.uint32val));
				}
				break;
*/
				/*
			case type_int64:
				{
					push(StkElt(dmsg.retval.int64val));
				}
				break;

			case type_unsigned_int64:
				{
					push(StkElt(dmsg.retval.uint64val));
				}
				break;
				*/

			case type_float:
				{
					push(StkElt(dmsg.retval.float_val));
				}
				break;

			case type_double:
				{
					push(StkElt(dmsg.retval.double_val));
				}
				break;

			case type_class:
				{
					// TODO, check that String

					push(StkElt(str));
				}
				break;

		case type_pointer:
		case type_reference:	// Treat like pointer
			{
				if (bReturnValue)
				{
					if (pReturnType->GetPointerTo()->get_Kind() == type_class)
					{
						// TODO??
						Object* obj = dynamic_cast<Object*>((Object*)dmsg.retval_struct);

						push(StkElt(obj));
					}
					else
					{
						push(StkElt());	// undefined
					}
				}
			}
			break;

			default:
				ASSERT(0);
			}
		}
	}
	else
	{
		switch (pReturnType->get_Kind())
		{
		case type_void:
			{
				if (object)
					pDispatch->void_Invoke(object, pMethod, args, argssize);
				else
					void_invoke_function_cdecl(pMethod->m_offset, args, argssize);

				if (bReturnValue)
				{
					push(StkElt());	// undefined
				}
			}
			break;

		case type_bool:
			{
				bool result = pDispatch->bool_Invoke(object, pMethod, args, argssize);
				if (bReturnValue)
				{
					push(StkElt(result));
				}
			}
			break;

		case type_int:
		case type_unsigned_int:
			{
				int32 result;

				if (object)
					result = pDispatch->int32_Invoke(object, pMethod, args, argssize);
				else
					result = int32_invoke_function_cdecl(pMethod->m_offset, args, argssize);

				if (bReturnValue)
				{
					push(StkElt(result));
				}
			}
			break;

		case type_long:
		case type_unsigned_long:
			{
				long result;

				if (object)
					result = pDispatch->int32_Invoke(object, pMethod, args, argssize);
				else
					result = int32_invoke_function_cdecl(pMethod->m_offset, args, argssize);

				if (bReturnValue)
				{
					push(StkElt(result));
				}
			}
			break;

		case type_float:
			{
				float result = pDispatch->float_Invoke(object, pMethod, args, argssize);
				if (bReturnValue)
				{
					push(StkElt(result));
				}
			}
			break;

		case type_double:
			{
				double result = pDispatch->double_Invoke(object, pMethod, args, argssize);
				if (bReturnValue)
				{
					push(StkElt(result));
				}
			}
			break;

		case type_pointer:
		case type_reference:	// Treat like pointer
			{
				void* result = pDispatch->pointer_Invoke(object, pMethod, args, argssize);

				if (bReturnValue)
				{
					Type* pReturnType2 = pReturnType->GetPointerTo()->GetStripped();

					if (pReturnType2 == typeid(StringA).GetType())
						push(StkElt(static_cast<StringA*>(result)));
					else if (pReturnType2 == typeid(StringW).GetType())
						push(StkElt(static_cast<StringW*>(result)));
					else
						push(StkElt((Object*)result));
				}
			}
			break;

		case type_class:
			{
				ClassType* pClassType = pReturnType->GetClass();
				if (pClassType == typeof(String))
				{
					String result = pDispatch->String_Invoke(object, pMethod, args, argssize);

					if (bReturnValue)
					{
						push(StkElt(result));
					}
				}
				else
				{
					// TODO, check that it doesn't have a destructor

					size_t size = pClassType->get_sizeof();
					byte* retval = new byte[size];
					pDispatch->struct_Invoke(object, pMethod, args, argssize, retval);

					if (bReturnValue)
					{
						push(StkElt(retval, pClassType));
					}
				}
			}
			break;

		case type_enum:
			{
				int32 result;

				if (object)
					result = pDispatch->int32_Invoke(object, pMethod, args, argssize);
				else
					result = int32_invoke_function_cdecl(pMethod->m_offset, args, argssize);

				if (bReturnValue)
				{
					push(StkElt(result));
				}
			}
			break;

		default:
			{
				throw new Exception(L"Unsupported return type");
				ASSERT(0);
			}
		}
	}
}

void VM::CallMethod(Dispatch* pDispatch, void* object, Declarator* pMethod, vector<StkElt>& argsarr, bool bReturnValue)
{
	uint8 args[512];
	uint8* p = args;

	VERIFY(pMethod->m_pType->GetFunction()->m_parameters.m_parameters.size() == argsarr.size());
	Type* pReturnType = pMethod->m_pType->GetFunction()->m_pReturnType;

	for (uint i = 0; i < argsarr.size(); ++i)
	{
		Type* argType = pMethod->m_pType->GetFunction()->m_parameters.m_parameters[i].m_pType->GetStripped();

		const StkElt& elt = argsarr[i];

		p = DoArg(argType, elt, p);
#if 0
		if (ArgType->get_Kind() == type_int)
		{
			*(int*)p = elt.toInt();
			p += 4;
		}
		else if (ArgType->get_Kind() == type_unsigned_char)
		{
			*(int*)p = elt.toInt();
			p += 4;
		}
		else if (ArgType->get_Kind() == type_double)
		{
			*(double*)p = elt.toDouble();
			p += 8;
		}
		else
		{
			//ASSERT(elt.m_variant.kind

			//Type* ArgType = method->m_decl->m_pType->GetFunction()->m_parameters.m_parameters[0]->m_pType->GetStripped();
			if (ArgType->get_Kind() == type_class)
			{
				ASSERT(0);
			}
			else if (ArgType->get_Kind() == type_pointer)
			{
				ArgType = ArgType->GetPointerTo()->GetStripped();

		//		ASSERT(pSourceType->m_type == type_pointer);
				if (ArgType->get_Kind() == type_class)
				{
					void* properType;

					if (ArgType == typeid(System::StringA).GetType())
					{
						Object* pObject = elt.ToString();
						properType = pObject;
					}
					else if (ArgType == typeid(System::StringW).GetType())
					{
						Object* pObject = elt.ToString();
						properType = pObject;
					}
					else
					{
						Object* pObject = elt.m_variant.kind == VARKIND_NULL? NULL: elt.m_variant.m_pObject;
						ClassType* pSourceType = pObject? pObject->GetType(): NULL;

						if (pObject)
						{
						//	Class* psrc = GetType(pObject);	// debug
						//	ASSERT(psrc->m_qname == ((Class*)pSourceType)->m_qname);	// debug
							
							properType = DynamicCast(pObject, pSourceType->GetClass()/*->GetPointerTo()*/, ArgType->GetClass());
						}
						else
							properType = NULL;
					}

					*(void**)p = properType;
					p += sizeof(void*);
					//dispatch->Invoke(_this, method, &properType, 4);
				}
				else
				{
					ASSERT(0);
				}
			}
			else
				ASSERT(0);
		}
#endif
		// TODO, convert to appropriate type

	//	*(void**)p = elt.m_variant.m_pStringA;
		//*(int*)p = elt.toInt();

		//switch (elt
	}

	if (pMethod->m_offset == -1)
	{
		raise(Exception("address of method '" + pDispatch->GetClass()->get_QName() + "::" + pMethod->ToString() + "' was not found"));
		ASSERT(0);
	}

	ptrdiff_t size = p - args;
	size = (size+3) & ~3;

	//long result = pDispatch->Invoke(object, pMethod, args, p - args);

	if (true)
	{
		DispatchMsg dmsg;
		dmsg.caller.m_hWaitEvent = CreateEventW(NULL, false, false, NULL);	// TODO, create once

		dmsg.m_dispatch = pDispatch;
		dmsg.m_object = object;
		dmsg.m_method = pMethod;
		dmsg.m_args = args;
		dmsg.m_args_nbytes = (uint)size;

		PostThreadMessageW(m_gui_threadId, WM_USER+1, (WPARAM)1, (LPARAM)&dmsg);
		WaitForSingleObject(dmsg.caller.m_hWaitEvent, INFINITE);
		CloseHandle(dmsg.caller.m_hWaitEvent);

		if (dmsg.m_exception)
		{
			throw dmsg.m_exception;
		}
	}
	else
	{
		void_invoke_method32(pMethod->m_offset, object, args, size);
	}

	/*
	if (bReturnValue)
	{
		if (pReturnType->GetKind() == type_void)
		{
			push(StkElt());	// undefined
		}
		else if (pReturnType->GetKind() == type_int)
		{
			push(StkElt(result));
		}
		else if (pReturnType->GetKind() == type_pointer)
		{
			if (pReturnType->GetPointerTo() == typeid(StringA).GetType())
				push(StkElt((StringA*)result));
			else
				push(StkElt((Object*)result));
		}
		else
		{
			ASSERT(0);
		}
	}
	*/
}

void VM::as_NewObject(unsigned int length)
{
	StkElt name0 = pop();
	int numargs = pop().ToInt();
	if (numargs < 0)
		raise(Exception(L"numargs < 0"));

	String name = name0.ToStringA();

//	char fullname[512];
//	sprintf_s(fullname, "System::UI::%s", CString(name).c_str());

	auto types_it = TypeDatabase::pD->m_namedTypesM.find(name);
	if (types_it != TypeDatabase::pD->m_namedTypesM.end())
	{
		NamedType* pType = types_it->second;

		if (pType->get_Kind() != type_class)
		{
			raise(Exception(L"Not a class type"));
		}
		ClassType* pClass = (ClassType*)pType;

		vector<StkElt> args;
		args.reserve(numargs);
		for (int i = 0; i < numargs; i++)
		{
			args.push_back(pop());
		}

		Dispatch* pDispatch = GetDispatch(pClass);

		String className = pClass->m_name;

	//	void* _this = object;

		// Try all constructors, if more than one constructor matches, there's an ambiguity
		auto it = pDispatch->m_methodnames.find(className);
		if (it != pDispatch->m_methodnames.end())
		{
			String cname;
			Declarator* pMethod = NULL;
			do
			{
				ASSERT((*it).second->m_pType != NULL);
				ASSERT((*it).second->m_pType->GetFunction() != NULL);

				FunctionType* pFunctionType = (*it).second->m_pType->GetFunction();

				/*
				if (pFunctionType->m_parameters.m_parameters.size() == args.size())
				{
					if (pMethod)
						throw -1;

					pMethod = (*it).second;//func = pMethod->m_func;
				}
				*/

				if (args.size() == pFunctionType->m_parameters.m_parameters.size())
				{
					unsigned int i;

					for (i = 0; i < args.size(); i++)
					{
						if (i == pFunctionType->m_parameters.m_parameters.size())
							break;	// Too many arguments given, so doesn't match

						Type* argType = pFunctionType->m_parameters.m_parameters[i].m_pType->GetStripped();

						if (IsCompatible(args[i], argType))
							continue;

						break;	// Argument wasn't compatible
					}

					if (i == args.size())
					{
						if (pMethod)
						{
							raise(Exception(L"ambiguity"));
							ASSERT(0);
						}

						pMethod = (*it).second;
					}
				}
				/*
					//Method* pMethod = (*it).second;
					if (->m_parameters.m_parameters.size() == 0)
					{
						pMethod = (*it).second;//func = pMethod->m_func;
						break;
					}
				*/

				++it;
				cname = it->first;
			}
			while (cname == className);

			if (pMethod)
			{
				uint8* object = (uint8*)allocate_object(pClass->get_sizeof(), NULL);
				if (object == NULL)
				{
					raise(Exception(L"Couldn't allocate object"));
					ASSERT(0);
				}

				CallMethod(pDispatch, object, pMethod, args, false);
				//Object* p = newobj(pType);

				push(StkElt((Object*)object));
			}
			else
			{
				raise(Exception(L"TODO: message"));
				ASSERT(0);
			}

		}
	}
	else
	{
		raise(Exception("'" + name + "' not found"));
	}
}

void VM::as_CallFunction(unsigned int length)
{
	String name = pop().ToString();
	int numargs = pop().ToInt();
	if (numargs < 0)
		raise(Exception("CallFunction: Invalid number of arguments"));

	StkElt obj = m_pFrame->m_variables.find(name)->second;
	if (obj.m_variant.kind == VARKIND_OBJECT)
	{
		Function* fun = dynamic_cast<Function*>(obj.m_variant.m_pObject);
		ASSERT(fun);

		StackFrame* frame = new StackFrame;
		frame->m_pContext = m_pFrame->m_pContext;
		frame->m_pCaller = m_pFrame;
		frame->m_return_pc = m_pc;	// TODO, not this ??
	//	frame->m_instructionsData = fun->m_codeData;
	//	frame->m_instructionsLength = fun->m_codeLength;

		fun->dispatch(this, frame, numargs);

		m_pFrame = frame;
	}
	else
	{
		auto it = TypeDatabase::pD->m_procs.find(name);
		if (it != TypeDatabase::pD->m_procs.end())
		{
			auto it2 = it;

			bool found = false;

			do
			{
				FunctionType* pFunctionType = it2->second->m_decl->m_pType->GetFunction();

				if (numargs == pFunctionType->m_parameters.m_parameters.size())
				{
					int i;

					for (i = 0; i < numargs; ++i)
					{
						if (i == pFunctionType->m_parameters.m_parameters.size())
							break;	// Too many arguments given, so doesn't match

						const StkElt& arg = *(m_stack.end()-1-i);
						Type* argType = pFunctionType->m_parameters.m_parameters[i].m_pType->GetStripped();

						if (IsCompatible(arg, argType))
							continue;

						break;	// Argument wasn't compatible
					}

					if (i == numargs)
					{
						found = true;
						// TODO ambiguity
						break;
					}
				}

				++it2;
			}
			while (it2 != TypeDatabase::pD->m_procs.end() && it2->first == name);

			if (found)
			{
				ubyte args[512];
				ubyte* p = GetArgs(it2->second->m_decl->m_pType->GetFunction(), args, numargs);

				int funcspec = it2->second->m_decl->m_pType->GetFunction()->m_funcspec;

				Type* pReturnType = it2->second->m_decl->m_pType->GetFunction()->get_ReturnType()->GetStripped();

				switch (pReturnType->get_Kind())
				{
				case type_void:
					{
						if (funcspec == 2)	// __stdcall
							void_invoke_function_stdcall(it2->second->m_decl->m_offset, args, p - args);
						else
							void_invoke_function_cdecl(it2->second->m_decl->m_offset, args, p - args);
					}
					break;

				case type_bool:
					{
						bool result;
						if (funcspec == 2)	// __stdcall
							result = int32_invoke_function_stdcall(it2->second->m_decl->m_offset, args, p - args) & 1;
						else
							result = int32_invoke_function_cdecl(it2->second->m_decl->m_offset, args, p - args) & 1;

						push(result);
					}
					break;

				case type_char:
				case type_signed_char:
				case type_unsigned_char:

				case type_wchar_t:

				case type_short:
				case type_unsigned_short:

				case type_int:
				case type_unsigned_int:
					{
						int result;
						if (funcspec == 2)	// __stdcall
							result = int32_invoke_function_stdcall(it2->second->m_decl->m_offset, args, p - args);
						else
							result = int32_invoke_function_cdecl(it2->second->m_decl->m_offset, args, p - args);

						push(result);
					}
					break;

					/*
				case type_float:
					{
						float result;
						if (funcspec == 2)	// __stdcall
							result = float_invoke_function_stdcall(it2->second->m_decl->m_offset, args, p - args);
						else
							result = float_invoke_function_cdecl(it2->second->m_decl->m_offset, args, p - args);

						push(result);
					}
					break;

				case type_double:
					{
						double result;
						if (funcspec == 2)	// __stdcall
							result = double_invoke_function_stdcall(it2->second->m_decl->m_offset, args, p - args);
						else
							result = double_invoke_function_cdecl(it2->second->m_decl->m_offset, args, p - args);

						push(result);
					}
					break;
					*/

				case type_pointer:
				case type_reference:
					{
						Type* pPointerTo = pReturnType->GetPointerTo()->GetStripped();
						if (pPointerTo->get_Kind() == type_class)
						{
							ClassType* pClass = pPointerTo->GetClass();

							void* result;

							if (true)
							{
								DispatchMsg dmsg;
								dmsg.caller.m_hWaitEvent = CreateEventW(NULL, false, false, NULL);	// TODO, create once

								dmsg.m_func = it2->second->m_decl;
								dmsg.m_args = args;
								dmsg.m_args_nbytes = (uint)(p - args);

								PostThreadMessageW(m_gui_threadId, WM_USER+1, (WPARAM)3, (LPARAM)&dmsg);
								WaitForSingleObject(dmsg.caller.m_hWaitEvent, INFINITE);
								CloseHandle(dmsg.caller.m_hWaitEvent);

								if (dmsg.m_exception)
								{
									throw dmsg.m_exception;
								}

								result = dmsg.retval_struct;
							}
							else
							{
								if (funcspec == 2)	// __stdcall
									result = pointer_invoke_function_stdcall(it2->second->m_decl->m_offset, args, p - args);
								else
									result = pointer_invoke_function_cdecl(it2->second->m_decl->m_offset, args, p - args);
							}

							if (pClass->HasVirtualTable())
							{
							// TODO, check address to know if it's one of our objects

								Object* obj = reinterpret_cast<Object*>(DynamicCast(result, GetType(result), typeof(Object)->GetClass()));

								if (obj)
								{
									push(obj);
								}
								else
								{
									// TODO, 64 bit
									push((int32)result);
								}
							}
							else
							{
								// TODO, 64 bit
								push((int32)result);
							}
						}
						else
						{
							raise(Exception("pointer to != class or interface"));
						}
					}
					break;

				default:
					raise(Exception("rettype"));
				}
			}
			else
			{
				raise(Exception(L"function signature not found"));
			}
		}
		else
		{
			auto typit = TypeDatabase::pD->m_namedTypesM.find(name);
			if (typit != TypeDatabase::pD->m_namedTypesM.end())
			{
				vector<StkElt> args;
				args.reserve(numargs);
				for (int i = 0; i < numargs; ++i)
				{
					args.push_back(pop());
				}

				ClassType* pClassType = dynamic_cast<ClassType*>(typit->second->GetStripped());
				if (pClassType == NULL)
				{
					raise(Exception("'" + name + L"' not a function or class"));
				}

				String className = pClassType->m_name;

				Dispatch* pDispatch = GetDispatch(pClassType);

				// Try all constructors, if more than one constructor matches, there's an ambiguity
				auto it = pDispatch->m_methodnames.find(className);
				if (it != pDispatch->m_methodnames.end())
				{
					String cname;
					Declarator* pMethod = NULL;
					do
					{
						ASSERT((*it).second->m_pType != NULL);
						ASSERT((*it).second->m_pType->GetFunction() != NULL);

						FunctionType* pFunctionType = (*it).second->m_pType->GetFunction();

						/*
						if (pFunctionType->m_parameters.m_parameters.size() == args.size())
						{
							if (pMethod)
								throw -1;

							pMethod = (*it).second;//func = pMethod->m_func;
						}
						*/


						if (args.size() == pFunctionType->m_parameters.m_parameters.size())
						{
							unsigned int i;

							for (i = 0; i < args.size(); i++)
							{
								if (i == pFunctionType->m_parameters.m_parameters.size())
									break;	// Too many arguments given, so doesn't match

								Type* argType = pFunctionType->m_parameters.m_parameters[i].m_pType->GetStripped();

								if (IsCompatible(args[i], argType))
									continue;

								break;	// Argument wasn't compatible
							}

							if (i == args.size())
							{
								if (pMethod)
								{
									raise(Exception(L"ambiguity"));
									ASSERT(0);
								}

								pMethod = (*it).second;
							}
						}
						/*
							//Method* pMethod = (*it).second;
							if (->m_parameters.m_parameters.size() == 0)
							{
								pMethod = (*it).second;//func = pMethod->m_func;
								break;
							}
						*/

						++it;
						cname = it->first;
					}
					while (cname == className);

					if (pMethod)
					{
						uint8* block = (uint8*)allocate_object(pClassType->get_sizeof(), NULL);

						CallMethod(pDispatch, block, pMethod, args, false);

						push(StkElt(block, pClassType));
					}
					else
					{
						raise(Exception(L"TODO: message"));
						ASSERT(0);
					}
				}
			}
			else
			{
				raise(Exception(L"Function '" + name + "' not found"));
			}
		}

#if 0
		ClassType* pType = m_global->GetType();
		Dispatch* pDispatch = GetDispatch(pType);//->m_pDispatch;

		Method* pMethod = pDispatch->GetMethod(methodName);
		VERIFY(pMethod);

		CallMethod(pDispatch, m_global, pMethod, numargs, true);
#endif
	}

	/*
	ActionScriptFunction* fun = m_ctx->m_pContext->m_funmap[name.ToString()];

	fun->dispatch(this, m_ctx, numargs);
	*/

	/*
	StackFrame* frame = new StackFrame;
	frame->m_pContext = m_ctx->m_pContext;
	frame->m_pCaller = m_ctx;
	frame->m_return_pc = pc;	// TODO, not this ??
	frame->m_instructionsData = fun->m_codeData;
	frame->m_instructionsLength = fun->m_codeLength;

	m_ctx = frame;
	pc = frame->m_instructionsData;
	*/

	//push(StkElt(0L));
}

void VM::as_CallMethod(unsigned int length)
{
	StkElt methodName = pop();
	StkElt object = pop();
	int numargs = pop().ToInt();
	if (numargs < 0)
		raise(Exception(L"Invalid number of arguments"));

	if (object.m_variant.kind != VARKIND_OBJECT)
	{
		raise(Exception(L"Expected object"));
	}

	if (true)
	{
		if (NamespaceType* pName = dynamic_cast<NamespaceType*>(object.m_variant.m_pObject))
		{
			Dispatch* pDispatch = GetDispatch(pName);

			String methodNameStr = methodName.ToStringA();
			Declarator* pMethod = pDispatch->GetMethod(methodNameStr);
			if (pMethod == NULL)
			{
				Declarator* pMethod = pDispatch->GetMethod(methodNameStr);
				raise(Exception(L"method " + methodNameStr + " not found"));
			}

			CallMethod(pDispatch, NULL, pMethod, numargs, true);
		}
		else
		{
			ClassType* pType = object.m_variant.m_pObject->GetType();
			Dispatch* pDispatch = GetDispatch(pType);

			String methodNameStr = methodName.ToStringA();
			auto it = pDispatch->m_methodnames.find(methodNameStr);

			if (it == pDispatch->m_methodnames.end())
			{
				Declarator* pMethod = pDispatch->GetMethod(methodNameStr);
				raise(Exception(L"method '" + pDispatch->GetClass()->get_QName() + "." + methodNameStr + "' not found"));
			}

			Declarator* pMethod = NULL;

			do
			{
				FunctionType* pFunctionType = it->second->m_pType->GetFunction();
				if (pFunctionType->m_parameters.m_parameters.size() == numargs)
				{
					uint i;

					for (i = 0; i < numargs; ++i)
					{
						if (i == pFunctionType->m_parameters.m_parameters.size())
							break;	// Too many arguments given, so doesn't match

						StkElt& arg = *(m_stack.end()-1-i);

						Type* argType = pFunctionType->m_parameters.m_parameters[i].m_pType->GetStripped();

						if (IsCompatible(arg, argType))
							continue;

						break;	// Argument wasn't compatible
					}

					if (i == numargs)	// All the args were compatible
					{
						if (pMethod)
						{
							raise(Exception(L"ambiguity"));
							ASSERT(0);
						}

						pMethod = it->second;
					}
				}

				++it;
			}
			while (it->first == methodNameStr);

			if (pMethod)
			{
			//	pMethod->m_decl->m_offset = 236;
				CallMethod(pDispatch, object.m_variant.m_pObject, pMethod, numargs, true);
			}
			else
			{
				raise(Exception(L"no method found with correct arguments"));
			}
		}
	}
	else
	{
		IMembersMap* pob = dynamic_cast<IMembersMap*>(object.m_variant.m_pObject);

		StkElt method = pob->GetMember(methodName.ToStringA());

		if (method.m_variant.kind != VARKIND_OBJECT)
		{
			raise(Exception("Expected object"));
		}

		Function* f = dynamic_cast<Function*>(method.m_variant.m_pObject);

		int numargs = pop().ToInt();

		StackFrame* pFrame = new StackFrame;

	#if 0
		ctx->m_variables["this"] = object.m_variant.u.m_pObject;

		//std::vector<

		for (int i = 0; i < numargs; i++)
		{
			ctx->m_variables[f->m_args[i]] = pop();
			/*
			StkElt elt = pop();
			if (i > 0) TRACE(", ");

			TRACE("%s", elt.ToString().c_str());
			*/
		}

		TRACE(")");
	#endif
		pFrame->m_pCaller = m_pFrame;
		f->dispatch(this, pFrame, numargs);
		/*
		ctx->m_pContext = f->m_pContext;
		ctx->m_pCaller = m_ctx;
		ctx->m_return_pc = pc;
		ctx->m_instructionsData = f->m_codeData;
		ctx->m_instructionsLength = f->m_codeLength;
	//	ctx->m_constantPool = m_ctx->m_constantPool;	// inherited ?
		pc = ctx->m_instructionsData;
		m_ctx = ctx;

	//	PrintActions(pc, f->m_codeLength);

		if (pc - m_ctx->m_instructionsData >= m_ctx->m_instructionsLength)
		{
			pc = m_ctx->m_return_pc;
			m_ctx = m_ctx->m_pCaller;
		}
		*/

		/*
		uint8* oldpc = pc;
		pc = pFunction->m_codeData;

		while (next())
			;

		pc = oldpc;
		*/
	}
}

void VM::as_GetVariable(unsigned int length)
{
	String name = pop().ToString();
	StkElt value;

	DebugTrace("GetVariable " << name << "\n");

	// TODO, improve.. I'm just winging
	StackFrame* frame = m_pFrame;
	do
	{
		map<String, StkElt>::iterator it = frame->m_variables.find(name);
		if (it != frame->m_variables.end())
		{
			value = it->second;
			break;
		}
		frame = frame->m_pCaller;
	}
	while (frame);

	if (frame == NULL)	// At root
	{
		auto it = TypeDatabase::pD->m_namedTypesM.find(name);
		if (it != TypeDatabase::pD->m_namedTypesM.end())
		{
			auto it2 = it;
			++it2;

			if (it2 != TypeDatabase::pD->m_namedTypesM.end() && it2->first == name)
			{
				NamedTypeMultiMapCollection* coll = new NamedTypeMultiMapCollection(TypeDatabase::pD->m_namedTypesM);
				coll->m_key = name;
			//	coll->m_map = pD->m_namedTypesM;
				coll->m_begin = it;
			//	coll->m_it = it;

				value = coll;
			}
			else
			{
				value = it->second;
			}
		}
		else
		{
			auto it = TypeDatabase::pD->m_procs.find(name);
			if (it != TypeDatabase::pD->m_procs.end())
			{
				auto it2 = it;
				++it2;

				if (it2 != TypeDatabase::pD->m_procs.end() && it2->first == name)
				{
					FunctionObjectMultiMapCollection* coll = new FunctionObjectMultiMapCollection(TypeDatabase::pD->m_procs);
					coll->m_key = name;
				//	coll->m_map = pD->m_procs;
					coll->m_begin = it;
				//	coll->m_it = it;

					value = coll;
				}
				else
				{
					value = it->second;
				}
			}
			else
			{
				raise(Exception("'" + name + L"' not found"));
				ASSERT(0);
			}
		}
	}

	push(value);

	DebugTrace(name << "=>" << value.ToString());
}

void VM::as_SetVariable(unsigned int length)
{
	StkElt value = pop();
	String name = pop().ToStringA();

	// TODO, improve.. I'm just winging
	StackFrame* frame = m_pFrame;
	do
	{
		auto it = frame->m_variables.find(name);
		if (it != frame->m_variables.end())
		{
			if (OnSetVariable)
			{
				OnSetVariable(frame, name, value);
			}

			it->second = value;
			break;
		}
		frame = frame->m_pCaller;
	}
	while (frame);

	if (frame == NULL)
	{
		// TODO, set global variable
	//	m_pFrame->m_variables[name.ToStringA()] = value;
	}

//	TRACE(" %s <= %s", CString(name.ToStringA()).c_str(), CString(value.ToStringA()).c_str());
	DebugTrace(name << "<=" << value.ToString());
}

void VM::as_NextFrame(unsigned int length)
{
	StkElt elt = m_pFrame->m_variables[String("this")];

	ASSERT(0);
#if 0
	PlacedCharacter* obj = dynamic_cast<PlacedCharacter*>(elt.m_variant.u.m_pObject);
	obj->m_gotoFrame++;
#endif
}

void VM::as_Play(unsigned int length)
{

}

void VM::as_SetTarget(unsigned int length)
{
	CString target((const char*)m_pc, length);

	DebugTrace("\"" << target.c_str() << "\"");
}

void VM::as_Typeof(unsigned int length)
{
	StkElt value = pop();
#if 0
	if (value.m_variant.kind == VARKIND_NULL)
	{
		push(StkElt("null"));
	}
	else if (value.m_variant.kind == VARKIND_UNDEFINED)
	{
		push(StkElt("undefined"));
	}
	else if (value.m_variant.kind == VARKIND_BOOL)
	{
		push(StkElt("boolean"));
	}
	else if (value.m_variant.kind == VARKIND_OBJECT)
	{
		push(StkElt("object"));
	}
	else if (	value.m_variant.kind == VARKIND_INT32 ||
				value.m_variant.kind == VARKIND_FLOAT32 ||
				value.m_variant.kind == VARKIND_FLOAT64)
	{
		push(StkElt("number"));
	}
	else
	{
		raise(Exception("unknown varkind"));
	}
#endif
}

VM::tyasfunc VM::asfuncs[256] =
{
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_NextFrame,//"ActionNextFrame = 0x04",
	&VM::as_notimpl,//"ActionPrevFrame = 0x05",
	&VM::as_Play,//"ActionPlay = 0x06",
	&VM::as_notimpl,//"ActionStop = 0x07",
	&VM::as_notimpl,//"ActionToggleQuality = 0x08",
	&VM::as_notimpl,//"ActionStopSounds = 0x09",
	&VM::as_notimpl,//"ActionAdd = 0x0A",
	&VM::as_notimpl,//"ActionSubtract = 0x0B",
	&VM::as_notimpl,//"ActionMultiply = 0x0C",
	&VM::as_notimpl,//"ActionDivide = 0x0D",
	&VM::as_notimpl,//"ActionEquals = 0x0E",
	&VM::as_notimpl,//"ActionLess = 0x0F",
	&VM::as_notimpl,//"ActionAnd = 0x10",
	&VM::as_notimpl,//"ActionOr = 0x11",
	&VM::as_notimpl,//"ActionNot = 0x12",
	&VM::as_notimpl,//"ActionStringEquals = 0x13",
	&VM::as_notimpl,//"ActionStringLength = 0x14",
	&VM::as_notimpl,//"ActionStringExtract = 0x15",
	&VM::as_notimpl,//NULL,
	&VM::as_Pop,//"ActionPop = 0x17",
	&VM::as_notimpl,//"ActionToInteger = 0x18",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_GetVariable,//"ActionGetVariable = 0x1C",
	&VM::as_SetVariable,//"ActionSetVariable = 0x1D",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//"ActionSetTarget2 = 0x20",
	&VM::as_StringAdd,//"ActionStringAdd = 0x21",
	&VM::as_notimpl,//"ActionGetProperty = 0x22",
	&VM::as_notimpl,//"ActionSetProperty = 0x23",
	&VM::as_notimpl,//"ActionCloneSprite = 0x24",
	&VM::as_notimpl,//"ActionRemoveSprite = 0x25",
	&VM::as_notimpl,//"ActionTrace = 0x26",
	&VM::as_notimpl,//"ActionStartDrag = 0x27",
	&VM::as_notimpl,//"ActionEndDrag = 0x28",
	&VM::as_notimpl,//"ActionStringLess = 0x29",
	&VM::as_notimpl,//"ActionThrow = 0x2A",
	&VM::as_notimpl,//"ActionCastOp = 0x2B",
	&VM::as_notimpl,//"ActionImplementsOp = 0x2C",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//"ActionRandomNumber = 0x30",
	&VM::as_notimpl,//"ActionMBStringLength = 0x31",
	&VM::as_notimpl,//"ActionCharToAscii = 0x32",
	&VM::as_notimpl,//"ActionAsciiToChar = 0x33",
	&VM::as_notimpl,//"ActionGetTime = 0x34",
	&VM::as_notimpl,//"ActionMBStringExtract = 0x35",
	&VM::as_notimpl,//"ActionMBCharToAscii = 0x36",
	&VM::as_notimpl,//"ActionMBAsciiToChar = 0x37",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//"ActionDelete = 0x3A",
	&VM::as_notimpl,//"ActionDelete2 = 0x3B",
	&VM::as_notimpl,//"ActionDefineLocal = 0x3C",
	&VM::as_CallFunction,//"ActionCallFunction = 0x3D",
	&VM::as_notimpl,//"ActionReturn = 0x3E",
	&VM::as_notimpl,//"ActionModulo = 0x3F",
	&VM::as_NewObject,//"ActionNewObject = 0x40",
	&VM::as_DefineLocal2,//"ActionDefineLocal2 = 0x41",
	&VM::as_notimpl,//"ActionInitArray = 0x42",
	&VM::as_notimpl,//"ActionInitObject = 0x43",
	&VM::as_Typeof,//"ActionTypeOf = 0x44",
	&VM::as_notimpl,//"ActionTargetPath = 0x45",
	&VM::as_notimpl,//"ActionEnumerate = 0x46",
	&VM::as_Add2,//"ActionAdd2 = 0x47",
	&VM::as_Less2,//"ActionLess2 = 0x48",
	&VM::as_notimpl,//"ActionEquals2 = 0x49",
	&VM::as_notimpl,//"ActionToNumber = 0x4A",
	&VM::as_notimpl,//"ActionToString = 0x4B",
	&VM::as_notimpl,//"ActionPushDuplicate = 0x4C",
	&VM::as_notimpl,//"ActionStackSwap = 0x4D",
	&VM::as_GetMember,//"ActionGetMember = 0x4E",
	&VM::as_SetMember,//"ActionSetMember = 0x4F",
	&VM::as_notimpl,//"ActionIncrement = 0x50",
	&VM::as_notimpl,//"ActionDecrement = 0x51",
	&VM::as_CallMethod,//"ActionCallMethod = 0x52",
	&VM::as_notimpl,//"ActionNewMethod = 0x53",
	&VM::as_notimpl,//"ActionInstanceOf = 0x54",
	&VM::as_notimpl,//"ActionEnumerate2 = 0x55",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//"ActionBitAnd = 0x60",
	&VM::as_notimpl,//"ActionBitOr = 0x61",
	&VM::as_notimpl,//"ActionBitXor = 0x62",
	&VM::as_notimpl,//"ActionBitLShift = 0x63",
	&VM::as_notimpl,//"ActionBitRShift = 0x64",
	&VM::as_notimpl,//"ActionBitURShift = 0x65",
	&VM::as_notimpl,//"ActionStrictEquals = 0x66",
	&VM::as_Greater,//"ActionGreater = 0x67",
	&VM::as_notimpl,//"ActionStringGreater = 0x68",
	&VM::as_notimpl,//"ActionExtends = 0x69",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//"ActionGotoFrame = 0x81",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//"ActionGetURL = 0x83",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//"ActionStoreRegister = 0x87",
	&VM::as_ConstantPool,//"ActionConstantPool = 0x88",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//"ActionWaitForFrame = 0x8A",
	&VM::as_SetTarget,//"ActionSetTarget = 0x8B",
	&VM::as_notimpl,//"ActionGoToLabel = 0x8C",
	&VM::as_notimpl,//"ActionWaitForFrame2 = 0x8D",
	&VM::as_notimpl,//"ActionDefineFunction2 = 0x8E",
	&VM::as_notimpl,//"ActionTry = 0x8F",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_With,//"ActionWith = 0x94",
	&VM::as_notimpl,//NULL,
	&VM::as_Push,//"ActionPush = 0x96",
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//NULL,
	&VM::as_notimpl,//"ActionJump = 0x99",
	&VM::as_notimpl,//"ActionGetURL2 = 0x9A",
	&VM::as_DefineFunction,//"ActionDefineFunction = 0x9B",
	&VM::as_notimpl,//NULL,
	&VM::as_If,//"ActionIf = 0x9D",
	&VM::as_notimpl,//"ActionCall = 0x9E",
	&VM::as_notimpl,//"ActionGotoFrame2 = 0x9F",
};

bool VM::next()
{
	if (m_pFrame == NULL)
	{
		return false;
	}

	if (m_flags & 1)	// single-step
	{
		m_breakpoint(this, m_userobject, 2);
	}

	if (m_pc - m_pFrame->m_instructionsData >= m_pFrame->m_instructionsLength)
	{
		m_pc = m_pFrame->m_return_pc;
		m_pFrame = m_pFrame->m_pCaller;

#if 0
		// ????
		// push function/method result onto stack
		push(0L);
#endif

		m_level--;
		return true;
		/*
		if (m_ctx == NULL)
			return false;
			*/
	}

	uint8* p = m_pc;
	uint8 ActionCode = *p++;

	if (ActionCode == 0)
		return false;

#if 0
	if (instr_code == 0x1B)	// else
	{
		level--;
		printlevel();
		LOG("else\n");
		level++;
		pc++;

		while (1)
		{
			uint8 instr_code = *pc;
			if (instr_code == 0x59)	// eif
				break;

			skip_next(this);
		}

		ASSERT(*pc == 0x59);	// eif
		return true;//break;
	}

	if (instr_code == 0x59)	// eif
	{
	}

	printlevel();
#endif

	ULONG Length = 0;
	if (ActionCode >= 0x80)
	{
		Length = *p++;
		Length |= (long)(*p++)<<8;
	}
	m_pc = p;
//	TRACE("%s - 0x%x ", ActionNames[ActionCode], ActionCode);
	(this->*asfuncs[ActionCode])(Length);

#ifdef _DEBUG
	if (ActionCode != 0x9B)
	{
	;//	ASSERT(pc == p+Length);
	}

//	pc = p + Length;
#endif

	TRACE("\n");

	return true;
}

/*
class Instructions
{
public:
	uint8* m_data;
	ULONG m_length;

//	std::vector<int> m_offset;
};
*/

/*
void PrintActions(uint8* p, ULONG length)
{
	uint8 ActionCode;
	while ((ActionCode = *p++) != 0)
	{
		TRACE("%s - 0x%x\n", ActionNames[ActionCode], ActionCode);
		uint16 Length = 0;
		if (ActionCode >= 0x80)
		{
			Length = *p++;
			Length |= (long)(*p++)<<8;
		}

		uint8* start = p;
		p = start+Length;
	}
}
*/

}
}
