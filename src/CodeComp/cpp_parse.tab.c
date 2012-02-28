
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse         cpp_parse
#define yylex           cpp_lex
#define yyerror         cpp_error
#define yylval          cpp_lval
#define yychar          cpp_char
#define yydebug         cpp_debug
#define yynerrs         cpp_nerrs
#define yylloc          cpp_lloc

/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"


//#define YYERROR_VERBOSE

#define YYMAXDEPTH	80000

struct PP_TokenList;

extern PP_TokenList* tlist;

bool bLastTok = false;

int GetString(int delimiter, bool bWide/*, CToken& token*/)
{
	int line = pPP2->m_pFile->m_line;

	IO::StringWriter strbuilder;

	int c2;
	while (!pPP2->_eof())
	{
		//CToken* ptok = pPP->GetToken4();
		c2 = pPP2->_getc();

		if (c2 == '\\')
		{
			int c3 = pPP2->_getc();
			if (c3 == '\\')
			{
				c2 = '\\';
			}
			else if (c3 == 'n')
			{
				c2 = '\n';
			}
			else if (c3 == 'r')
			{
				c2 = '\r';
			}
			else if (c3 == 't')
			{
				c2 = '\t';
			}
			else if (c3 == '?')
			{
				c2 = '\?';
			}
			else if (c3 == '\'')
			{
				c2 = '\'';
			}
			else if (c3 == '0')
			{
				c2 = '\0';
			}
			else
			{
				printf("Unrecognized string sequence\n");
			}
		}
		else if (c2 == delimiter)
		{
		//	ungetc(c2, fpin);
			break;
		}
		strbuilder << (char)c2;
	}

	if (c2 != delimiter)
	{
		char msg[256];
		sprintf_s(msg, "expected end of string, started on line(%d)", line);
		printf(msg);
	}

/*
	if (bWide)
	{
		token.type = CToken::tok_wstring;
	}
	else
	{
		token.type = CToken::tok_string;
	}
	*/

	cpp_lval.string_literal = dynamic_cast<StringA*>(strbuilder.str().m_stringObject);
	cpp_lval.string_literal->IncRef();

	return STRING_LITERAL;
}

