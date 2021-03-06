#include "stdafx.h"
#include "EcmaScript.h"

#include "CodeComp/ir.h"

#include "ecmascript_ast.h"

//#include "ActionScript2/ActionScript.h"	// TODO, manage to remove

namespace System
{
namespace ecma
{

//using namespace ActionScripting;	// TODO, manage to remove

struct yy_buffer_state;
yy_buffer_state* ecmascript_scan_string(const char* str);

int ecmascriptparse();

extern A_ElemList* g_program;

T_StmList* translate(A_ElemList* elems, vector<Variant>& constantPool, map<String, uint16>& pool);

/*
class PoolString
{
public:
	PoolString(StringA* str)
	{
		m_str = str;
		m_strw = NULL;
	}
	StringA* m_str;
	StringW* m_strw;
};
*/

//ActionScript::ActionContext* constantPool;

//using namespace cpp;

#if 0
class T_Exp;
class Temp_label;

class T_Stm
{
public:
	T_Stm()
	{
//		kind = 0;
		seq.left = NULL;
		seq.right = NULL;
	}

	enum
	{
		T_LBRAC = 1,
		T_RBRAC,
		T_LSYM,
		T_LABEL,

		T_EXPR = 64,
		T_SEQ,
		T_JUMP,
		T_CJUMP,
		T_MOVE,
		T_ZERO,
	}
	kind;

	int m_line_pos;

	union
	{
		CDeclarator* declarator;

		struct
		{
			T_Stm* left;
			T_Stm* right;
		}
		seq;

		struct
		{
			T_Exp* left;
			T_Exp* right;
		}
		move;

		struct
		{
			int relop;
			T_Exp* left;
			T_Exp* right;
			Temp_label* t;
			Temp_label* f;
		}
		cjump;

		struct
		{
			T_Exp* exp;
		}
		jump;

		struct
		{
			int m_spOffset;
			int m_length;
		}
		zero;

		T_Exp* expr;

		Temp_label* label;
	};
};

class T_Exp
{
public:
	T_Exp()
	{
		binop.left = NULL;
		binop.right = NULL;
//		call.m_pBaseDeclarator = NULL;	// libcall

#if 0
		m_pDeclaratorRef = NULL;
#endif
	}

#define T_IMMEDIATE	64

	enum
	{
		T_MEM = 1,
		T_BINOP = 2,
		T_UNOP = 3,
		T_TEMP = 4,
		T_ESEQ = 5,
		T_CALL = 6,
		T_CONST = 7 | T_IMMEDIATE,
		T_NAME = 8 | T_IMMEDIATE,

		//
		T_MEMBER
	}
	kind;

#if 0
	CDeclaratorReference* m_pDeclaratorRef;	// Used if tok ident
#endif

//	long m_spOffset;

	union
	{
		struct
		{
			long int32;
#if 0
			//FRelocation* reloc;
			RelocConst* reloc;//CDeclarator* pDeclarator;
#endif
		}
		const_int;

		struct
		{
			int op;
			T_Exp* expr;
		}
		unop;

		struct
		{
			int op;
			T_Exp* left;
			T_Exp* right;
		}
		binop;

		struct
		{
			T_Stm* s;
			T_Exp* e;
		}
		eseq;

#if 0
		struct
		{
			T_Exp* f;
//			CDeclaratorReference* m_pBaseDeclarator;	// libcall
			ExpList* args;
		}
		call;
#endif

		struct
		{
			Temp_label* label;
		}
		name;

		struct
		{
			T_Exp* mem;
			int size;
		}
		mem;

#if 0
		Temp_temp* temp;
#endif
	};
};

struct T_stmList
{
	T_stmList()
	{
	}

	T_stmList(T_Stm* _head, T_stmList* _tail)
	{
		head = _head;
		tail = _tail;
	}

