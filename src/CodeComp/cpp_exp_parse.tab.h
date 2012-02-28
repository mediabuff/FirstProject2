
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 197 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\CodeComp\\cpp_exp_parse.y"

LONGLONG num;
double doubleValue;
System::StringA* id;
System::StringA* string_literal;
ast::A_ClassKey classKey;
ast::A_ClassHead* classHead;
AccessSpec accessSpec;
ast::DeclSpecifierKeyword declSpecifierKeyword;
ast::A_MemberSpec* memberSpec;
ast::Condition* cond;
ast::Expression* exp;
ast::ExpressionList* expList;

ast::Expression2* exp2;
ast::Expression2List* exp2List;

ast::A_ExpOrDecl* exp_or_decl;

ast::Initializer* initializer;

ast::InitDeclarator* initDeclarator;
ast::InitDeclaratorList* initDeclaratorList;

ast::A_Stm* stm;
ast::CompoundStm* compoundstm;
ast::A_StmList* stmList;
ast::A_Declarator* declarator;
ast::A_DirectDeclarator* directDeclarator;
ast::Declaration* declaration;
ast::DeclarationList* declarationList;
ast::A_DeclaratorList* declaratorList;
ast::A_TypeId* typeId;
ast::A_TypeIdList* typeIdList;

ast::TemplateParameter* templateParameter;
ast::TemplateParameterList* templateParameterList;

ast::A_TemplateArgument* templateArgument;
ast::A_TemplateArgumentList* templateArgumentList;

Scope* pScope;

ast::A_DeclSpecExt* declspecExt;
ast::A_DeclSpecExtList* declspecExtList;

ast::A_DeclSpec* declspec;
ast::A_DeclSpec* typespec;
ast::DeclSpecList* declspecList;
ast::ParameterDeclarations* paramDecls;
ast::EnumDefinition* enumDef;
ast::EnumDefList* enumList;
ast::A_BaseSpecifier* baseSpecifier;
ast::A_BaseSpecifierList* baseSpecifierList;
ast::BasicTypeSpecifier simpleTypeSpec;



/* Line 1676 of yacc.c  */
#line 240 "cpp_exp_parse.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE cpp_exp_lval;

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

extern YYLTYPE cpp_exp_lloc;

