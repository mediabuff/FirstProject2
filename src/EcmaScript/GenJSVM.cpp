#include "stdafx.h"
#include "EcmaScript.h"
#include "CodeComp/ir.h"
//#include "JSVM/JSVM.h"
//#include "GenActionScript2.h"

namespace System
{
namespace ecmajsvm
{

using namespace ecma;
//using namespace ActionScripting;

void munch(T_StmList* stms, vector<ecma::as_instr*>& instrs, vector<Variant>& constantPool, map<String, uint16>& pool);


// munch

void push_string(StringIn str, vector<as_instr*>& instrs, vector<Variant>& constantPool, map<String, uint16>& pool)
{
	auto it = pool.find(str);
	if (it == pool.end())
	{
		it = pool.insert(map<String, uint16>::value_type(str, (uint16)constantPool.size())).first;
		constantPool.push_back(Variant(str));
	}
	uint16 index = it->second;

	as_instr* p = new as_instr;
	p->data.push_back(0x96);	// push
	//if (ActionCode >= 0x80)
	{
		uint16 length = 1 + 2;
		p->data.push_back(length & 0xFF);
		p->data.push_back(length >> 8);
	}

	p->data.push_back(9);	// string
	p->data.push_back(index & 0xFF);
	p->data.push_back(index >> 8);
	instrs.push_back(p);
}

void push_bool(bool value, vector<as_instr*>& instrs)
{
	as_instr* p = new as_instr;
	p->data.push_back(0x96);	// push
	//if (ActionCode >= 0x80)
	{
		uint16 length = 1 + 1;
		p->data.push_back(length & 0xFF);
		p->data.push_back(length >> 8);
	}

	p->data.push_back(5);	// boolean
	p->data.push_back(value);
	instrs.push_back(p);
}

void push_int32(int32 value, vector<as_instr*>& instrs)
{
	as_instr* p = new as_instr;
	p->data.push_back(0x96);	// push
	//if (ActionCode >= 0x80)
	{
		uint16 length = 1 + 4;
		p->data.push_back(length & 0xFF);
		p->data.push_back(length >> 8);
	}

	p->data.push_back(7);	// int32
	p->data.push_back(value & 0xFF);
	p->data.push_back((value >> 8) & 0xFF);
	p->data.push_back((value >> 16) & 0xFF);
	p->data.push_back((value >> 24) & 0xFF);
	instrs.push_back(p);
}

void munchStm(T_Stm* s, vector<as_instr*>& instrs, vector<Variant>& constantPool, map<String, uint16> & pool);

void munchExp(T_Exp* e, vector<as_instr*>& instrs, vector<Variant>& constantPool, map<String, uint16> & pool)
{
	switch (e->kind)
	{
	case T_Exp::T_ESEQ:
		{
			munchStm(e->eseq.s, instrs, constantPool, pool);
			munchExp(e->eseq.e, instrs, constantPool, pool);
		}
		break;

	case T_Exp::T_NULL:
		{
			as_instr* p = new as_instr;
			p->data.push_back(0x96);	// push
			//if (ActionCode >= 0x80)
			{
				uint16 length = 1;
				p->data.push_back(length & 0xFF);
				p->data.push_back(length >> 8);
			}

			p->data.push_back(2);	// null

			instrs.push_back(p);
		}
		break;

	case T_Exp::T_CONST_INT32:
		{
			push_int32(e->const_int.int32Val, instrs);
		}
		break;

	case T_Exp::T_CONST_BOOL:
		{
			push_bool(e->const_bool.value, instrs);
		}
		break;

	case T_Exp::T_CONST_DOUBLE:
		{
		//	TRACE("Push %f\n", e->const_double.float64);

			as_instr* p = new as_instr;
			p->data.push_back(0x96);	// push
			//if (ActionCode >= 0x80)
			{
				uint16 length = 1 + 8;
				p->data.push_back(length & 0xFF);
				p->data.push_back(length >> 8);
			}

			p->data.push_back(6);	// float64
			p->data.push_back(e->const_double.intbits & 0xFF);
			p->data.push_back((e->const_double.intbits>>8) & 0xFF);
			p->data.push_back((e->const_double.intbits>>16) & 0xFF);
			p->data.push_back((e->const_double.intbits>>24) & 0xFF);
			p->data.push_back((e->const_double.intbits>>32) & 0xFF);
			p->data.push_back((e->const_double.intbits>>40) & 0xFF);
			p->data.push_back((e->const_double.intbits>>48) & 0xFF);
			p->data.push_back((e->const_double.intbits>>56) & 0xFF);

			instrs.push_back(p);
		}
		break;

	case T_Exp::T_STRING:
		{
			T_StringExp* te = static_cast<T_StringExp*>(e);

			push_string(te->m_string, instrs, constantPool, pool);
		}
		break;

	case T_Exp::T_UNOP:
		{
			munchExp(e->unop.exp, instrs, constantPool, pool);

			if (e->unop.op == '-')
			{
				as_instr* p = new as_instr;
				p->data.push_back(0x09);	// Negate
				instrs.push_back(p);
			}
			else if (e->unop.op == '!')
			{
				as_instr* p = new as_instr;
				p->data.push_back(0x12);	// Not
				instrs.push_back(p);
			}
			else if (e->unop.op == 'type')
			{
				as_instr* p = new as_instr;
				p->data.push_back(0x44);	// Typeof
				instrs.push_back(p);
			}
			else
				ASSERT(0);
		}
		break;

	case T_Exp::T_BINOP:
		{
			munchExp(e->binop.left, instrs, constantPool, pool);
			munchExp(e->binop.right, instrs, constantPool, pool);

			switch (e->binop.op)
			{
			case '<':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x48);	// Less2
					instrs.push_back(p);
				}
				break;

			case '>':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x67);	// Greater
					instrs.push_back(p);
				}
				break;

