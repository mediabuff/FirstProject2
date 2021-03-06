
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
#define yyparse         xpath_parse
#define yylex           xpath_lex
#define yyerror         xpath_error
#define yylval          xpath_lval
#define yychar          xpath_char
#define yydebug         xpath_debug
#define yynerrs         xpath_nerrs
#define yylloc          xpath_lloc

/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"


void xpath_error (/*YYLTYPE *llocp,*/ char const *s);
int xpath_lex(/*YYSTYPE *lvalp, YYLTYPE *llocp*/);

extern Exp* g_exp;

#define YYERROR_VERBOSE

#define yyfalse	0
#define yytrue		1

union YYSTYPE;

     

/* Line 189 of yacc.c  */
#line 98 "xpath_parse.tab.c"

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
     INTEGER_LITERAL = 258,
     STRING_LITERAL = 259,
     identifier = 260,
     TWO_COLON = 261,
     TWO_PUNCT = 262,
     TWO_SLASH = 263,
     NCName = 264,
     AxisName = 265,
     OR = 266,
     AND = 267,
     EQ = 268,
     NEQ = 269,
     LTEQ = 270,
     GTEQ = 271,
     DIV = 272,
     IDIV = 273,
     MOD = 274,
     ELEMENT_KW = 275,
     INSTANCE_KW = 276,
     OF_KW = 277,
     EMPTY_SEQUENCE_KW = 278,
     EMPTY_ELEMENT = 279,
     ELEMENT_END = 280
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 21 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"

int num;
System::StringW* id;
System::StringW* string_literal;
Exp* exp;
Step* step;
StepList* steps;
ExpList* expList;
QName* qname;
NodeTest* nodetest;



