#include "StdAfx.h"
#include "Comp2.h"
#include "LFC/disasm.h"

#include "coff.h"	// for IMAGE_SYMBOL

#include "m68kassem.h"
#include "ppcassem.h"
#include "AOutWrite.h"
#include "ir.h"
#include "codegen.h"
#include "stab.h"
#include "cpp_compile.h"

using namespace std;

namespace System
{

//#include "a_tree.h"

void fput8(FILE* fp, uint8 v);
void fput16(FILE* fp, uint16 v);
void fput32(FILE* fp, uint32 v);

int32 fget32(FILE* fp)
{
	int32 v;
	fread(&v, 1, 4, fp);
	return BigEndian32(v);
}

char *ltoa( long value, char *string, int radix )
{
	ASSERT(0);
	return NULL;
}

//namespace cpp
//{

void assem(x86instr* assem, CFunctionCode* pFun, FILE* fileout);
void assem(m68kinstr* assem, CFunctionCode* pFun, FILE* fileout);
void assem(ppcinstr* assem, /*CFunctionCode* pFun,*/ FILE* fileout);

map<System::Type*,int> types;

IO::TextWriter& stab_type(System::Type* pType, int& n, IO::TextWriter& strbuilder)
{
	//String str;

	if (pType->get_Kind() == type_int)
	{
		strbuilder << "1";
	}
	else if (pType->get_Kind() == type_char)
	{
		strbuilder << "2";
	}
	else if (pType->get_Kind() == type_void)
	{
		strbuilder << "3";
	}
	else
	{
////////////
		int typeNumber = types[pType];
		if (typeNumber == 0)
		{
			types[pType] = ++n;

		//	char buf[33];
		//	ltoa(n, buf, 10);

			strbuilder << n;// = *str + buf;
			strbuilder << "=";

			if (pType->get_Kind() == type_pointer)
			{
				strbuilder << "*";
				stab_type(pType->GetPointerTo(), n, strbuilder);
			}
			else if (pType->get_Kind() == type_reference)
			{
				strbuilder << "&";
				stab_type(pType->GetPointerTo(), n, strbuilder);
			}
			else if (pType->get_Kind() == type_cv)
			{
				if (pType->IsConst()) strbuilder << "k";

				if (pType->IsVolatile()) strbuilder << "B";

				stab_type(pType->GetPointerTo(), n, strbuilder);
			}
			else if (pType->get_Kind() == type_class)
			{
				strbuilder << "s";
				//ltoa(pType->get_sizeof(), buf, 10);
				//str = *str + buf;
				strbuilder << pType->get_sizeof();

				ClassType* pClassType = pType->AsClass(); 

				for (size_t i = 0; i < pClassType->m_pScope->m_orderedDecls.size(); ++i)
				{
					Declarator* pDecl = pClassType->m_pScope->m_orderedDecls[i];

					strbuilder << pDecl->m_name;
					strbuilder << ":";

					int typeNumber = types[pDecl->m_pType];
					if (typeNumber == 0)
					{
					//	types[pDecl->m_pType] = n;
					//	ltoa(++n, buf, 10);

						stab_type(pDecl->m_pType, n, strbuilder);

					//	str += buf;
					}
					else	// type-reference
					{
						//ltoa(typeNumber, buf, 10);
						//str = *str + buf;
						strbuilder << typeNumber;
					}

					strbuilder << ",";

					//ltoa(pDecl->get_bitOffset(), buf, 10);
					//str = *str + buf;
					strbuilder << pDecl->get_bitOffset();

					strbuilder << ",";

					//ltoa(pDecl->get_bitSize(), buf, 10);
					//str = *str + buf;
					strbuilder << pDecl->get_bitSize();

					strbuilder << ";";
				}

				strbuilder << ";";
			}
			else if (pType->get_Kind() == type_enum)
			{
				strbuilder << "e";

				EnumType* pEnum = (EnumType*)pType;

				for (size_t i = 0; i < pEnum->m_deflist.GetSize(); ++i)
				{
					const EnumDef& enumDef = pEnum->m_deflist[i];

					strbuilder << enumDef.m_name;
					strbuilder << ":";

					//ltoa(enumDef->m_value, buf, 10);
					//str = *str + buf;
					strbuilder << enumDef.m_value;

					strbuilder << ",";
				}

				strbuilder << ";";
			}
			else
				ASSERT(0);
		}
		else	// reference
		{
		//	char buf[33];
		//	ltoa(typeNumber, buf, 10);
		//	str = *str + buf;
			strbuilder << typeNumber;
		}
	}

	return strbuilder;
}

////////////////////////////////////////////////////////////
// CAOut

int CAOut::AddSymbol(const char* symbolName, FSymbol** ppSymbol)
{
	FSymbol* pSymbol = new FSymbol;
	memset(pSymbol, 0, sizeof(IMAGE_SYMBOL));

	pSymbol->N.Name = _strdup(symbolName);

//	nlist* pSymbol = new nlist;
	//memset(pSymbol, 0, sizeof(nlist));

#if 0
	int len = strlen(symbolName);

	/*
	if (len <= 7)	// must also have room for 0-termination
	{
		strcpy((char*)pSymbol->N.ShortName, symbolName);
	}
	else
	*/
	{
		if (m_SizeOfStringTable+len+1 >= m_StringTableBytes)
		{
			while (m_SizeOfStringTable+len+1 >= m_StringTableBytes)
			{
				m_StringTableBytes += 256;
			}

			m_StringTable = (char*)realloc(m_StringTable, m_StringTableBytes);
			if (m_StringTable == NULL)
			{
				return -1;
			}
		}
		pSymbol->N.Name.Long = m_SizeOfStringTable;
		memcpy(m_StringTable + m_SizeOfStringTable, symbolName, len+1);
		m_SizeOfStringTable += len+1;
	}

#endif
	m_symbols.Add(pSymbol);

	*ppSymbol = pSymbol;

	return m_symbols.GetSize();
}

//extern AS_instrList* instrList;

int CAOut::Create(FILE* fileout, cpp::CGlobal* pGlobal, Scope* pGlobalScope, const vector<T_Declarator*> &globals, CFunctionCode* pFunctionCode, int cpu_id)
{
	exec execHeader;
	memset(&execHeader, 0, sizeof(execHeader));

	if (cpu_id == CPU_M68K)
	{
		execHeader.a_midmag = A_MAGIC;
		if (true)	// 68020
		{
			execHeader.a_midmag |= (2<<16);
		}
	}
	else if (cpu_id == CPU_IX86)
		execHeader.a_midmag = I_MAGIC;
	else if (cpu_id == CPU_PPC)
		execHeader.a_midmag = Q_MAGIC;
	else
		ASSERT(0);

	fseek(fileout, sizeof(exec), SEEK_SET);	// Make room for header

//	std::vector<CDeclarator*> globals;

	int ntype = 3;
	{

		{
			FSymbol* pSymbol;
			AddSymbol("int:t1=r1;-2147483648;2147483647;", &pSymbol);
			pSymbol->Value = 0;
			pSymbol->Type = N_LSYM;
		}

		{
			FSymbol* pSymbol;
			AddSymbol("char:t2=r2;0;127;", &pSymbol);
			pSymbol->Value = 0;
			pSymbol->Type = N_LSYM;
		}

		{
			FSymbol* pSymbol;
			AddSymbol("void:t3=3", &pSymbol);
			pSymbol->Value = 0;
			pSymbol->Type = N_LSYM;
		}

		for (size_t i = 0; i < pGlobal->m_types.GetSize(); ++i)
		{
			Type* pType = pGlobal->m_types[i]->Normalized();


			char buffer[256];
			ASSERT(0);
			//StringBuffer(buffer, 255) << ((NamedType*)pType)->m_name << ":tT" << stab_type(pType, ntype, strbuilder) << '\0';

			FSymbol* pSymbol;
			AddSymbol(buffer, &pSymbol);
			pSymbol->Value = 0;//ftell(fileout) - TextOffset;
			pSymbol->Type = N_LSYM;
		}
	}

	for (size_t i = 0; i < globals.GetSize(); ++i)
	{
		T_Declarator* pDeclarator = globals[i];

		//ASSERT(0);
#if 0
		if (pDeclarator->m_mangledName->Length())
		{
			FSymbol* pSymbol;
			pDeclarator->m_tempLabel->m_symbolIndex = AddSymbol(pDeclarator->m_mangledName->c_str(), &pSymbol);
			pSymbol->m_tempLabel = pDeclarator->m_tempLabel;
		}
#endif
	}

	// text
	long TextOffset = ftell(fileout);
	{
		for (size_t i = 0; i < globals.GetSize(); ++i)
		{
			//ASSERT(0);
#if 0
			CDeclarator* pDeclarator = globals[i];

			FSymbol* pSymbol = NULL;
			if (pDeclarator->m_tempLabel->m_symbolIndex > 0)
				pSymbol = m_symbols[pDeclarator->m_tempLabel->m_symbolIndex-1];
			/*
			pDeclarator->m_symbolIndex = AddSymbol(pDeclarator->m_mangledName.c_str(), &pSymbol);
			pSymbol->m_pDeclarator = pDeclarator;
			*/

			if (pSymbol)
			{
				if (pDeclarator->m_pType->GetType()->m_type == type_function)
				{
					if (pDeclarator->/*m_pDeclarator->*/m_defined)
						pSymbol->SectionNumber = 0;// text;
					else
						pSymbol->SectionNumber = -1;// undefined;
				}
				else
				{
					pSymbol->SectionNumber = 1;// data
					pSymbol->Value = execHeader.a_data;
#if 0
					pDeclarator->m_pDeclarator->m_spOffset = execHeader.a_data;	// ???
#endif

					ULONG declsize = pDeclarator->/*m_pDeclarator->*/m_pType->get_sizeof();
					ASSERT(declsize != -1);

					execHeader.a_data += declsize;
				}
			}
#endif
		}

		AS_instrList* il = pFunctionCode->instrList;

		while (il)
		{
			AS_instr* instr = il->head;

			if (instr->kind == AS_instr::I_LBRAC)
			{
				AS_instrList* il2 = il->tail;

				while (il2)
				{
					AS_instr* instr = il2->head;

					if (instr->kind == AS_instr::I_LSYM)
					{
#if 0
						FSymbol* pSymbol;

						//__live_object_ptr<StringA> str;
						BufferImp<char> buffer;
						StringBuilderA strbuilder(&buffer);
						strbuilder << *instr->fun->m_pDeclarator->m_name;
						strbuilder << ":";

						Type* pType = instr->fun->m_pDeclarator->m_pType->Normalized();
						/*
						int typeNumber = types[pType];
						if (typeNumber == 0)
						*/
						{
							strbuilder << "T";
							stab_type(pType, ntype, strbuilder);
						}
						/*
						else
						{
							char buf[33];
							ltoa(typeNumber, buf, 10);
							str += buf;
						}
						*/

						AddSymbol(buffer.DetachToString()->c_str(), &pSymbol);

						pSymbol->Type = N_LSYM;
						pSymbol->Value = instr->fun->m_pDeclarator->m_offset;
#endif
					}
					else if (instr->kind == AS_instr::I_LBRAC || instr->kind == AS_instr::I_RBRAC)
					{
						break;
					}

					il2 = il2->tail;
				}

#if 0
				FSymbol* pSymbol;
				AddSymbol(NULL, &pSymbol);
				pSymbol->Value = ftell(fileout) - TextOffset;
				pSymbol->Type = N_LBRAC;
#endif
			}
			else if (instr->kind == AS_instr::I_RBRAC)
			{
#if 0
				FSymbol* pSymbol;
				AddSymbol(NULL, &pSymbol);
				pSymbol->Value = ftell(fileout) - TextOffset;
				pSymbol->Type = N_RBRAC;
#endif
			}
			else if (instr->kind == AS_instr::I_ALIGN)
			{
				int value = ftell(fileout) - TextOffset;
				if (value & 1)
				{
					fputc(0, fileout);
					value++;
				}
			}
			else if (instr->kind == AS_instr::I_FUN)
			{
				/*
				const Type* pType = instr->fun->m_pDeclarator->m_pType->Normalized();

				//__live_object_ptr<StringA> str;
				BufferImp<char> buffer;
				StringBuilderA strbuilder(&buffer);

				strbuilder << *instr->fun->m_pDeclarator->m_name;
				strbuilder << ":";
				strbuilder << "f";
				stab_type(pType->GetFunction()->m_pReturnType->Normalized(), ntype, strbuilder);

				FSymbol* pSymbol;
				AddSymbol(buffer.DetachToString()->c_str(), &pSymbol);
				pSymbol->Value = ftell(fileout) - TextOffset;
				pSymbol->Type = N_FUN;
				*/

				char symbolName[256];
				ASSERT(0);
				//StringBuffer(symbolName, 255) << '_' << instr->fun->m_pDeclarator->m_name << '\0';

				FSymbol* pSymbol;
				instr->fun->m_tempLabel->m_symbolIndex = AddSymbol(symbolName, &pSymbol);
				if (instr->fun->m_tempLabel->m_defined)
				{
					int value = ftell(fileout) - TextOffset;
					pSymbol->Value = value;
					pSymbol->SectionNumber = 0;
				}
				else
					pSymbol->SectionNumber = -1;

				pSymbol->Type = 0;//N_EXT;//N_RBRAC;
			}
			else if (instr->kind == AS_instr::I_LINE)
			{
#if 0
				FSymbol* pSymbol;
				AddSymbol(NULL, &pSymbol);
				pSymbol->Value = ftell(fileout) - TextOffset;

				pSymbol->Type = N_SLINE;
				pSymbol->Desc = instr->LINE.num;
#endif
			}
			else if (instr->kind == AS_instr::I_SO)
			{
#if 0
				FSymbol* pSymbol;
				AddSymbol(instr->filename, &pSymbol);
				pSymbol->Value = ftell(fileout) - TextOffset;

				pSymbol->Type = N_SO;
#endif
			}
			else if (instr->kind == AS_instr::I_LABEL)
			{
				ASSERT(instr->LABEL.label->m_spOffset == -1);
				instr->LABEL.label->m_spOffset = ftell(fileout) - TextOffset;
			}
			else if (instr->kind == AS_instr::I_DATA)
			{
				fwrite(instr->DATA.buffer, instr->DATA.size, 1, fileout);
				//MessageBeep(-1);
			}
			else if (instr->kind == AS_instr::I_DATA2)
			{
				//fwrite(instr->DATA.buffer, instr->DATA.size, 1, fileout);
				T_ExpList* explist = instr->explist;

				while (explist)
				{
					T_Exp* e = explist->head;

					if (e->kind == T_Exp::T_CONST)
					{
						if (e->m_size == 1)
						{
							fputc(e->const_int.int32Val, fileout);
						}
						else if (e->m_size == 2)
						{
							fput16(fileout, e->const_int.int32Val);
						}
						else if (e->m_size == 4)
						{
							fput32(fileout, e->const_int.int32Val);
						}
					}
					else if (e->kind == T_Exp::T_NAME)
					{
						FRelocation* reloc = new FRelocation;
						reloc->VirtualAddress = ftell(fileout);
						reloc->tempLabel = e->name.label;
						pFunctionCode->AddRelocation(reloc);

						fput32(fileout, 0);
					}
					else if (e->kind == T_Exp::T_BINOP &&
						e->binop.op == '+' &&
						e->binop.left->kind == T_Exp::T_NAME &&
						e->binop.right->kind == T_Exp::T_CONST)
					{
						FRelocation* reloc = new FRelocation;
						reloc->VirtualAddress = ftell(fileout);
						reloc->tempLabel = e->binop.left->name.label;
					//	reloc->Type = 2;
						pFunctionCode->AddRelocation(reloc);

						fput32(fileout, e->binop.right->const_int.int32Val);
					}
					else
					{
						ASSERT(0);
					}

					explist = explist->tail;
				}

				//MessageBeep(-1);
			}
			else if (instr->kind == AS_instr::I_LSYM)
			{
			//	fwrite(instr->DATA.buffer, instr->DATA.size, 1, fileout);
				//MessageBeep(-1);
			}
			else if (instr->kind == AS_instr::I_MOVE ||
						instr->kind == AS_instr::I_OPER)
			{
				int value = ftell(fileout) - TextOffset;

				if (cpu_id == CPU_IX86)
				{
				;//	assem((x86instr*)instr->assem, pFunctionCode, fileout);
				}
				else if (cpu_id == CPU_M68K)
				{
					ASSERT((value & 1) == 0);

					assem((m68kinstr*)instr->assem, pFunctionCode, fileout);
				}
				else if (cpu_id == CPU_PPC)
				{
					ASSERT((value & 3) == 0);

				;//	assem((ppcinstr*)instr->assem, /*pFunctionCode,*/ fileout);
				}
			}
			else
				ASSERT(0);

			/*
			if (instr->getsize())
			{
				fwrite(instr->buffer, 1, instr->getsize(), fileout);
			}
			*/

			il = il->tail;
		}

		{
			long fpos = ftell(fileout);

			for (size_t i = 0; i < pFunctionCode->m_labels.size(); ++i)
			{
				RelativeLabel& label = pFunctionCode->m_labels[i];

				printf("0x%x\n", label.m_label);

				long address = label.m_address;// - pSectionHeader->PointerToRawData;

				fseek(fileout, label.m_address, SEEK_SET);

				ASSERT(label.m_label->m_spOffset != -1);

				long value = (long)label.m_label->m_spOffset;// - (address+4);

				fput32(fileout, value);
			}

			fseek(fileout, fpos, SEEK_SET);
		}

#if 0
		for (i = 0; i < globals.size(); i++)
		{
			cpp::CDeclarator* pDeclarator = globals[i];

			if (TRUE)//pDeclarator->type == T_Declarator::function)
			{
				if (pDeclarator->/*m_pDeclarator->*/m_pType->GetType()->m_type == type_function)
				{
				//	if (pDeclarator->m_pDeclarator->m_pType->GetType()->m_pFunction->m_funcspec != cpp::KW_INLINE)
					{
						if (pDeclarator->m_pBody && pDeclarator->m_name == "main")
						{
							pDeclarator->/*m_pDeclarator->*/m_spOffset = ftell(fileout) - TextOffset;
							if (pDeclarator->m_tempLabel->m_symbolIndex > 0)
								m_symbols[pDeclarator->m_tempLabel->m_symbolIndex-1]->Value = pDeclarator->/*m_pDeclarator->*/m_spOffset;

							pFunctionCode->m_ofile = this;
						//	pFunctionCode->m_pBody = pDeclarator->m_pDeclarator->m_pBody;

						//	pDeclarator->m_pDeclarator->m_spOffset = ftell(fileout) - TextOffset;
						//	m_symbols[pDeclarator->m_pDeclarator->m_symbolIndex-1]->Value = pDeclarator->m_pDeclarator->m_spOffset;

							pCodeGen->Function_Prologue(pFunctionCode);

							{
								// Instruction selection
								{
									T_stmList *p = pDeclarator->m_pBody->m_statementList;
									//for (int i = 0; i < pDeclarator->m_statementList.size(); i++)
								//	CTStatement* p = pDeclarator->m_statementList->head;
									while (p)
									{
									//	ASSERT(p->kind == CTStatement::TS_SEQ);
										pCodeGen->WriteStatement(pFunctionCode, p->head);

										//pCodeGen->WriteStatement(pFunctionCode, pDeclarator->m_statementList[i]);

										p = p->tail;//seq.right;
									}
								}

								// Control flow graph
								//use[] and def[] on each node


							pCodeGen->Function_Epilogue(pFunctionCode);

						//	pCodeGen->Function(fileout, pFunctionCode);
						}
					}
					/*
					else
					{
						MessageBeep(-1);
					}
					*/
				}
			}
			else
			{
				pDeclarator->/*m_pDeclarator->*/m_spOffset = ftell(fileout) - TextOffset;
				if (pDeclarator->m_tempLabel->m_symbolIndex > 0)
					m_symbols[pDeclarator->m_tempLabel->m_symbolIndex-1]->Value = pDeclarator->/*m_pDeclarator->*/m_spOffset;

				ASSERT(0);
#if 0
				fwrite(pDeclarator->m_pDeclarator->m_stringtok->string, 1, pDeclarator->m_pDeclarator->m_stringtok->stringlen+1, fileout);
#endif
			}
		}
#endif

#if 0
	// String constants
		for (i = 0; i < pGlobal->m_strings.size(); i++)
		{
			cpp::CExpression* pExpr = pGlobal->m_strings[i];
			pExpr->m_spOffset = ftell(fileout);
			fwrite(pExpr->tok.string, 1, pExpr->tok.stringlen+1, fileout);
		}
#endif
		long CurOffset = ftell(fileout);
	//	ASSERT((CurOffset&1)==0);	// Must be word-aligned
		execHeader.a_text = CurOffset - TextOffset;
	}

	// data
	long DataOffset = ftell(fileout);
	{
		// TODO
		for (uint i = 0; i < execHeader.a_data; ++i)
		{
			fput8(fileout, 0);
		}
	}

	// text relocations
	{
		int count = 0;

		for (size_t i = 0; i < pFunctionCode->m_ImageRelocation.GetSize(); ++i)
		{
			// relocation_info

			FRelocation* pReloc = pFunctionCode->m_ImageRelocation[i];

			FSymbol* pSymbol;
			if (pReloc->tempLabel->m_symbolIndex != 0)
				pSymbol = m_symbols[pReloc->tempLabel->m_symbolIndex-1];
			else
				pSymbol = NULL;

			long r_address = pReloc->VirtualAddress - TextOffset;

			// ??
			if (pSymbol == NULL)//pReloc->Type == IMAGE_REL_I386_REL32)
			{
				long pos = ftell(fileout);

				fseek(fileout, pReloc->VirtualAddress, SEEK_SET);

				if (pReloc->Type == 1)
				{
					long value = fget32(fileout);
					fseek(fileout, pReloc->VirtualAddress, SEEK_SET);

					value += pReloc->tempLabel->m_spOffset - (r_address);//pReloc->pRelocConst->pDeclarator->m_spOffset;// - TextOffset;
					if (cpu_id == CPU_M68K)
						fput16(fileout, value);
					else
						fwrite(&value, 2, 1, fileout);
				}
				else if (pReloc->Type == 0)
				{
				//	ASSERT(0);
					long value = fget32(fileout);
					fseek(fileout, pReloc->VirtualAddress, SEEK_SET);

					value += pReloc->tempLabel->m_spOffset;//pReloc->pRelocConst->pDeclarator->m_spOffset;// - TextOffset;

					if (cpu_id == CPU_M68K)
						fput32(fileout, value);
					else
						fwrite(&value, 4, 1, fileout);
				}

				fseek(fileout, pos, SEEK_SET);
			}
			else
			{

				if (/*pReloc->*/pSymbol)
				{
				//	fput32(fileout, pReloc->pExpr->m_spOffset - TextOffset);

					if (/*pReloc->*/pReloc->tempLabel->m_defined)	// local
					{
						long pos = ftell(fileout);

					//	pReloc->pSymbol->Value = pReloc->pSymbol->m_pDeclarator->m_spOffset;	// Here ??

						fseek(fileout, pReloc->VirtualAddress, SEEK_SET);

					//	printf("%s - %d\n", pReloc->pSymbol->m_pDeclarator->m_name.c_str(), pReloc->VirtualAddress - sizeof(exec));

						if (pSymbol->SectionNumber == 0)
						{
							if (false)	// local relative
							{
								long value = (pReloc->tempLabel->m_spOffset+TextOffset) - ftell(fileout) - 4;
								fwrite(&value, 4, 1, fileout);
								fseek(fileout, pos, SEEK_SET);
								continue;
							}
							else
							{
								long value = pReloc->tempLabel->m_spOffset;
								if (cpu_id == CPU_M68K)
									fput32(fileout, value);
								else
									fwrite(&value, 4, 1, fileout);
							}
						}
						else if (pSymbol->SectionNumber == 1)
						{
							long value = execHeader.a_text + pReloc->tempLabel->m_spOffset;
							if (cpu_id == CPU_M68K)
								fput32(fileout, value);
							else
								fwrite(&value, 4, 1, fileout);
						}
						else
						{
							fput32(fileout, 0);
						//	ASSERT(0);
						}

						fseek(fileout, pos, SEEK_SET);
					}
					/*
					else
					{
						ASSERT(0);
					}
					*/
				}
			}

			if (pReloc->Type == 0)// || pReloc->Type == 2)
			{
				uint32 bf = 0;

				if (/*pReloc->*/pSymbol == NULL)
				{
					// symbolnum

					// local, so no symbol
					//fput32(fileout, N_TEXT<<8);
					bf |= (2<<5);
					bf |= N_TEXT<<8;
				}
				//	else if (pReloc->pSymbol->m_pDeclarator->m_referenced == 2)	// local
				else if (/*pReloc->*/pReloc->tempLabel->m_defined)	// local
				{
					bf |= (2<<5);

					if (/*pReloc->*/pSymbol->SectionNumber == 0)
					{
						bf |= N_TEXT<<8;
					}
					else if (/*pReloc->*/pSymbol->SectionNumber == 1)
					{
						bf |= N_DATA<<8;
					}
					else
					{
						ASSERT(0);
					}
				}
				else	// external
				{
				//	fput32(fileout, (pReloc->SymbolTableIndex<<8) | (1<<(4))/*r_extern*/);
					bf |= (2<<5);
					bf |= ((pReloc->tempLabel->m_symbolIndex-1)<<8);
					bf |= (1<<(4))/*r_extern*/;
				}

				fput32(fileout, r_address);
				fput32(fileout, bf);

				count++;
			}
		}

		execHeader.a_trsize = count * sizeof(relocation_info);
	}

	// data relocations
	{
	}

// Write Symbols
#if 0
	for (int i = 0; i < functions.size(), i++)
	{
		nlist nlist;
		nlist.n_type = N_FUN;
		nlist.n_value = startAddress;

	//
		for (int i = 0; i < pScope->m_decls.size(); i++)
		{
			CDeclarator* pDeclarator;

			nlist nlist;
			nlist.n_type = N_FUN;
			nlist.n_value = startAddress;
			nlist.n_desc = pDeclarator->m_offset;
			StringA stab_string(pDeclarator->m_pType);
		}
	}
#endif
	// and stabs
	int strx = 4+1;	// Make room for first empty string
	{
		long SymbolOffset = ftell(fileout);

		for (size_t i = 0; i < m_symbols.GetSize(); ++i)
		{
			FSymbol* pSymbol = m_symbols[i];

			nlist n;

			n.n_value = pSymbol->Value;
			n.n_other = 0;
			n.n_desc = pSymbol->Desc;

			if (pSymbol->N.Name)
			{
				int len = strlen(pSymbol->N.Name);
				ASSERT(len > 0);
				n.n_un.n_strx = strx;//pSymbol->N.Name.Long;
				strx += len+1;
			}
			else
				n.n_un.n_strx = 0;

			if ((pSymbol->Type & N_STAB) == 0)
			{
				if (pSymbol->SectionNumber == -1)
				{
					n.n_type = N_UNDF | N_EXT;
				}
				else if (pSymbol->SectionNumber == 0)
				{
					n.n_type = N_TEXT | N_EXT;
				}
				else if (pSymbol->SectionNumber == 1)
				{
					n.n_type = N_DATA;
				}
				else
					ASSERT(0);
			}
			else
			{
			//	n.n_un.n_strx = 0;
				n.n_type = pSymbol->Type;
			}

			//n.n_type = pSymbol->

			fput32(fileout, n.n_un.n_strx);
			fput8(fileout, n.n_type);
			fput8(fileout, n.n_other);
			fput16(fileout, n.n_desc);

			if ((n.n_type & N_STAB) == 0)
			{
				if ((n.n_type & N_TYPE) == N_UNDF)
				{
					fput32(fileout, 0);
				}
				else if ((n.n_type & N_TYPE) == N_TEXT)
				{
					fput32(fileout, n.n_value);
				}
				else if ((n.n_type & N_TYPE) == N_DATA)
				{
					fput32(fileout, n.n_value + execHeader.a_text);
				}
				else
					ASSERT(0);
			}
			else
			{
				fput32(fileout, n.n_value);
			}
		}

		execHeader.a_syms = ftell(fileout) - SymbolOffset;
	}

// Write String data
	{
		int len = strx;//-4;

		/*
		{
			for (int i = 0; i < m_symbols.GetSize(); i++)
			{
				if (m_symbols[i]->N.Name)
				{
					len += strlen(m_symbols[i]->N.Name)+1;
				}
			}
		*/

			fput32(fileout, len);
//		}

		{
			fputc(0, fileout);	// First empty (null-terminated) string

			for (size_t i = 0; i < m_symbols.GetSize(); ++i)
			{
				if (m_symbols[i]->N.Name)
				{
					fwrite(m_symbols[i]->N.Name, strlen(m_symbols[i]->N.Name)+1, 1, fileout);
				}
			}
		}
		//	fwrite(m_StringTable, 1, m_SizeOfStringTable, fp);
	}

// Update header
	fseek(fileout, 0, SEEK_SET);
	fput32(fileout, execHeader.a_midmag);
	fput32(fileout, execHeader.a_text);
	fput32(fileout, execHeader.a_data);
	fput32(fileout, execHeader.a_bss);
	fput32(fileout, execHeader.a_syms);
	fput32(fileout, execHeader.a_entry);
	fput32(fileout, execHeader.a_trsize);
	fput32(fileout, execHeader.a_drsize);

	delete pFunctionCode;

	return 0;
}

//}	// cpp

}