static int cpp_lex(/*YYSTYPE *lvalp, YYLTYPE *llocp*/)
{
	if (!bLastTok)
		pPP2->_GetToken();
	else
		bLastTok = false;

	yylloc.first_line = pPP2->m_pFile->m_line;
	yylloc.first_column = pPP2->m_pFile->m_column;
	yylloc.filename = /*dynamic_cast<StringA*>*/(pPP2->m_pFile->m_filepath.m_stringObject);

#if 0
	while (pPP2->curtokpp->type == CToken::tok_space ||
		(pPP2->curtokpp->type == CToken::tok_symbol && pPP2->curtokpp->symbol == '\n'))
	{
		yylloc.first_line = pPP2->m_pFile->m_line;
		pPP2->GetToken();
	//	yylpos.first_line = pPP2->m_location.m_line;
	}
#endif

	if (pPP2->curtokpp->GetKind() == CToken::tok_ident)
	{
		kwmap::iterator it = kwm.find(String(pPP2->curtokpp->m_astr));
		if (it != kwm.end())
		{
			return (*it).second;
		}
		else
		{
			cpp_lval.id = pPP2->curtokpp->m_astr;//ident.ident;
			return IDENTIFIER;
		}

		/*
		int kw;
		if (kwm.Lookup(ckw(pPP2->curtokpp->ident.ident), kw))
			return kw;
		else
		{
			cpp_lval.id = pPP2->curtokpp->ident.ident;
			return IDENTIFIER;
		}
		*/
	}
	else if (pPP2->curtokpp->GetKind() == CToken::tok_integer)
	{
		cpp_lval.num = pPP2->curtokpp->intval;
		return INTEGER_LITERAL;
	}
	else if (pPP2->curtokpp->GetKind() == CToken::tok_double)
	{
		cpp_lval.double_num = pPP2->curtokpp->double64;
		return DOUBLE_LITERAL;
	}
	else if (pPP2->curtokpp->GetKind() == CToken::tok_string_literal)
	{
		cpp_lval.string_literal = pPP2->curtokpp->m_astr;
		return STRING_LITERAL;
	}
	else if (pPP2->curtokpp->GetKind() == CToken::tok_import)
	{
		cpp_lval.string_literal = pPP2->curtokpp->m_astr;
		return IMPORT_KW;
	}
	else if (pPP2->curtokpp->GetKind() == CToken::tok_char_literal)
	{
		cpp_lval.num = pPP2->curtokpp->intval;
		return CHAR_LITERAL;
	}
	else if (pPP2->curtokpp->GetKind() == CToken::tok_char ||
				pPP2->curtokpp->GetKind() == CToken::tok_symbol)
	{
		int c = pPP2->curtokpp->symbol;

#if 0
		if (c == '\'')
		{
			return GetString(c, FALSE/*, token*/);
		}
		else if (c == '\"')
		{
			return GetString(c, FALSE/*, token*/);
		}
#endif
		if (pPP2->curtokpp->symbol == '*')
		{
			return STAR;
		}
		else if (pPP2->curtokpp->symbol == '&')
		{
			return BITAND;
		}
		else if (pPP2->curtokpp->symbol == '|')
		{
			return BITOR;
		}
		else if (pPP2->curtokpp->symbol == '^')
		{
			return BITXOR;
		}
		else if (pPP2->curtokpp->symbol == '<<')
		{
			return LSHIFT;
		}
		else if (pPP2->curtokpp->symbol == '>>')
		{
			return RSHIFT;
		}
		else if (pPP2->curtokpp->symbol == '==')
		{
			return EQUAL;
		}
		else if (pPP2->curtokpp->symbol == '|=')
		{
			return ASSIGN_OR;
		}
		else if (pPP2->curtokpp->symbol == '&=')
		{
			return ASSIGN_AND;
		}
		else if (pPP2->curtokpp->symbol == '^=')
		{
			return ASSIGN_XOR;
		}
		else if (pPP2->curtokpp->symbol == '!=')
		{
			return NOT_EQUAL;
		}
		else if (pPP2->curtokpp->symbol == '+=')
		{
			return ASSIGN_PLUS;
		}
		else if (pPP2->curtokpp->symbol == '-=')
		{
			return ASSIGN_MINUS;
		}
		else if (pPP2->curtokpp->symbol == '*=')
		{
			return ASSIGN_MUL;
		}
		else if (pPP2->curtokpp->symbol == '/=')
		{
			return ASSIGN_DIV;
		}
		else if (pPP2->curtokpp->symbol == '%=')
		{
			return ASSIGN_MOD;
		}
		else if (pPP2->curtokpp->symbol == '>>=')
		{
			return ASSIGN_RSHIFT;
		}
		else if (pPP2->curtokpp->symbol == '<<=')
		{
			return ASSIGN_LSHIFT;
		}
		else if (pPP2->curtokpp->symbol == '...')
		{
			return THREE_DOTS;
		}
		else if (pPP2->curtokpp->symbol == '->')
		{
			return ARROW;
		}
		else if (pPP2->curtokpp->symbol == '++')
		{
			return PLUSPLUS;
		}
		else if (pPP2->curtokpp->symbol == '--')
		{
			return MINUSMINUS;
		}
		else if (pPP2->curtokpp->symbol == '::')
		{
			return TWO_COLON;
		}
		else if (pPP2->curtokpp->symbol == '<=')
		{
			return LEQ;
		}
		else if (pPP2->curtokpp->symbol == '>=')
		{
			return GEQ;
		}
		else if (pPP2->curtokpp->symbol == '&&')
		{
			return LAND;
		}
		else if (pPP2->curtokpp->symbol == '||')
		{
			return LOR;
		}
		else if (pPP2->curtokpp->symbol == '.*')
		{
			return DOT_STAR;
		}
		else if (pPP2->curtokpp->symbol == '->*')
		{
			return ARROW_STAR;
		}
		else
		{
		//	ASSERT(pPP2->curtokpp->symbol < 256);
			return pPP2->curtokpp->symbol;
		}
	}
	else
		ASSERT(0);

#if 0
	if (tlist == NULL)
		return -1;

   while (tlist->head.kind == ' ')
		tlist = tlist->tail;

	PP_Token& pptok = tlist->head;
	tlist = tlist->tail;

	cpp_lloc.first_line = pptok.line_pos;

	if (pptok.kind == PP_Token::PP_CHAR)
	{
		return pptok.c;
	}
	else if (pptok.kind == PP_Token::PP_INT32)
	{
		cpp_lval.num = pptok.int32;
		return INTEGER_LITERAL;
	}
	else if (pptok.kind == PP_Token::PP_IDENTIFIER)
	{
		if (!strcmp(pptok.id, "void"))
		{
			return VOID_KW;
		}
		else if (!strcmp(pptok.id, "int"))
		{
			return INT_KW;
		}
		else if (!strcmp(pptok.id, "long"))
		{
			return LONG_KW;
		}
		else if (!strcmp(pptok.id, "if"))
		{
			return IF_KW;
		}
		else if (!strcmp(pptok.id, "else"))
		{
			return ELSE_KW;
		}
		else
		{
			cpp_lval.id = pptok.id;
			return IDENTIFIER;
		}
	}
	else
		ASSERT(0);
#endif

	return -1;
}