/* Line 214 of yacc.c  */
#line 173 "xpath_parse.tab.c"
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
#line 198 "xpath_parse.tab.c"

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
#define YYFINAL  47
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   133

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  51
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNRULES -- Number of states.  */
#define YYNSTATES  146

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      30,    31,    32,    38,    26,    39,    28,    27,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    33,     2,
      36,     2,    37,    40,    29,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    34,     2,    35,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    11,    13,    15,    17,    21,
      23,    25,    29,    32,    34,    37,    38,    40,    42,    44,
      48,    53,    60,    62,    64,    66,    68,    70,    74,    78,
      80,    84,    88,    90,    92,    96,    98,   100,   102,   104,
     106,   110,   115,   117,   119,   121,   126,   135,   136,   138,
     140,   143,   147,   149,   152,   154,   156,   160,   162,   166,
     168,   172,   176,   178,   182,   186,   190,   194,   196,   200,
     204,   206,   210,   214,   218,   222,   224,   227,   230,   232,
     234,   236,   238,   240,   242,   244,   246,   251,   255,   257,
     260,   262,   264,   266,   268
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      42,     0,    -1,    43,    -1,    44,    -1,    43,    26,    44,
      -1,    73,    -1,    46,    -1,    47,    -1,    47,    27,    46,
      -1,    48,    -1,    91,    -1,    49,    52,    51,    -1,    10,
       6,    -1,    58,    -1,    50,    58,    -1,    -1,    50,    -1,
      54,    -1,    53,    -1,    20,    30,    31,    -1,    20,    30,
      55,    31,    -1,    20,    30,    55,    26,    57,    31,    -1,
      56,    -1,    57,    -1,    32,    -1,    57,    -1,    32,    -1,
       9,    33,    32,    -1,    32,    33,     9,    -1,     9,    -1,
       9,    33,     9,    -1,    34,    59,    35,    -1,    43,    -1,
       3,    -1,    30,    43,    31,    -1,    60,    -1,    64,    -1,
      63,    -1,    65,    -1,    44,    -1,    62,    26,    44,    -1,
      57,    30,    62,    31,    -1,    28,    -1,    66,    -1,    67,
      -1,    36,    57,    68,    24,    -1,    36,    57,    68,    37,
      71,    25,    57,    37,    -1,    -1,    69,    -1,    70,    -1,
      69,    70,    -1,    57,    13,     4,    -1,    72,    -1,    71,
      72,    -1,    66,    -1,    74,    -1,    73,    11,    74,    -1,
      75,    -1,    74,    12,    75,    -1,    76,    -1,    75,    13,
      76,    -1,    75,    14,    76,    -1,    77,    -1,    76,    36,
      77,    -1,    76,    37,    77,    -1,    76,    15,    77,    -1,
      76,    16,    77,    -1,    78,    -1,    77,    38,    78,    -1,
      77,    39,    78,    -1,    81,    -1,    78,    32,    81,    -1,
      78,    17,    81,    -1,    78,    18,    81,    -1,    78,    19,
      81,    -1,    80,    -1,    39,    79,    -1,    38,    79,    -1,
      83,    -1,    82,    -1,    87,    -1,    45,    -1,    79,    -1,
      84,    -1,    85,    -1,    86,    -1,    86,    21,    22,    88,
      -1,    23,    30,    31,    -1,    89,    -1,    89,    90,    -1,
      53,    -1,    40,    -1,    32,    -1,    38,    -1,    61,    51,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   105,   105,   109,   110,   113,   116,   124,   125,   149,
     150,   153,   157,   160,   161,   164,   165,   168,   169,   176,
     177,   178,   181,   182,   185,   186,   189,   190,   191,   194,
     195,   198,   201,   204,   208,   210,   211,   212,   213,   216,
     217,   220,   222,   225,   228,   234,   235,   238,   239,   242,
     243,   246,   249,   250,   253,   256,   257,   260,   261,   264,
     265,   266,   268,   269,   270,   271,   272,   275,   276,   277,
     280,   281,   282,   283,   284,   287,   288,   289,   292,   301,
     305,   308,   315,   318,   321,   324,   325,   328,   329,   330,
     333,   336,   337,   338,   344
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER_LITERAL", "STRING_LITERAL",
  "identifier", "TWO_COLON", "TWO_PUNCT", "TWO_SLASH", "NCName",
  "AxisName", "OR", "AND", "EQ", "NEQ", "LTEQ", "GTEQ", "DIV", "IDIV",
  "MOD", "ELEMENT_KW", "INSTANCE_KW", "OF_KW", "EMPTY_SEQUENCE_KW",
  "EMPTY_ELEMENT", "ELEMENT_END", "','", "'/'", "'.'", "'@'", "'('", "')'",
  "'*'", "':'", "'['", "']'", "'<'", "'>'", "'+'", "'-'", "'?'", "$accept",
  "XPath", "Expr", "ExprSingle", "LocationPath", "RelativeLocationPath",
  "StepExpr", "AxisStep", "AxisSpecifier", "Predicates", "Predicates_opt",
  "NodeTest", "KindTest", "NameTest", "QNameOrWildcard", "Wildcard",
  "QName", "Predicate", "PredicateExpr", "Number", "PrimaryExpr",
  "ExprList", "FunctionCall", "ContextItemExpr", "Constructor",
  "DirectConstructor", "DirElemConstructor", "DirAttributeList_opt",
  "DirAttributeList", "DirAttribute", "DirElemContents", "DirElemContent",
  "OrExpr", "AndExpr", "EqualityExpr", "RelationalExpr", "AdditiveExpr",
  "MultiplicativeExpr", "UnaryExpr", "ValueExpr", "UnionExpr",
  "IntersectExceptExpr", "PathExpr", "CastExpr", "CastableExpr",
  "TreatExpr", "InstanceofExpr", "SequenceType", "ItemType",
  "OccurrenceIndicator", "FilterExpr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    44,    47,    46,    64,
      40,    41,    42,    58,    91,    93,    60,    62,    43,    45,
      63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    45,    46,    46,    47,
      47,    48,    49,    50,    50,    51,    51,    52,    52,    53,
      53,    53,    54,    54,    55,    55,    56,    56,    56,    57,
      57,    58,    59,    60,    61,    61,    61,    61,    61,    62,
      62,    63,    64,    65,    66,    67,    67,    68,    68,    69,
      69,    70,    71,    71,    72,    73,    73,    74,    74,    75,
      75,    75,    76,    76,    76,    76,    76,    77,    77,    77,
      78,    78,    78,    78,    78,    79,    79,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    87,    88,    88,    88,
      89,    90,    90,    90,    91
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     3,     2,     1,     2,     0,     1,     1,     1,     3,
       4,     6,     1,     1,     1,     1,     1,     3,     3,     1,
       3,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       3,     4,     1,     1,     1,     4,     8,     0,     1,     1,
       2,     3,     1,     2,     1,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     1,     2,
       1,     1,     1,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    33,    29,     0,    42,     0,     0,     0,     0,     0,
       2,     3,    81,     6,     7,     9,     0,     0,    35,    15,
      37,    36,    38,    43,    44,     5,    55,    57,    59,    62,
      67,    82,    75,    70,    79,    78,    83,    84,    85,    80,
      10,     0,    12,     0,    47,    77,    76,     1,     0,     0,
      29,     0,    26,    15,    18,    17,    22,    23,     0,     0,
      16,    94,    13,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    30,    34,
       0,     0,    48,    49,     4,     8,     0,     0,     0,    11,
      39,     0,    32,     0,    14,    56,    58,    60,    61,    65,
      66,    63,    64,    68,    69,    72,    73,    74,    71,     0,
       0,    45,     0,    50,    27,    19,    24,     0,    25,    28,
       0,    41,    31,     0,    90,    86,    88,    51,    54,     0,
      52,     0,    20,    40,     0,    92,    93,    91,    89,     0,
      53,     0,    87,     0,    21,    46
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    60,
      61,    53,    54,    55,   117,    56,    17,    62,    93,    18,
      19,    91,    20,    21,    22,    23,    24,    81,    82,    83,
     129,   130,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   125,   126,   138,
      40
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -110
static const yytype_int8 yypact[] =
{
       3,  -110,   -14,    10,  -110,     3,    14,     3,     3,    26,
       6,  -110,  -110,  -110,     7,  -110,    -5,    35,  -110,    18,
    -110,  -110,  -110,  -110,  -110,    38,    47,    50,    21,    32,
      29,  -110,  -110,  -110,  -110,  -110,  -110,  -110,    51,  -110,
    -110,    77,  -110,   -17,    14,  -110,  -110,  -110,     3,    15,
      54,    58,    56,    18,  -110,  -110,  -110,  -110,     3,     3,
      18,  -110,  -110,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,    68,  -110,  -110,
      78,   -16,    14,  -110,  -110,  -110,    -4,    -2,    83,  -110,
    -110,    -9,     6,    59,  -110,    47,    50,    21,    21,    32,
      32,    32,    32,    29,    29,  -110,  -110,  -110,  -110,    30,
      89,  -110,    60,  -110,  -110,  -110,  -110,     9,  -110,  -110,
       3,  -110,  -110,    65,  -110,  -110,    22,  -110,  -110,    19,
    -110,    14,  -110,  -110,    66,  -110,  -110,  -110,  -110,    14,
    -110,    69,  -110,    61,  -110,  -110
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -110,  -110,    -3,   -47,  -110,    52,  -110,  -110,  -110,  -110,
      46,  -110,    -7,  -110,  -110,  -110,    -6,    43,  -110,  -110,
    -110,  -110,  -110,  -110,  -110,  -109,  -110,  -110,  -110,    23,
    -110,   -25,  -110,    44,    42,    17,    -1,    13,    67,  -110,
       4,  -110,  -110,  -110,  -110,  -110,  -110,  -110,  -110,  -110,
    -110
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      44,    84,    43,   128,    50,    78,     1,     2,   111,    48,
      57,    90,     2,     3,    79,    51,    42,   120,     1,    41,
     128,   112,   121,     2,     2,     3,    47,    52,   114,   115,
     116,     4,    48,     5,    49,   131,    67,    68,    80,     6,
     132,     7,     8,     4,   139,     5,    73,    74,    75,    63,
      51,     6,    59,   123,   135,     6,    92,    69,    70,    64,
     136,    76,   137,    65,    66,    58,    99,   100,   101,   102,
      71,    72,    77,   133,    45,    46,    80,   105,   106,   107,
     108,   118,    97,    98,   103,   104,    78,    86,    87,    88,
     109,   110,   119,   127,   122,   134,     6,   142,   145,    89,
     144,    85,   124,    94,   140,   113,    96,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,   143
};