			case '+':
				{
					as_instr* p = new as_instr;
				//	p->data.push_back(0x0A);	// Add
					p->data.push_back(0x47);	// Add2
					instrs.push_back(p);
				}
				break;

			case '-':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x0B);	// Subtract
					instrs.push_back(p);
				}
				break;

			case '*':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x0C);
					instrs.push_back(p);
				}
				break;

			case '/':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x0D);
					instrs.push_back(p);
				}
				break;

			case '%':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x3F);
					instrs.push_back(p);
				}
				break;

			case '&&':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x10);
					instrs.push_back(p);
				}
				break;

			case '||':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x11);
					instrs.push_back(p);
				}
				break;

			case '&':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x60);
					instrs.push_back(p);
				}
				break;

			case '|':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x61);
					instrs.push_back(p);
				}
				break;

			case '^':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x62);
					instrs.push_back(p);
				}
				break;

			case 's+':
				{
					as_instr* p = new as_instr;
					p->data.push_back(0x21);	// StringAdd
					instrs.push_back(p);
				}
				break;

			default:
				ASSERT(0);
			}
		}
		break;

	case T_Exp::T_MEM:
		{
			if (e->mem.mem->kind == T_Exp::T_MEMBER)
			{
				T_MemberExp* member_exp = static_cast<T_MemberExp*>(e->mem.mem);

				munchExp(member_exp->m_left, instrs, constantPool, pool);
				push_string(member_exp->m_right, instrs, constantPool, pool);

				{
					as_instr* p = new as_instr;
					p->data.push_back(0x4E);	// GetMember
					instrs.push_back(p);
				}
			}
			else if (e->mem.mem->kind == T_Exp::T_VARIABLE)
			{
				T_VariableExp* varexp = static_cast<T_VariableExp*>(e->mem.mem);
				as_instr* p;
				
				push_string(varexp->m_identifier, instrs, constantPool, pool);

				{
					p = new as_instr;
					p->data.push_back(0x1C);	// GetVariable
					instrs.push_back(p);
				}
			}
			else
				ASSERT(0);
		}
		break;

	case T_Exp::T_CALL:
		{
			as_instr* p;

			int numArgs = 0;
			for (auto args = e->call.args; args != NULL; args = args->tail)
			{
				munchExp(args->head, instrs, constantPool, pool);
				++numArgs;
			}

			// push number of arguments
			push_int32(numArgs, instrs);

			if (e->call.f->mem.mem->kind == T_Exp::T_MEMBER)
			{
				T_MemberExp* member_exp = static_cast<T_MemberExp*>(e->call.f->mem.mem);

				// push object
				munchExp(member_exp->m_left, instrs, constantPool, pool);

				// push method name
				push_string(member_exp->m_right, instrs, constantPool, pool);

				p = new as_instr;
				p->data.push_back(0x52);	// ActionCallMethod
				instrs.push_back(p);
			}
			else
			{
				T_VariableExp* var_exp = static_cast<T_VariableExp*>(e->call.f->mem.mem);

				// push function name
				push_string(var_exp->m_identifier, instrs, constantPool, pool);

				p = new as_instr;
				p->data.push_back(0x3D);	// ActionCallFunction
				instrs.push_back(p);
			}
		}
		break;

	case T_Exp::T_NEW:
		{
			as_instr* p;

			int numArgs = 0;
			for (auto args = e->call.args; args != NULL; args = args->tail)
			{
				munchExp(args->head, instrs, constantPool, pool);
				++numArgs;
			}

			push_int32(numArgs, instrs);

			if (e->call.f->mem.mem->kind == T_Exp::T_MEMBER)
			{
				T_MemberExp* member_exp = static_cast<T_MemberExp*>(e->call.f->mem.mem);

				munchExp(member_exp->m_left, instrs, constantPool, pool);
				push_string(member_exp->m_right, instrs, constantPool, pool);

				{
					as_instr* p = new as_instr;
					p->data.push_back(0x06);	// GetTypeMember
					instrs.push_back(p);
				}
			}
			else if (e->call.f->mem.mem->kind == T_Exp::T_VARIABLE)
			{
				munchExp(e->call.f, instrs, constantPool, pool);
			}
			else
			{
				raise(Exception("syntax error : new identifier()"));
			}

			p = new as_instr;
			p->data.push_back(0x40);	// NewObject
			instrs.push_back(p);
		}
		break;

	case T_Exp::T_FUNCTION:
		{
			T_FunctionExp* tfun = static_cast<T_FunctionExp*>(e);

			as_instr* p = new as_instr;
			p->data.push_back(0x9B);	// DefineFunction
			p->data.push_back(0);	// reserve codesize
			p->data.push_back(0);	// reserve codesize

			p->data.push_back(0);	// null name
			ULONG length = 1;

			uint16 numParams = Count(tfun->m_formals);
			p->data.push_back(numParams & 0xFF);
			p->data.push_back(numParams >> 8);
			length += 2;
			for (auto formals = tfun->m_formals; formals != NULL; formals = formals->tail)
			{
				CString cstr(formals->head);
				p->data.insert(p->data.end(), (ubyte*)cstr.c_str(), (ubyte*)cstr.c_str()+cstr.len()+1);

				length += cstr.len()+1;
			}

			vector<as_instr*> body;

			munch(tfun->m_body, body, constantPool, pool);

			uint32 codelength = 0;
			for (int i = 0; i < body.size(); ++i)
			{
				codelength += body[i]->data.size();
			}
			length += codelength;

			p->data.push_back(codelength & 0xFF);
			p->data.push_back(codelength >> 8);
			length += 2;

			if (length >= 65536)
			{
				raise(Exception("JSVM: length >= 65536"));
			}

			for (int i = 0; i < body.size(); ++i)
			{
				for (int j = 0; j < body[i]->data.size(); ++j)
					p->data.push_back(body[i]->data[j]);
			}

			p->data[1] = length & 0xFF;
			p->data[2] = length>>8;

			instrs.push_back(p);
		}
		break;

	default:
		ASSERT(0);
	}
}