void cpp_error (/*YYLTYPE *llocp,*/ char const *s);
/*#define YYERROR_VERBOSE*/

#define yyfalse	0
#define yytrue		1

union YYSTYPE;

ast::A_Stm* stmtMerge (YYSTYPE& x0, YYSTYPE& x1);
//ast::A_Declaration* Merge (YYSTYPE& x0, YYSTYPE& x1);

//ast::A_TemplateArgument* template_argument_merge (YYSTYPE& x0, YYSTYPE& x1);

ast::DeclarationList* translation_unit;

ast::Expression2List* g_primlist;

//ast::A_Type* gType;


     

/* Line 189 of yacc.c  */
#line 462 "cpp_parse.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     EXP_TOKEN = 258,
     DECLS_TOKEN = 259,
     IDENTIFIER = 260,
     identifier_lparen = 261,
     PLUSPLUS = 262,
     MINUSMINUS = 263,
     ARROW = 264,
     EQUAL = 265,
     NOT_EQUAL = 266,
     ASSIGN_PLUS = 267,
     ASSIGN_MINUS = 268,
     ASSIGN_OR = 269,
     ASSIGN_XOR = 270,
     ASSIGN_AND = 271,
     ASSIGN_MUL = 272,
     ASSIGN_DIV = 273,
     ASSIGN_MOD = 274,
     ASSIGN_LSHIFT = 275,
     ASSIGN_RSHIFT = 276,
     LEQ = 277,
     GEQ = 278,
     STAR = 279,
     BITAND = 280,
     BITOR = 281,
     BITXOR = 282,
     LAND = 283,
     LOR = 284,
     DOT_STAR = 285,
     ARROW_STAR = 286,
     NAMESPACE_KW = 287,
     USING_KW = 288,
     EXTERN_KW = 289,
     IF_KW = 290,
     ELSE_KW = 291,
     SWITCH_KW = 292,
     CASE_KW = 293,
     DEFAULT_KW = 294,
     GOTO_KW = 295,
     BREAK_KW = 296,
     CONTINUE_KW = 297,
     FOR_KW = 298,
     DO_KW = 299,
     WHILE_KW = 300,
     CONST_KW = 301,
     VOLATILE_KW = 302,
     TYPEDEF_KW = 303,
     TYPENAME_KW = 304,
     ENUM_KW = 305,
     CLASS_KW = 306,
     STRUCT_KW = 307,
     UNION_KW = 308,
     PRIVATE_KW = 309,
     PROTECTED_KW = 310,
     PUBLIC_KW = 311,
     AUTO_KW = 312,
     REGISTER_KW = 313,
     STATIC_KW = 314,
     MUTABLE_KW = 315,
     INLINE_KW = 316,
     VIRTUAL_KW = 317,
     EXPLICIT_KW = 318,
     FRIEND_KW = 319,
     FALSE_KW = 320,
     TRUE_KW = 321,
     STATIC_ASSERT_KW = 322,
     STATIC_CAST_KW = 323,
     REINTERPRET_CAST_KW = 324,
     DYNAMIC_CAST_KW = 325,
     CONST_CAST_KW = 326,
     RETURN_KW = 327,
     TRY_KW = 328,
     THROW_KW = 329,
     CATCH_KW = 330,
     CHAR_KW = 331,
     WCHAR_T_KW = 332,
     BOOL_KW = 333,
     SHORT_KW = 334,
     INT_KW = 335,
     INTERFACE_KW = 336,
     LONG_KW = 337,
     SIGNED_KW = 338,
     UNSIGNED_KW = 339,
     FLOAT_KW = 340,
     DOUBLE_KW = 341,
     VOID_KW = 342,
     TEMPLATE_KW = 343,
     EXPORT_KW = 344,
     NULLPTR_KW = 345,
     CONSTEXPR_KW = 346,
     DECLTYPE_KW = 347,
     __FLOAT32_KW = 348,
     __FLOAT64_KW = 349,
     __INT8_KW = 350,
     __INT16_KW = 351,
     __INT32_KW = 352,
     __INT64_KW = 353,
     __INT128_KW = 354,
     __CDECL_KW = 355,
     __STDCALL_KW = 356,
     __FASTCALL_KW = 357,
     __DECLSPEC_KW = 358,
     __ASM_KW = 359,
     LSHIFT = 360,
     RSHIFT = 361,
     TWO_COLON = 362,
     THREE_DOTS = 363,
     NEW_KW = 364,
     DELETE_KW = 365,
     OPERATOR_KW = 366,
     SIZEOF_KW = 367,
     TYPEID_KW = 368,
     STRING_LITERAL = 369,
     LSTRING_LITERAL = 370,
     CHAR_LITERAL = 371,
     INTEGER_LITERAL = 372,
     DOUBLE_LITERAL = 373,
     IMPORT_KW = 374,
     TEMPLATE_NAME = 375,
     ATTRIBUTE_KW = 376,
     THIS_KW = 377,
     NTA = 378,
     YTA = 379,
     YTT = 380,
     NTY = 381,
     YTY = 382,
     CTORN = 383,
     SHIFT_THERE = 384
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 385 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"

