#include "stdafx.h"

#include "EcmaScript.h"
#include "CodeComp/ir.h"
//#include "ActionScript2/ActionScript2.h"
#include "ecmascript_ast.h"

//#include "GenActionScript2.h"

#pragma comment(lib, "LFC")
#pragma comment(lib, "CodeComp")

namespace System
{

namespace ecmajsvm
{

void munch(T_StmList* stms, vector<ecma::as_instr*>& instrs, vector<Variant>& constantPool, map<String, uint16>& pool);
}

namespace ecma
{
//#include "lex.ecmascript.h"
#include "ecmascript_parse.tab.hh"

A_ElemList* g_program;

int ecmascriptparse();

struct yy_buffer_state;
yy_buffer_state* ecmascript_scan_string( const char *yy_str );

T_StmList* translate(A_ElemList* elems, vector<Variant>& constantPool, map<String, uint16>& pool);
void munch(T_StmList* stms, vector<as_instr*> & instrs, vector<Variant>& constantPool, map<String, uint16>& pool);

void ScriptCode::AddText(StringIn code)
{
//StringA* adata = code->ToStringA();

//	CString command(code);

	ecma::yy_buffer_state* state = ecma::ecmascript_scan_string(code.c_str());
//	g_stm = NULL;
	int error = ecma::ecmascriptparse();
	if (error != 0)
	{
		raise(Exception("parse error"));
	}

	ecma::A_ElemList* sourceElems = ecma::g_program;//ecma::ecmascript_lval.elem_list;

	ASSERT(sourceElems);

	m_as_context = new ConstantPool;

	map<String, uint16> pool;

	T_StmList* slist = ecma::translate(sourceElems, m_as_context->m_constantPool, pool);
//	ecma::munch(sourceElems);

//	CTStatement* pStmt;
//	T_stmList* slist = /*pDeclarator->m_statementList =*/ linearize(pStmt);

	vector<ecma::as_instr*> instrs;

	ecmajsvm::munch(slist, instrs, m_as_context->m_constantPool, pool);

	m_code.clear();

	//vector<uint8> data;
	for (int i = 0; i < instrs.size(); ++i)
	{
		for (int j = 0; j < instrs[i]->data.size(); ++j)
			m_code.push_back(instrs[i]->data[j]);
	}
}

}
}