static const yytype_int16 yycheck[] =
{
       6,    48,     5,   112,     9,     9,     3,     9,    24,    26,
      16,    58,     9,    10,    31,    20,     6,    26,     3,    33,
     129,    37,    31,     9,     9,    10,     0,    32,    32,    31,
      32,    28,    26,    30,    27,    26,    15,    16,    44,    36,
      31,    38,    39,    28,    25,    30,    17,    18,    19,    11,
      20,    36,    34,    23,    32,    36,    59,    36,    37,    12,
      38,    32,    40,    13,    14,    30,    67,    68,    69,    70,
      38,    39,    21,   120,     7,     8,    82,    73,    74,    75,
      76,    87,    65,    66,    71,    72,     9,    33,    30,    33,
      22,    13,     9,     4,    35,    30,    36,    31,    37,    53,
      31,    49,   109,    60,   129,    82,    64,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     9,    10,    28,    30,    36,    38,    39,    42,
      43,    44,    45,    46,    47,    48,    49,    57,    60,    61,
      63,    64,    65,    66,    67,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      91,    33,     6,    43,    57,    79,    79,     0,    26,    27,
       9,    20,    32,    52,    53,    54,    56,    57,    30,    34,
      50,    51,    58,    11,    12,    13,    14,    15,    16,    36,
      37,    38,    39,    17,    18,    19,    32,    21,     9,    31,
      57,    68,    69,    70,    44,    46,    33,    30,    33,    51,
      44,    62,    43,    59,    58,    74,    75,    76,    76,    77,
      77,    77,    77,    78,    78,    81,    81,    81,    81,    22,
      13,    24,    37,    70,    32,    31,    32,    55,    57,     9,
      26,    31,    35,    23,    53,    88,    89,     4,    66,    71,
      72,    26,    31,    44,    30,    32,    38,    40,    90,    25,
      72,    57,    31,    57,    31,    37
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
#line 105 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); g_exp = (yyval.exp); YYACCEPT ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 109 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp) ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 110 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), ',', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 124 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.steps) = new StepList((yyvsp[(1) - (1)].step), NULL) ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 125 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.steps) = new StepList((yyvsp[(1) - (3)].step), (yyvsp[(3) - (3)].steps)) ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 153 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.step) = new AxisStep((yyvsp[(1) - (3)].num), (yyvsp[(2) - (3)].nodetest), Reverse((yyvsp[(3) - (3)].expList))) ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 160 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.expList) = new ExpList((yyvsp[(1) - (1)].exp), NULL) ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 161 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.expList) = new ExpList((yyvsp[(2) - (2)].exp), (yyvsp[(1) - (2)].expList)) ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 164 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.expList) = NULL ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 176 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.nodetest) = new ElementNodeTest(NULL, nullptr) ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 177 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.nodetest) = new ElementNodeTest((yyvsp[(3) - (4)].qname), nullptr) ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 178 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.nodetest) = new ElementNodeTest((yyvsp[(3) - (6)].qname), (yyvsp[(5) - (6)].qname)) ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 181 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.nodetest) = new NameNodeTest((yyvsp[(1) - (1)].qname)) ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 182 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.nodetest) = new NameNodeTest((yyvsp[(1) - (1)].qname)) ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 185 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.qname) = nullptr ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 189 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.qname) = NULL ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 190 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.qname) = new QName((yyvsp[(1) - (3)].id), &star_str) ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 191 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.qname) = new QName(&star_str, (yyvsp[(3) - (3)].id)) ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 194 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.qname) = new QName(nullptr, (yyvsp[(1) - (1)].id)) ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 195 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.qname) = new QName((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id)) ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 198 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = (yyvsp[(2) - (3)].exp) ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 204 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = NumberExp((yyvsp[(1) - (1)].num)) ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 208 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = (yyvsp[(2) - (3)].exp) ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 216 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.expList) = new ExpList((yyvsp[(1) - (1)].exp), NULL) ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 217 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.expList) = new ExpList((yyvsp[(3) - (3)].exp), (yyvsp[(1) - (3)].expList)) ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 220 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new FunctionCallExp((yyvsp[(1) - (4)].qname), Reverse((yyvsp[(3) - (4)].expList))) ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 222 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new ContextItemExp() ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 260 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp) ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 261 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), 'AND', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 265 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), '=', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 266 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), '!=', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 269 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), '<', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 270 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), '>', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 271 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), '<=', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 272 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), '>=', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 276 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), '+', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 277 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), '-', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 281 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), '*', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 282 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), 'div', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 283 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), 'idiv', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 284 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new BinopExp((yyvsp[(1) - (3)].exp), 'mod', (yyvsp[(3) - (3)].exp)) ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 288 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new UnopExp((yyvsp[(2) - (2)].exp), '-') ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 289 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new UnopExp((yyvsp[(2) - (2)].exp), '+') ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 308 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new PathExp((yyvsp[(1) - (1)].steps));}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 325 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.exp) = new InstanceOfExp((yyvsp[(1) - (4)].exp), (yyvsp[(4) - (4)].nodetest)) ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 328 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.nodetest) = NULL ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 330 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { ASSERT(0) ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 344 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"
    { (yyval.step) = new FilterStep((yyvsp[(1) - (2)].exp), Reverse((yyvsp[(2) - (2)].expList))) ;}
    break;



/* Line 1455 of yacc.c  */
#line 1912 "xpath_parse.tab.c"
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
#line 347 "D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\src\\XPath\\xpath_parse.y"