void munch(T_StmList* stms, vector<as_instr*> & instrs, vector<Variant>& constantPool, map<String, uint16> & pool);

void munchStm(T_Stm* s, vector<as_instr*> & instrs, vector<Variant>& constantPool, map<String, uint16>& pool)
{
	switch (s->kind)
	{
	case T_Stm::T_FUN:
		{
			as_instr* p;

			p = new as_instr;
			p->data.push_back(0x9B);	// DefineFunction
			//if (ActionCode >= 0x80)
			{
				p->data.push_back(0);
				p->data.push_back(0);
			}
			ULONG length = 0;

			CString cstr(s->fun.m_name);
			p->data.insert(p->data.end(), (ubyte*)cstr.c_str(), (ubyte*)cstr.c_str()+cstr.len()+1);

			length += cstr.len()+1;

			uint16 numParams = Count(s->fun.m_formals);
			p->data.push_back(numParams & 0xFF);
			p->data.push_back(numParams >> 8);
			length += 2;
			for (auto formals = s->fun.m_formals; formals != NULL; formals = formals->tail)
			{
				CString cstr(formals->head);
				p->data.insert(p->data.end(), (ubyte*)cstr.c_str(), (ubyte*)cstr.c_str()+cstr.len()+1);

				length += cstr.len()+1;
			}

			vector<as_instr*> body;

			munch(s->fun.m_body, body, constantPool, pool);

			ULONG codelength = 0;
			for (int i = 0; i < body.size(); i++)
			{
				codelength += body[i]->data.size();
			}
			length += codelength;

			p->data.push_back(codelength & 0xFF);
			p->data.push_back(codelength >> 8);
			length += 2;

			if (length >= 65536)
			{
				ASSERT(0);
				raise(Exception("JSVM: length >= 65536"));
			}

			{
				for (int i = 0; i < body.size(); i++)
				{
					for (int j = 0; j < body[i]->data.size(); j++)
						p->data.push_back(body[i]->data[j]);
				}
			}

			p->data[1] = length & 0xFF;
			p->data[2] = length>>8;

			instrs.push_back(p);
		}
		break;

	case T_Stm::T_VAR:
		{
			as_instr* p;

			p = new as_instr;
			p->data.push_back(0x96);	// push
			//if (ActionCode >= 0x80)
			{
				uint16 length = 1 + 2;
				p->data.push_back(length & 0xFF);
				p->data.push_back(length >> 8);
			}
			p->data.push_back(9/*16*/);	// constant
			p->data.push_back(s->var.name & 0xFF);
			p->data.push_back(s->var.name >> 8);
			instrs.push_back(p);

			p = new as_instr;
			p->data.push_back(0x41);	// DefineLocal2
			instrs.push_back(p);
		}
		break;

	case T_Stm::T_MOVE:
		{
			T_Exp* dst = s->move.left;
			T_Exp* src = s->move.right;

			if (dst->kind == T_Exp::T_MEM)
			{
				if (dst->mem.mem->kind == T_Exp::T_VARIABLE)
				{
					T_VariableExp* varexp = static_cast<T_VariableExp*>(dst->mem.mem);

					as_instr* p;
					push_string(varexp->m_identifier, instrs, constantPool, pool);

					munchExp(src, instrs, constantPool, pool);

					p = new as_instr;
					p->data.push_back(0x1D);	// SetVariable
					instrs.push_back(p);
				}
				else if (dst->mem.mem->kind == T_Exp::T_MEMBER)
				{
					T_MemberExp* member_exp = static_cast<T_MemberExp*>(dst->mem.mem);

					as_instr* p;

					// object
					munchExp(member_exp->m_left, instrs, constantPool, pool);

					// property name
					push_string(member_exp->m_right, instrs, constantPool, pool);

					// value
					munchExp(src, instrs, constantPool, pool);

					p = new as_instr;
					p->data.push_back(0x4F);	// SetMember
					instrs.push_back(p);
				}
				else
					ASSERT(0);
			}
			else
				ASSERT(0);
		}
		break;

	case T_Stm::T_EXP:
		{
			munchExp(s->exp, instrs, constantPool, pool);
		}
		break;

	case T_Stm::T_SEQ:
		{
			munchStm(s->seq.left, instrs, constantPool, pool);
			munchStm(s->seq.right, instrs, constantPool, pool);
		}
		break;

	default:
		ASSERT(0);
	}
}

void munch(T_StmList* stms, vector<as_instr*>& instrs, vector<Variant>& constantPool, map<String, uint16>& pool)
{
	for (auto tail = stms; tail != NULL; tail = tail->tail)
	{
		munchStm(tail->head, instrs, constantPool, pool);
	}
}

}	// ecmajsvm
}	// System