	T_Stm* head;
	T_stmList* tail;
};

#endif


T_NullExp* Null = new T_NullExp;

Tr_Exp* TransExpression(A_Exp* exp, vector<Variant>& constantPool, map<String, uint16>& pool)
{
	/*
	if (exp->kind == A_Exp::A_THIS)
	{
		Tr_Exp* pTExpr = Tr_Ex(T_Const(exp->int32));
	//	pTExpr->m_pType = new PrimitiveType(type_int);
		return pTExpr;
	}
	else
	*/
	if (exp->kind == A_Exp::A_ID)
	{
		StringA* str = new ImmutableString<char>(string_copy(exp->id));//->ToStringW();

		return Tr_Ex(T_Mem(new T_VariableExp(str), -1));
	}
	else if (exp->kind == A_Exp::A_INT32)
	{
		Tr_Exp* pTExpr = Tr_Ex(T_ConstExp(exp->int32));
	//	pTExpr->m_pType = new PrimitiveType(type_int);
		return pTExpr;
	}
	else if (exp->kind == A_Exp::A_FLOAT64)
	{
		Tr_Exp* pTExpr = Tr_Ex(T_ConstDoubleExp(static_cast<Float64Exp*>(exp)->m_number));
	//	pTExpr->m_pType = new PrimitiveType(type_int);
		return pTExpr;
	}
	else if (exp->kind == A_Exp::A_BOOL)
	{
		Tr_Exp* pTExpr = Tr_Ex(T_ConstBoolExp(exp->boolValue));
	//	pTExpr->m_pType = new PrimitiveType(type_bool);
		return pTExpr;
	}
	else if (exp->kind == A_Exp::A_NULL)
	{
		Tr_Exp* pTExpr = Tr_Ex(Null);
	//	pTExpr->m_pType = new PrimitiveType(type_int);
		return pTExpr;
	}
	else if (exp->kind == A_Exp::A_STRING)
	{
		Tr_Exp* pTExpr = Tr_Ex(new T_StringExp(static_cast<A_StringExp*>(exp)->m_string));
	//	pTExpr->m_pType = new PrimitiveType(type_int);
		return pTExpr;
	}
	else if (exp->kind == A_Exp::A_STRINGW)
	{
		Tr_Exp* pTExpr = Tr_Ex(new T_StringWExp(static_cast<A_StringWExp*>(exp)->m_string));
	//	pTExpr->m_pType = new PrimitiveType(type_int);
		return pTExpr;
	}
	else if (exp->kind == A_Exp::A_MEMBER)
	{
		A_MemberExp* me = static_cast<A_MemberExp*>(exp);
		Tr_Exp* tleft = TransExpression(me->m_left, constantPool, pool);
		if (tleft == NULL)
			return NULL;

		T_Exp* left = unEx(tleft);
		
//		ASSERT(left->kind == T_Exp::T_MEM);
//		left = left->mem.mem;

	//	T_Exp* setMember = new T_Exp;
	//	setMember->kind ) 
		Tr_Exp* tr;

		/*
		tr = Tr_Ex(
			T_Mem(
			T_Binop('+', left, T_Const(pRightDeclarator->m_offset)), pRightDeclarator->m_pType->GetType()->get_sizeof()));
			*/
//		ASSERT(0);
		return Tr_Ex(T_Mem(new T_MemberExp(left, me->m_right), -1));

		return tr;
	}
	else if (exp->kind == A_Exp::A_NEW)
	{
		A_NewExp* newexp = static_cast<A_NewExp*>(exp);

		Tr_Exp* tleft = TransExpression(newexp->m_left, constantPool, pool);
		if (tleft == NULL)
			return NULL;

		ArgList* targs = NULL;

		A_ExpList* args = newexp->m_args;
		while (args)
		{
			Tr_Exp* tr = TransExpression(args->head, constantPool, pool);

			targs = new ArgList(unEx(tr), targs);

			args = args->tail;
		}

		T_Exp* left = unEx(tleft);

		return Tr_Ex(new T_NewExp(left, targs));
	}
	else if (exp->kind == A_Exp::A_CALL)
	{
		A_CallExp* callexp = static_cast<A_CallExp*>(exp);

		Tr_Exp* tleft = TransExpression(callexp->m_left, constantPool, pool);
		if (tleft == NULL)
			return NULL;

		ArgList* targs = NULL;

		A_ExpList* args = callexp->m_args;
		while (args)
		{
			Tr_Exp* tr = TransExpression(args->head, constantPool, pool);

			targs = new ArgList(unEx(tr), targs);

			args = args->tail;
		}

		T_Exp* left = unEx(tleft);

		/*
		if (exp->call.left->kind == A_MEMBER)
		{

		}
		else
		*/
		{
			return Tr_Ex(T_Call(left, targs));
		}
	}
	else if (exp->kind == A_Exp::A_FUNCTION)
	{
		FunctionExp* funexp = static_cast<FunctionExp*>(exp);

		T_StmList* stmlist = translate(funexp->m_fun->m_body, constantPool, pool);

		return Tr_Ex(new T_FunctionExp(funexp->m_fun->m_formals, stmlist));

#if 0
			T_Stm* stm = new T_Stm;
			stm->kind = T_Stm::T_FUN;
			stm->fun.m_name = tail->head->fun->m_id;
			stm->fun.m_formals = tail->head->fun->m_formals;
			stm->fun.m_body = translate(tail->head->fun->m_body, constantPool, pool);

		Tr_Exp* tleft = TransExpression(callexp->m_left, constantPool);
		if (tleft == NULL)
			return NULL;

		ArgList* targs = NULL;

		A_ExpList* args = callexp->m_args;
		while (args)
		{
			Tr_Exp* tr = TransExpression(args->head, constantPool);

			targs = new ArgList(unEx(tr), targs);

			args = args->tail;
		}

		T_Exp* left = unEx(tleft);

		/*
		if (exp->call.left->kind == A_MEMBER)
		{

		}
		else
		*/
		{
			return Tr_Ex(T_Call(left, targs));
		}
#endif
	}
	else if (exp->kind == A_Exp::A_UNOP)
	{
		switch (exp->unop.op)
		{
		case '-':
			{
				Tr_Exp* exp2 = TransExpression(exp->unop.exp, constantPool, pool);
				Tr_Exp* tr = Tr_Ex(T_UnopExp('-', unEx(exp2)));

				return tr;
			}
			break;

		case '!':
			{
				Tr_Exp* exp2 = TransExpression(exp->unop.exp, constantPool, pool);
				Tr_Exp* tr = Tr_Ex(T_UnopExp('!', unEx(exp2)));

				return tr;
			}
			break;

		case 'type':
			{
				Tr_Exp* left = TransExpression(exp->unop.exp, constantPool, pool);
				Tr_Exp* tr = Tr_Ex(T_UnopExp('type', unEx(left)));

				return tr;
			}
			break;

		case 'new':
			{
				raise(Exception("error : new is missing parentheses?"));
			}
			break;

		default:
			ASSERT(0);
		}
	}
	else if (exp->kind == A_Exp::A_BINOP)
	{
		switch (exp->binop.op)
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '|':
		case '&':
		case '^':
		case '>':
		case '>=':
		case '<':
		case '<=':
			{
				Tr_Exp* left = TransExpression(/*frame, */exp->binop.left/*, pGlobal, pScope*/, constantPool, pool);
				Tr_Exp* right = TransExpression(/*frame, */exp->binop.right/*, pGlobal, pScope*/, constantPool, pool);

				if (left->ex->kind == A_Exp::A_STRING || left->ex->kind == A_Exp::A_STRINGW)
				{
					if (exp->binop.op == '+')
					{
						Tr_Exp* tr = Tr_Ex(T_BinopExp('s+', unEx(left), unEx(right)));
						return tr;
					}
					else
						raise(Exception("invalid op for string"));
				}
				else

			//	if (left->m_pType && right->m_pType)
				{
					Tr_Exp* tr = Tr_Ex(T_BinopExp(exp->binop.op, unEx(left), unEx(right)));

					return tr;
				}

				return NULL;
			}
			break;

		case '=':
			{
				Tr_Exp* left = TransExpression(/*frame,*/ exp->binop.left, constantPool, pool);//, pGlobal, pScope);
				if (left == NULL)
				{
					return NULL;
				}
				else
				{
					Tr_Exp* right = TransExpression(/*frame,*/ exp->binop.right, constantPool, pool);//, pGlobal, pScope);

					if (right == NULL)
					{
						return NULL;
					}
					else
					{
					//	Tr_Exp* p = Tr_Ex(T_ESeq(T_MoveStm(left->ex, unEx(right)), left->ex));
						Tr_Exp* p = Tr_Nx(T_MoveStm(left->ex, unEx(right)));
					//	p->m_pType = left->m_pType;
						return p;
					}
				}
			}
			break;

		default:
			ASSERT(0);
		}
	}
	else
		ASSERT(0);