__int64 num;
double double_num;
System::StringA* id;
System::StringA* string_literal;
ast::A_ClassKey classKey;
ast::A_ClassHead* classHead;
AccessSpec accessSpec;
ast::DeclSpecifierKeyword declSpecifierKeyword;
ast::A_MemberSpec* memberSpec;
//ast::A_Condition* cond;
ast::Expression* exp;
ast::ExpressionList* expList;

ast::Expression2* exp2;
ast::Expression2List* exp2List;

ast::A_Stm* stm;
ast::CompoundStm* compoundstm;
ast::A_StmList* stmList;
ast::A_Declarator* declarator;
ast::A_DirectDeclarator* directDeclarator;
ast::Declaration* declaration;
ast::DeclarationList* declarationList;
ast::A_DeclaratorList* declaratorList;
//ast::A_TypeSpec* typespec;
ast::A_TypeId* typeId;
ast::A_TypeIdList* typeIdList;
/*ast::A_TypeSpecList* typespecList;*/

ast::TemplateParameter* templateParameter;
ast::TemplateParameterList* templateParameterList;

//ast::A_TemplateArgument* templateArgument;
ast::A_TemplateArgumentList* templateArgumentList;

ast::A_DeclSpec* declspec;
ast::A_DeclSpec* typespec;
ast::DeclSpecList* declspecList;
ast::ParameterDeclarations* paramDecls;
ast::EnumDefinition* enumDef;
ast::EnumDefList* enumList;
ast::A_BaseSpecifierList* baseSpecifierList;
ast::BasicTypeSpecifier simpleTypeSpec;