	return NULL;
}

T_Stm* TransStatement(A_Stm* stm, vector<Variant>& constantPool, map<String, uint16>& pool)
{
	switch (stm->GetKind())
	{
		/*
	case A_Stm::A_FUN:
		{
		}
		break;
		*/

	case A_Stm::A_VAR:
		{
			T_Stm* tstm = nullptr;

			A_VarStm* varstm = static_cast<A_VarStm*>(stm);
			for (auto vardecls = varstm->m_vardecls; vardecls != nullptr; vardecls = vardecls->tail)
			{
				String str = vardecls->head->m_identifier;
				map<String, uint16>::iterator it = pool.find(str);
				if (it == pool.end())
				{
					it = pool.insert(map<String, uint16>::value_type(str, (uint16)constantPool.size())).first;
					constantPool.push_back(Variant(str));
				}

				uint16 name = it->second;

				T_Stm* tstm2 = new T_Stm;
				tstm2->kind = T_Stm::T_VAR;
				tstm2->var.name = name;

				if (tstm)
					tstm = T_Seq(tstm, tstm2);
				else
					tstm = tstm2;

				if (vardecls->head->m_initializer)
				{
					Tr_Exp* left = Tr_Ex(T_Mem(new T_VariableExp(vardecls->head->m_identifier), -1));
					if (left == nullptr)
					{
						return nullptr;
					}

					Tr_Exp* right = TransExpression(/*frame,*/ vardecls->head->m_initializer, constantPool, pool);//, pGlobal, pScope);
					if (right == nullptr)
					{
						return nullptr;
					}

					tstm = T_Seq(tstm, T_MoveStm(left->ex, unEx(right)));
				}
			}

			return tstm;
		}
		break;

	case A_Stm::A_EXP:
		{
			A_ExpStm* expstm = static_cast<A_ExpStm*>(stm);

			Tr_Exp* tr = TransExpression(/*frame,*/ expstm->m_exp/*, pGlobal, pScope*/, constantPool, pool);
			
			if (tr == nullptr)
				return nullptr;
			
			T_Stm* t = unNx(tr);
			//	t->m_line_pos = stm->exp->line_pos;
			
			return t;
		}
		break;
		
	case A_Stm::A_IF:
		{
			A_IfStm* ifstm = static_cast<A_IfStm*>(stm);

			Tr_Exp* cond = TransExpression(/*frame,*/ ifstm->m_cond/*, pGlobal, pScope*/, constantPool, pool);

			Cx cx = unCx(cond);

			Temp_label* top = new Temp_label;
			Temp_label* contlabel = new Temp_label;
			Temp_label* t = new Temp_label;
			Temp_label* f = new Temp_label;

			PatchTrue(cx.trues, t);
			PatchFalse(cx.falses, f);

			T_Stm* for_stm = TransStatement(ifstm->m_stm, constantPool, pool);

			ASSERT(0);
		//	T_Stm* ts = T_Seq(unNx(for_init),
		}
		break;

	case A_Stm::A_FOR:
		{
			A_ForStm* forstm = (A_ForStm*)stm;

			Tr_Exp* for_init = TransExpression(/*frame,*/ forstm->m_init/*, pGlobal, pScope*/, constantPool, pool);
			Tr_Exp* for_cond = TransExpression(/*frame,*/ forstm->m_cond/*, pGlobal, pScope*/, constantPool, pool);
			Tr_Exp* for_exp = TransExpression(/*frame,*/ forstm->m_exp/*, pGlobal, pScope*/, constantPool, pool);

			Cx cx = unCx(for_cond);

			Temp_label* top = new Temp_label;
			Temp_label* contlabel = new Temp_label;
			Temp_label* t = new Temp_label;
			Temp_label* f = new Temp_label;

			PatchTrue(cx.trues, t);
			PatchFalse(cx.falses, f);

			T_Stm* for_stm = TransStatement(forstm->m_stm, constantPool, pool);

			T_Stm* ts = T_Seq(unNx(for_init),
								T_Seq(T_Label(top),
									T_Seq(cx.stm,
										T_Seq(T_Label(t),
											T_Seq(for_stm,
												T_Seq(T_Label(contlabel),
													T_Seq(unNx(for_exp),
														T_Seq(T_JumpStm(T_Name(top), new Temp_labelList(top, NULL)),
															T_Label(f, forstm->m_last_line)))))))));

			/*
			return T_Seq(unEx(tr_init),
							T_Seq(top_label,
								T_Seq(for_stm,
									T_ESeq(for_exp,
										T_Seq(
											
											T_Jump(begin_label),
												f)))));
												*/

			return ts;
		}
		break;

	case A_Stm::A_RETURN:
		{
			A_ReturnStm* retstm = static_cast<A_ReturnStm*>(stm);

			Tr_Exp* retexp = TransExpression(/*frame,*/ retstm->m_exp/*, pGlobal, pScope*/, constantPool, pool);

			return nullptr;
		}
		break;

	default:
		ASSERT(0);
	}

	return nullptr;
}

T_StmList* translate(A_ElemList* elems, vector<Variant>& constantPool, map<String, uint16> & pool)
{
	T_StmList* slist = NULL;

	for (auto tail = elems; tail != nullptr; tail = tail->tail)
	{
		if (tail->head->kind == A_Elem::A_FUNCTION)
		{
			T_Stm* stm = new T_Stm;
			stm->kind = T_Stm::T_FUN;
			stm->fun.m_name = tail->head->fun->m_id;
			stm->fun.m_formals = tail->head->fun->m_formals;
			stm->fun.m_body = translate(tail->head->fun->m_body, constantPool, pool);

			slist = new T_StmList(stm, slist);
		}
		else
		{
			slist = new T_StmList(TransStatement(tail->head->stm, constantPool, pool), slist);
		}
	}

	return slist;
}

}	// ecma
}	// System