/* Line 214 of yacc.c  */
#line 675 "cpp_parse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 700 "cpp_parse.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  129
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   145

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  151
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  4
/* YYNRULES -- Number of rules.  */
#define YYNRULES  129
/* YYNRULES -- Number of states.  */
#define YYNSTATES  131

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   384

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   134,     2,     2,     2,   140,     2,     2,
     130,   131,     2,   137,   144,   138,   143,   139,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   146,   147,
     136,   133,   135,   145,   150,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   141,     2,   142,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   148,     2,   149,   132,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    29,    31,    33,    35,    37,
      39,    41,    43,    45,    47,    49,    51,    53,    55,    57,
      59,    61,    63,    65,    67,    69,    71,    73,    75,    77,
      79,    81,    83,    85,    87,    89,    91,    93,    95,    97,
      99,   101,   103,   105,   107,   109,   111,   113,   115,   117,
     119,   121,   123,   125,   127,   129,   131,   133,   135,   137,
     139,   141,   143,   145,   147,   149,   151,   153,   155,   157,
     159,   161,   163,   165,   167,   169,   171,   173,   175,   177,
     179,   181,   183,   185,   187,   189,   191,   193,   195,   197,
     199,   201,   203,   205,   207,   209,   211,   213,   215,   217,
     219,   221,   223,   225,   227,   229,   231,   233,   235,   237,
     239,   241,   243,   245,   247,   249,   251,   253,   255,   257
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     152,     0,    -1,   154,    -1,    65,    -1,    66,    -1,   117,
      -1,   118,    -1,   114,    -1,   115,    -1,   116,    -1,   107,
      -1,   119,    -1,     5,    -1,   122,    -1,   109,    -1,   110,
      -1,   111,    -1,    52,    -1,    51,    -1,    53,    -1,    81,
      -1,    50,    -1,    46,    -1,    47,    -1,    48,    -1,    61,
      -1,    62,    -1,    59,    -1,    63,    -1,    64,    -1,   121,
      -1,   104,    -1,   100,    -1,   101,    -1,   102,    -1,   103,
      -1,   108,    -1,    44,    -1,    45,    -1,    37,    -1,    35,
      -1,    36,    -1,    43,    -1,    72,    -1,    41,    -1,    42,
      -1,    38,    -1,    39,    -1,    40,    -1,    74,    -1,    32,
      -1,    34,    -1,    76,    -1,    77,    -1,    78,    -1,    80,
      -1,    79,    -1,    82,    -1,    83,    -1,    84,    -1,    85,
      -1,    86,    -1,    87,    -1,    95,    -1,    96,    -1,    97,
      -1,    98,    -1,    93,    -1,    94,    -1,    56,    -1,    55,
      -1,    54,    -1,    88,    -1,    49,    -1,    92,    -1,    90,
      -1,   147,    -1,   130,    -1,   131,    -1,   141,    -1,   142,
      -1,   148,    -1,   149,    -1,    68,    -1,    70,    -1,    69,
      -1,    71,    -1,   112,    -1,   113,    -1,   136,    -1,   135,
      -1,   137,    -1,   138,    -1,   139,    -1,   140,    -1,   133,
      -1,   132,    -1,   145,    -1,   146,    -1,   134,    -1,    10,
      -1,    11,    -1,    22,    -1,    23,    -1,   144,    -1,   143,
      -1,   150,    -1,   105,    -1,   106,    -1,    29,    -1,    28,
      -1,    26,    -1,    25,    -1,    27,    -1,     9,    -1,    24,
      -1,     7,    -1,     8,    -1,    12,    -1,    13,    -1,    17,
      -1,    18,    -1,    19,    -1,    14,    -1,    15,    -1,    16,
      -1,    20,    -1,    21,    -1,   153,    -1,   154,   153,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   800,   800,  1311,  1312,  1313,  1314,  1315,  1316,  1317,
    1318,  1319,  1323,  1327,  1328,  1329,  1330,  1331,  1332,  1333,
    1334,  1335,  1336,  1337,  1338,  1339,  1340,  1341,  1342,  1343,
    1345,  1347,  1349,  1350,  1351,  1352,  1354,  1356,  1357,  1358,
    1359,  1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,  1369,
    1370,  1371,  1373,  1374,  1375,  1376,  1377,  1378,  1379,  1380,
    1381,  1382,  1383,  1384,  1385,  1386,  1387,  1389,  1390,  1392,
    1393,  1394,  1396,  1398,  1400,  1401,  1404,  1408,  1409,  1410,
    1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,  1420,
    1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1433,  1434,  1435,  1436,  1438,  1443,  1444,  1445,
    1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453,  1454,  1455,
    1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,  1559,  1560
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "EXP_TOKEN", "DECLS_TOKEN", "IDENTIFIER",
  "identifier_lparen", "PLUSPLUS", "MINUSMINUS", "ARROW", "EQUAL",
  "NOT_EQUAL", "ASSIGN_PLUS", "ASSIGN_MINUS", "ASSIGN_OR", "ASSIGN_XOR",
  "ASSIGN_AND", "ASSIGN_MUL", "ASSIGN_DIV", "ASSIGN_MOD", "ASSIGN_LSHIFT",
  "ASSIGN_RSHIFT", "LEQ", "GEQ", "STAR", "BITAND", "BITOR", "BITXOR",
  "LAND", "LOR", "DOT_STAR", "ARROW_STAR", "NAMESPACE_KW", "USING_KW",
  "EXTERN_KW", "IF_KW", "ELSE_KW", "SWITCH_KW", "CASE_KW", "DEFAULT_KW",
  "GOTO_KW", "BREAK_KW", "CONTINUE_KW", "FOR_KW", "DO_KW", "WHILE_KW",
  "CONST_KW", "VOLATILE_KW", "TYPEDEF_KW", "TYPENAME_KW", "ENUM_KW",
  "CLASS_KW", "STRUCT_KW", "UNION_KW", "PRIVATE_KW", "PROTECTED_KW",
  "PUBLIC_KW", "AUTO_KW", "REGISTER_KW", "STATIC_KW", "MUTABLE_KW",
  "INLINE_KW", "VIRTUAL_KW", "EXPLICIT_KW", "FRIEND_KW", "FALSE_KW",
  "TRUE_KW", "STATIC_ASSERT_KW", "STATIC_CAST_KW", "REINTERPRET_CAST_KW",
  "DYNAMIC_CAST_KW", "CONST_CAST_KW", "RETURN_KW", "TRY_KW", "THROW_KW",
  "CATCH_KW", "CHAR_KW", "WCHAR_T_KW", "BOOL_KW", "SHORT_KW", "INT_KW",
  "INTERFACE_KW", "LONG_KW", "SIGNED_KW", "UNSIGNED_KW", "FLOAT_KW",
  "DOUBLE_KW", "VOID_KW", "TEMPLATE_KW", "EXPORT_KW", "NULLPTR_KW",
  "CONSTEXPR_KW", "DECLTYPE_KW", "__FLOAT32_KW", "__FLOAT64_KW",
  "__INT8_KW", "__INT16_KW", "__INT32_KW", "__INT64_KW", "__INT128_KW",
  "__CDECL_KW", "__STDCALL_KW", "__FASTCALL_KW", "__DECLSPEC_KW",
  "__ASM_KW", "LSHIFT", "RSHIFT", "TWO_COLON", "THREE_DOTS", "NEW_KW",
  "DELETE_KW", "OPERATOR_KW", "SIZEOF_KW", "TYPEID_KW", "STRING_LITERAL",
  "LSTRING_LITERAL", "CHAR_LITERAL", "INTEGER_LITERAL", "DOUBLE_LITERAL",
  "IMPORT_KW", "TEMPLATE_NAME", "ATTRIBUTE_KW", "THIS_KW", "NTA", "YTA",
  "YTT", "NTY", "YTY", "CTORN", "SHIFT_THERE", "'('", "')'", "'~'", "'='",
  "'!'", "'>'", "'<'", "'+'", "'-'", "'/'", "'%'", "'['", "']'", "'.'",
  "','", "'?'", "':'", "';'", "'{'", "'}'", "'@'", "$accept",
  "translation_unit", "prim", "primlist", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
      40,    41,   126,    61,    33,    62,    60,    43,    45,    47,
      37,    91,    93,    46,    44,    63,    58,    59,   123,   125,
      64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   151,   152,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   154,   154
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    12,   116,   117,   114,   100,   101,   118,   119,   123,
     124,   125,   120,   121,   122,   126,   127,   102,   103,   115,
     112,   111,   113,   110,   109,    50,    51,    40,    41,    39,
      46,    47,    48,    44,    45,    42,    37,    38,    22,    23,
      24,    73,    21,    18,    17,    19,    71,    70,    69,    27,
      25,    26,    28,    29,     3,     4,    83,    85,    84,    86,
      43,    49,    52,    53,    54,    56,    55,    20,    57,    58,
      59,    60,    61,    62,    72,    75,    74,    67,    68,    63,
      64,    65,    66,    32,    33,    34,    35,    31,   107,   108,
      10,    36,    14,    15,    16,    87,    88,     7,     8,     9,
       5,     6,    11,    30,    13,    77,    78,    96,    95,    99,
      90,    89,    91,    92,    93,    94,    79,    80,   105,   104,
      97,    98,    76,    81,    82,   106,     0,   128,     2,     1,
     129
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   126,   127,   128
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -104
static const yytype_int8 yypact[] =
{
      -5,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,     1,  -104,    -5,  -104,
    -104
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -104,  -104,  -103,  -104
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       1,   129,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,   130,     0,    25,     0,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,     0,     0,    49,     0,    50,    51,    52,    53,
      54,    55,     0,    56,    57,    58,    59,    60,     0,    61,
       0,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,     0,    75,     0,    76,    77,    78,
      79,    80,    81,    82,     0,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,     0,   103,   104,     0,     0,
       0,     0,     0,     0,     0,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125
};

static const yytype_int16 yycheck[] =
{
       5,     0,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,   128,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    -1,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    -1,    92,    93,    94,
      95,    96,    97,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    32,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    59,
      61,    62,    63,    64,    65,    66,    68,    69,    70,    71,
      72,    74,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    90,    92,    93,    94,    95,
      96,    97,    98,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   121,   122,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   152,   153,   154,     0,
     153
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 800 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2List) = (yyvsp[(1) - (1)].exp2List); g_primlist = (yyval.exp2List);	;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 1311 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), FALSE_KW) ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 1312 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), TRUE_KW) ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 1313 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), INTEGER_LITERAL, (yyvsp[(1) - (1)].num)) ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 1314 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), DOUBLE_LITERAL, (yyvsp[(1) - (1)].double_num)) ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 1315 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), STRING_LITERAL, (yyvsp[(1) - (1)].string_literal))	;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 1316 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), LSTRING_LITERAL) ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 1317 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), CHAR_LITERAL, (yyvsp[(1) - (1)].num)) ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 1318 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), TWO_COLON) ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 1320 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    {
	  (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), IMPORT_KW, (yyvsp[(1) - (1)].string_literal))
	;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 1324 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    {
		 (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), IDENTIFIER, (yyvsp[(1) - (1)].id))
		 ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 1327 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), THIS_KW) ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 1328 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), NEW_KW) ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 1329 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), DELETE_KW) ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 1330 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), OPERATOR_KW) ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 1331 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), STRUCT_KW) ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 1332 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), CLASS_KW) ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 1333 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), UNION_KW) ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 1334 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), INTERFACE_KW) ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 1335 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ENUM_KW) ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 1336 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), CONST_KW) ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 1337 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), VOLATILE_KW) ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 1338 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), TYPEDEF_KW) ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 1339 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), INLINE_KW) ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 1340 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), VIRTUAL_KW) ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 1341 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), STATIC_KW) ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 1342 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), EXPLICIT_KW) ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 1343 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), FRIEND_KW) ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 1345 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ATTRIBUTE_KW) ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 1347 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __ASM_KW) ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 1349 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __CDECL_KW) ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 1350 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __STDCALL_KW) ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 1351 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __FASTCALL_KW) ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 1352 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __DECLSPEC_KW) ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 1354 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), THREE_DOTS) ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 1356 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), DO_KW) ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 1357 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), WHILE_KW) ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 1358 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), SWITCH_KW) ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 1359 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), IF_KW) ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 1360 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ELSE_KW) ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 1361 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), FOR_KW) ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 1362 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), RETURN_KW) ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 1363 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), BREAK_KW) ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 1364 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), CONTINUE_KW) ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 1365 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), CASE_KW) ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 1366 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), DEFAULT_KW) ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 1367 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), GOTO_KW) ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 1369 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), THROW_KW) ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 1370 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), NAMESPACE_KW) ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 1371 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), EXTERN_KW) ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 1373 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), CHAR_KW) ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 1374 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), WCHAR_T_KW) ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 1375 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), BOOL_KW) ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 1376 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), INT_KW) ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 1377 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), SHORT_KW) ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 1378 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), LONG_KW) ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 1379 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), SIGNED_KW) ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 1380 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), UNSIGNED_KW) ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 1381 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), FLOAT_KW) ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 1382 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), DOUBLE_KW) ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 1383 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), VOID_KW) ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 1384 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __INT8_KW) ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 1385 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __INT16_KW) ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 1386 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __INT32_KW) ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 1387 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __INT64_KW) ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 1389 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __FLOAT32_KW) ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 1390 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), __FLOAT64_KW) ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 1392 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), PUBLIC_KW) ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 1393 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), PROTECTED_KW) ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 1394 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), PRIVATE_KW) ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 1396 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), TEMPLATE_KW) ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 1398 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), TYPENAME_KW) ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 1400 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), DECLTYPE_KW) ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 1401 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), NULLPTR_KW) ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 1404 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ';') ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 1408 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '(') ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1409 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ')') ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 1410 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '[') ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 1411 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ']') ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 1412 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '{') ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1413 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '}') ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1414 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), STATIC_CAST_KW) ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1415 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), DYNAMIC_CAST_KW) ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1416 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), REINTERPRET_CAST_KW) ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1417 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), CONST_CAST_KW) ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1418 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), SIZEOF_KW) ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1419 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), TYPEID_KW) ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1420 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '<') ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1421 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '>') ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1422 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '+') ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1423 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '-') ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1424 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '/') ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1425 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '%') ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1426 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '=') ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1427 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '~') ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1428 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '?') ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 1429 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ':') ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 1430 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '!') ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1431 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), EQUAL) ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1432 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), NOT_EQUAL) ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1433 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), LEQ) ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1434 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), GEQ) ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1435 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ',') ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1436 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '.') ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1439 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    {
	(yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), '#')
	;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1443 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), LSHIFT) ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1444 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), RSHIFT) ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 1445 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), LOR) ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 1446 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), LAND) ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 1447 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), BITOR) ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 1448 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), BITAND) ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1449 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), BITXOR) ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1450 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ARROW) ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1451 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), STAR) ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1452 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), PLUSPLUS) ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1453 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), MINUSMINUS) ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1454 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_PLUS) ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 1455 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_MINUS) ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1456 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_MUL) ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1457 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_DIV) ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1458 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_MOD) ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1459 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_OR) ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1460 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_XOR) ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1461 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_AND) ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1462 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_LSHIFT) ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1463 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2) = new ast::Expression2((yylsp[(1) - (1)]), ASSIGN_RSHIFT) ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1559 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2List) = new ast::Expression2List((yyvsp[(1) - (1)].exp2), NULL) ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1560 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"
    { (yyval.exp2List) = new ast::Expression2List((yyvsp[(2) - (2)].exp2), (yyvsp[(1) - (2)].exp2List)) ;}
    break;



/* Line 1455 of yacc.c  */
#line 3010 "cpp_parse.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 2515 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_parse.y"


static ast::Declaration* Merge (YYSTYPE& x0, YYSTYPE& x1)
{
	return ast::A_Declaration2(x0.declaration, x1.declaration);
}

#if 0
static ast::A_Stm*
     stmtMerge (YYSTYPE& x0, YYSTYPE& x1)
     {
	  //YYSTYPE ytype;
     //  printf ("<OR> ");
      // return ytype;

		ast::A_Stm* p = new ast::A_ExpOrDeclStm(x1.exp, x0.declaration);
		//p->kind = ast::A_Stm::ast::A_EXP_OR_DECLARATION;
		//p->EXP_OR_DECL.exp = x1.exp;
		//p->EXP_OR_DECL.declaration = x0.declaration;
		 return p;
     }
#endif

/*
static ast::A_TemplateArgument*
     template_argument_merge (YYSTYPE& x0, YYSTYPE& x1)
     {

		ast::A_TemplateArgument* p = new ast::A_TemplateArgument(x0.templateArgument->m_pTypeId, x1.templateArgument->m_pExp);
		 return p;
     }
*/

