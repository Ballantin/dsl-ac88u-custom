/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 31 "cfparse.y"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <sys/time.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include "dhcp6.h"
#include "config.h"
#include "common.h"

extern int lineno;
extern int cfdebug;

extern void yywarn __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));
extern void yyerror __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));

#define MAKE_NAMELIST(l, n, p) do { \
	(l) = (struct cf_namelist *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (p) cleanup_cflist(p); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->name = (n); \
	l->params = (p); \
	} while (0)

#define MAKE_CFLIST(l, t, pp, pl) do { \
	(l) = (struct cf_list *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (pp) free(pp); \
		if (pl) cleanup_cflist(pl); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->type = (t); \
	l->ptr = (pp); \
	l->list = (pl); \
	l->tail = (l); \
	} while (0)

static struct cf_namelist *iflist_head, *hostlist_head, *iapdlist_head;
static struct cf_namelist *addrpoollist_head;
static struct cf_namelist *authinfolist_head, *keylist_head;
static struct cf_namelist *ianalist_head;
struct cf_list *cf_dns_list, *cf_dns_name_list, *cf_ntp_list;
struct cf_list *cf_sip_list, *cf_sip_name_list;
struct cf_list *cf_nis_list, *cf_nis_name_list;
struct cf_list *cf_nisp_list, *cf_nisp_name_list;
struct cf_list *cf_bcmcs_list, *cf_bcmcs_name_list;
long long cf_refreshtime = -1;

extern int yylex __P((void));
extern int cfswitch_buffer __P((char *));
static int add_namelist __P((struct cf_namelist *, struct cf_namelist **));
static void cleanup __P((void));
static void cleanup_namelist __P((struct cf_namelist *));
static void cleanup_cflist __P((struct cf_list *));

/* Line 371 of yacc.c  */
#line 141 "y.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTERFACE = 258,
     IFNAME = 259,
     PREFIX_INTERFACE = 260,
     SLA_ID = 261,
     SLA_LEN = 262,
     DUID_ID = 263,
     ID_ASSOC = 264,
     IA_PD = 265,
     IAID = 266,
     IA_NA = 267,
     ADDRESS = 268,
     REQUEST = 269,
     SEND = 270,
     ALLOW = 271,
     PREFERENCE = 272,
     HOST = 273,
     HOSTNAME = 274,
     DUID = 275,
     OPTION = 276,
     RAPID_COMMIT = 277,
     DNS_SERVERS = 278,
     DNS_NAME = 279,
     NTP_SERVERS = 280,
     REFRESHTIME = 281,
     SIP_SERVERS = 282,
     SIP_NAME = 283,
     NIS_SERVERS = 284,
     NIS_NAME = 285,
     NISP_SERVERS = 286,
     NISP_NAME = 287,
     BCMCS_SERVERS = 288,
     BCMCS_NAME = 289,
     INFO_ONLY = 290,
     SCRIPT = 291,
     DELAYEDKEY = 292,
     AUTHENTICATION = 293,
     PROTOCOL = 294,
     ALGORITHM = 295,
     DELAYED = 296,
     RECONFIG = 297,
     HMACMD5 = 298,
     MONOCOUNTER = 299,
     AUTHNAME = 300,
     RDM = 301,
     KEY = 302,
     KEYINFO = 303,
     REALM = 304,
     KEYID = 305,
     SECRET = 306,
     KEYNAME = 307,
     EXPIRE = 308,
     ADDRPOOL = 309,
     POOLNAME = 310,
     RANGE = 311,
     TO = 312,
     ADDRESS_POOL = 313,
     INCLUDE = 314,
     AFTR = 315,
     NUMBER = 316,
     SLASH = 317,
     EOS = 318,
     BCL = 319,
     ECL = 320,
     STRING = 321,
     QSTRING = 322,
     PREFIX = 323,
     INFINITY = 324,
     COMMA = 325
   };
#endif
/* Tokens.  */
#define INTERFACE 258
#define IFNAME 259
#define PREFIX_INTERFACE 260
#define SLA_ID 261
#define SLA_LEN 262
#define DUID_ID 263
#define ID_ASSOC 264
#define IA_PD 265
#define IAID 266
#define IA_NA 267
#define ADDRESS 268
#define REQUEST 269
#define SEND 270
#define ALLOW 271
#define PREFERENCE 272
#define HOST 273
#define HOSTNAME 274
#define DUID 275
#define OPTION 276
#define RAPID_COMMIT 277
#define DNS_SERVERS 278
#define DNS_NAME 279
#define NTP_SERVERS 280
#define REFRESHTIME 281
#define SIP_SERVERS 282
#define SIP_NAME 283
#define NIS_SERVERS 284
#define NIS_NAME 285
#define NISP_SERVERS 286
#define NISP_NAME 287
#define BCMCS_SERVERS 288
#define BCMCS_NAME 289
#define INFO_ONLY 290
#define SCRIPT 291
#define DELAYEDKEY 292
#define AUTHENTICATION 293
#define PROTOCOL 294
#define ALGORITHM 295
#define DELAYED 296
#define RECONFIG 297
#define HMACMD5 298
#define MONOCOUNTER 299
#define AUTHNAME 300
#define RDM 301
#define KEY 302
#define KEYINFO 303
#define REALM 304
#define KEYID 305
#define SECRET 306
#define KEYNAME 307
#define EXPIRE 308
#define ADDRPOOL 309
#define POOLNAME 310
#define RANGE 311
#define TO 312
#define ADDRESS_POOL 313
#define INCLUDE 314
#define AFTR 315
#define NUMBER 316
#define SLASH 317
#define EOS 318
#define BCL 319
#define ECL 320
#define STRING 321
#define QSTRING 322
#define PREFIX 323
#define INFINITY 324
#define COMMA 325



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 127 "cfparse.y"

	long long num;
	char* str;
	struct cf_list *list;
	struct dhcp6_prefix *prefix;
	struct dhcp6_range *range;
	struct dhcp6_poolspec *pool;


/* Line 387 of yacc.c  */
#line 334 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

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

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 362 "y.tab.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

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

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   229

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNRULES -- Number of states.  */
#define YYNSTATES  232

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   325

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    30,    37,    42,    47,    52,    57,    62,
      67,    72,    77,    82,    87,    92,    97,   105,   112,   120,
     127,   134,   141,   145,   152,   153,   156,   158,   159,   162,
     166,   170,   173,   177,   181,   185,   189,   193,   197,   201,
     205,   209,   211,   215,   217,   220,   223,   226,   228,   230,
     232,   234,   236,   238,   240,   242,   244,   246,   248,   250,
     252,   256,   259,   263,   268,   274,   277,   281,   283,   285,
     286,   289,   291,   295,   302,   303,   306,   310,   314,   315,
     318,   322,   323,   326,   330,   334,   338,   342,   344,   346,
     348,   350,   351,   354,   358,   362,   366
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      72,     0,    -1,    -1,    72,    73,    -1,    74,    -1,    75,
      -1,    76,    -1,    77,    -1,    78,    -1,    79,    -1,    81,
      -1,    80,    -1,     3,     4,    64,    84,    65,    63,    -1,
      18,    19,    64,    84,    65,    63,    -1,    21,    23,    82,
      63,    -1,    21,    24,    67,    63,    -1,    21,    25,    82,
      63,    -1,    21,    27,    82,    63,    -1,    21,    28,    67,
      63,    -1,    21,    29,    82,    63,    -1,    21,    30,    67,
      63,    -1,    21,    31,    82,    63,    -1,    21,    32,    67,
      63,    -1,    21,    33,    82,    63,    -1,    21,    34,    67,
      63,    -1,    21,    26,    61,    63,    -1,     9,    10,    11,
      64,    93,    65,    63,    -1,     9,    10,    64,    93,    65,
      63,    -1,     9,    12,    11,    64,    98,    65,    63,    -1,
       9,    12,    64,    98,    65,    63,    -1,    38,    45,    64,
     100,    65,    63,    -1,    48,    52,    64,   105,    65,    63,
      -1,    59,    67,    63,    -1,    54,    55,    64,    84,    65,
      63,    -1,    -1,    82,    83,    -1,    66,    -1,    -1,    84,
      85,    -1,    15,    86,    63,    -1,    14,    86,    63,    -1,
      35,    63,    -1,    16,    87,    63,    -1,    20,     8,    63,
      -1,    13,    89,    63,    -1,    68,    90,    63,    -1,    17,
      61,    63,    -1,    36,    67,    63,    -1,    37,    66,    63,
      -1,    56,    88,    63,    -1,    58,    91,    63,    -1,    87,
      -1,    87,    70,    86,    -1,    22,    -1,    38,    45,    -1,
      10,    61,    -1,    12,    61,    -1,    27,    -1,    28,    -1,
      23,    -1,    24,    -1,    25,    -1,    26,    -1,    29,    -1,
      30,    -1,    31,    -1,    32,    -1,    33,    -1,    34,    -1,
      60,    -1,    66,    57,    66,    -1,    66,    92,    -1,    66,
      92,    92,    -1,    66,    62,    61,    92,    -1,    66,    62,
      61,    92,    92,    -1,    66,    92,    -1,    66,    92,    92,
      -1,    69,    -1,    61,    -1,    -1,    93,    94,    -1,    95,
      -1,    68,    90,    63,    -1,     5,     4,    64,    96,    65,
      63,    -1,    -1,    96,    97,    -1,     6,    61,    63,    -1,
       7,    61,    63,    -1,    -1,    98,    99,    -1,    13,    89,
      63,    -1,    -1,   100,   101,    -1,    39,   102,    63,    -1,
      40,   103,    63,    -1,    46,   104,    63,    -1,    47,    66,
      63,    -1,    41,    -1,    42,    -1,    43,    -1,    44,    -1,
      -1,   105,   106,    -1,    49,    67,    63,    -1,    50,    61,
      63,    -1,    51,    67,    63,    -1,    53,    67,    63,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   150,   150,   152,   156,   157,   158,   159,   160,   161,
     162,   163,   167,   179,   191,   200,   215,   224,   233,   248,
     257,   272,   281,   296,   305,   320,   349,   358,   372,   381,
     398,   410,   422,   433,   445,   446,   464,   487,   488,   506,
     514,   522,   530,   538,   546,   554,   562,   571,   579,   587,
     595,   606,   610,   620,   628,   636,   644,   652,   660,   668,
     676,   684,   692,   700,   708,   716,   724,   732,   740,   748,
     759,   790,   817,   850,   877,   910,   935,   966,   970,   977,
     978,   996,   997,  1008,  1018,  1019,  1037,  1045,  1056,  1057,
    1075,  1086,  1087,  1105,  1113,  1121,  1129,  1140,  1141,  1145,
    1149,  1153,  1154,  1172,  1180,  1188,  1196
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTERFACE", "IFNAME",
  "PREFIX_INTERFACE", "SLA_ID", "SLA_LEN", "DUID_ID", "ID_ASSOC", "IA_PD",
  "IAID", "IA_NA", "ADDRESS", "REQUEST", "SEND", "ALLOW", "PREFERENCE",
  "HOST", "HOSTNAME", "DUID", "OPTION", "RAPID_COMMIT", "DNS_SERVERS",
  "DNS_NAME", "NTP_SERVERS", "REFRESHTIME", "SIP_SERVERS", "SIP_NAME",
  "NIS_SERVERS", "NIS_NAME", "NISP_SERVERS", "NISP_NAME", "BCMCS_SERVERS",
  "BCMCS_NAME", "INFO_ONLY", "SCRIPT", "DELAYEDKEY", "AUTHENTICATION",
  "PROTOCOL", "ALGORITHM", "DELAYED", "RECONFIG", "HMACMD5", "MONOCOUNTER",
  "AUTHNAME", "RDM", "KEY", "KEYINFO", "REALM", "KEYID", "SECRET",
  "KEYNAME", "EXPIRE", "ADDRPOOL", "POOLNAME", "RANGE", "TO",
  "ADDRESS_POOL", "INCLUDE", "AFTR", "NUMBER", "SLASH", "EOS", "BCL",
  "ECL", "STRING", "QSTRING", "PREFIX", "INFINITY", "COMMA", "$accept",
  "statements", "statement", "interface_statement", "host_statement",
  "option_statement", "ia_statement", "authentication_statement",
  "key_statement", "include_statement", "addrpool_statement",
  "address_list", "address_list_ent", "declarations", "declaration",
  "dhcpoption_list", "dhcpoption", "rangeparam", "addressparam",
  "prefixparam", "poolparam", "duration", "iapdconf_list", "iapdconf",
  "prefix_interface", "ifparams", "ifparam", "ianaconf_list", "ianaconf",
  "authparam_list", "authparam", "authproto", "authalg", "authrdm",
  "keyparam_list", "keyparam", YY_NULL
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
     325
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    71,    72,    72,    73,    73,    73,    73,    73,    73,
      73,    73,    74,    75,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    77,    77,    77,    77,
      78,    79,    80,    81,    82,    82,    83,    84,    84,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    86,    86,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      88,    89,    89,    90,    90,    91,    91,    92,    92,    93,
      93,    94,    94,    95,    96,    96,    97,    97,    98,    98,
      99,   100,   100,   101,   101,   101,   101,   102,   102,   103,
     104,   105,   105,   106,   106,   106,   106
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     6,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     7,     6,     7,     6,
       6,     6,     3,     6,     0,     2,     1,     0,     2,     3,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     3,     4,     5,     2,     3,     1,     1,     0,
       2,     1,     3,     6,     0,     2,     3,     3,     0,     2,
       3,     0,     2,     3,     3,     3,     3,     1,     1,     1,
       1,     0,     2,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     3,     4,     5,     6,     7,     8,     9,    11,    10,
       0,     0,     0,     0,    34,     0,    34,     0,    34,     0,
      34,     0,    34,     0,    34,     0,     0,     0,     0,     0,
      37,     0,    79,     0,    88,    37,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,   101,
      37,    32,     0,    79,     0,    88,     0,     0,    14,    36,
      35,    15,    16,    25,    17,    18,    19,    20,    21,    22,
      23,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,    80,    81,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,   102,     0,     0,     0,     0,     0,    53,    59,    60,
      61,    62,    57,    58,    63,    64,    65,    66,    67,    68,
       0,    69,     0,    51,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,    12,     0,     0,     0,     0,
      27,     0,     0,     0,    29,    13,    97,    98,     0,    99,
       0,   100,     0,     0,    30,     0,     0,     0,     0,    31,
      33,    78,    77,    71,    44,    55,    56,    54,    40,     0,
      39,    42,    46,    43,    47,    48,     0,    49,    75,    50,
       0,    45,    26,    84,    82,    28,    90,    93,    94,    95,
      96,   103,   104,   105,   106,    72,    52,    70,    76,     0,
       0,    73,     0,     0,     0,    85,    74,     0,     0,    83,
      86,    87
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    46,    70,    62,    98,   142,   143,   152,   124,   157,
     154,   183,    64,   103,   104,   220,   225,    66,   108,    82,
     115,   168,   170,   172,    83,   121
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -153
static const yytype_int16 yypact[] =
{
    -153,     5,  -153,     2,    69,    11,   148,    -8,    18,   -11,
     -17,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
       9,    -7,    -4,    21,  -153,    26,  -153,    33,  -153,    34,
    -153,    37,  -153,    38,  -153,    39,    43,    44,    45,    50,
    -153,    51,  -153,    52,  -153,  -153,   -25,    54,   -21,    58,
     -15,    59,   -14,    60,    -5,    61,    20,    62,  -153,  -153,
    -153,  -153,    19,  -153,    -3,  -153,     6,    75,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,   -18,   -38,    83,    63,   132,   132,   132,    53,
     118,    64,    65,    68,    70,    71,    67,    72,  -153,    -2,
     124,    82,    72,  -153,  -153,     7,    63,    84,  -153,    86,
     -24,    92,   102,    87,    89,  -153,   100,   107,   116,   117,
     106,  -153,   122,   -45,   123,   126,   127,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
     105,  -153,   128,   119,   130,   131,   133,   134,  -153,   135,
     136,   138,   137,   -45,   139,  -153,   141,   142,   143,   140,
    -153,   144,   145,   146,  -153,  -153,  -153,  -153,   147,  -153,
     149,  -153,   150,   151,  -153,   152,   153,   154,   155,  -153,
    -153,  -153,  -153,   -45,  -153,  -153,  -153,  -153,  -153,   132,
    -153,  -153,  -153,  -153,  -153,  -153,   156,  -153,   -45,  -153,
     129,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,   -45,
       3,   -45,   158,   159,   160,  -153,  -153,   161,   162,  -153,
    -153,  -153
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,    48,  -153,   -20,  -153,   -87,   113,  -153,   115,   109,
    -153,  -152,   163,  -153,  -153,  -153,  -153,   164,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
     144,   198,   100,   100,    41,     2,    20,    43,     3,   222,
     223,   116,   117,   118,     4,   119,   181,   166,   167,   106,
     106,   110,   111,     5,   182,    67,     6,   120,   112,   113,
      23,   215,    85,    86,    87,    88,    89,    36,    68,    90,
      84,    69,    72,     7,    38,    69,   218,   114,    74,    76,
      39,    69,    69,     8,    91,    92,    93,    42,    78,     9,
      44,    69,   101,   158,    10,   102,   102,   221,   224,   226,
      37,   107,   162,    40,    48,    94,    50,    95,    52,    21,
      54,    22,    56,    80,    96,    45,    69,    97,    85,    86,
      87,    88,    89,    47,    49,    90,    85,    86,    87,    88,
      89,    51,   216,    90,    53,    55,    57,    58,    59,    60,
      91,    92,    93,    61,   146,    63,    65,    71,    91,    92,
      93,    73,    75,    77,    79,    81,   147,   148,   159,   123,
     155,    94,   149,    95,   150,   169,   151,   153,   156,    94,
     109,    95,   125,    97,   126,   160,   171,   164,   122,   165,
     187,    97,   174,   173,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   175,   176,   179,
     140,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,   177,   178,   180,   184,   185,   186,   189,
     219,   188,   141,   190,   191,   196,   192,   193,   194,   195,
     197,   145,   199,   200,   203,   201,   202,   204,   205,   206,
     207,   161,   208,   209,   210,   211,   212,   213,   214,   227,
     228,   163,   217,   229,   230,   231,    99,     0,     0,   105
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-153)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      87,   153,     5,     5,    11,     0,     4,    11,     3,     6,
       7,    49,    50,    51,     9,    53,    61,    41,    42,    13,
      13,    39,    40,    18,    69,    45,    21,    65,    46,    47,
      19,   183,    13,    14,    15,    16,    17,    45,    63,    20,
      60,    66,    63,    38,    55,    66,   198,    65,    63,    63,
      67,    66,    66,    48,    35,    36,    37,    64,    63,    54,
      64,    66,    65,    65,    59,    68,    68,   219,    65,   221,
      52,    65,    65,    64,    26,    56,    28,    58,    30,    10,
      32,    12,    34,    63,    65,    64,    66,    68,    13,    14,
      15,    16,    17,    67,    61,    20,    13,    14,    15,    16,
      17,    67,   189,    20,    67,    67,    67,    64,    64,    64,
      35,    36,    37,    63,    61,    64,    64,    63,    35,    36,
      37,    63,    63,    63,    63,    63,     8,    63,     4,    66,
      63,    56,    67,    58,    66,    43,    66,    66,    66,    56,
      65,    58,    10,    68,    12,    63,    44,    63,    65,    63,
      45,    68,    63,    66,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    67,    61,    63,
      38,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    67,    67,    63,    63,    61,    61,    70,
      61,    63,    60,    63,    63,    57,    63,    63,    63,    63,
      63,    88,    63,    62,    64,    63,    63,    63,    63,    63,
      63,   102,    63,    63,    63,    63,    63,    63,    63,    61,
      61,   106,    66,    63,    63,    63,    63,    -1,    -1,    65
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    72,     0,     3,     9,    18,    21,    38,    48,    54,
      59,    73,    74,    75,    76,    77,    78,    79,    80,    81,
       4,    10,    12,    19,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    45,    52,    55,    67,
      64,    11,    64,    11,    64,    64,    82,    67,    82,    61,
      82,    67,    82,    67,    82,    67,    82,    67,    64,    64,
      64,    63,    84,    64,    93,    64,    98,    84,    63,    66,
      83,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,   100,   105,    84,    13,    14,    15,    16,    17,
      20,    35,    36,    37,    56,    58,    65,    68,    85,    93,
       5,    65,    68,    94,    95,    98,    13,    65,    99,    65,
      39,    40,    46,    47,    65,   101,    49,    50,    51,    53,
      65,   106,    65,    66,    89,    10,    12,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      38,    60,    86,    87,    86,    87,    61,     8,    63,    67,
      66,    66,    88,    66,    91,    63,    66,    90,    65,     4,
      63,    90,    65,    89,    63,    63,    41,    42,   102,    43,
     103,    44,   104,    66,    63,    67,    61,    67,    67,    63,
      63,    61,    69,    92,    63,    61,    61,    45,    63,    70,
      63,    63,    63,    63,    63,    63,    57,    63,    92,    63,
      62,    63,    63,    64,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    92,    86,    66,    92,    61,
      96,    92,     6,     7,    65,    97,    92,    61,    61,    63,
      63,    63
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
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
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

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

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

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
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
  if (yypact_value_is_default (yyn))
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 12:
/* Line 1792 of yacc.c  */
#line 168 "cfparse.y"
    {
		struct cf_namelist *ifl;

		MAKE_NAMELIST(ifl, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(ifl, &iflist_head))
			return (-1);
	}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 180 "cfparse.y"
    {
		struct cf_namelist *host;

		MAKE_NAMELIST(host, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(host, &hostlist_head))
			return (-1);
	}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 192 "cfparse.y"
    {
			if (cf_dns_list == NULL)
				cf_dns_list = (yyvsp[(3) - (4)].list);
			else {
				cf_dns_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_dns_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 201 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_dns_name_list == NULL) {
				cf_dns_name_list = l;
				cf_dns_name_list->tail = l;
				cf_dns_name_list->next = NULL;
			} else {
				cf_dns_name_list->tail->next = l;
				cf_dns_name_list->tail = l->tail;
			}
		}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 216 "cfparse.y"
    {
			if (cf_ntp_list == NULL)
				cf_ntp_list = (yyvsp[(3) - (4)].list);
			else {
				cf_ntp_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_ntp_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 225 "cfparse.y"
    {
			if (cf_sip_list == NULL)
				cf_sip_list = (yyvsp[(3) - (4)].list);
			else {
				cf_sip_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_sip_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 234 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_sip_name_list == NULL) {
				cf_sip_name_list = l;
				cf_sip_name_list->tail = l;
				cf_sip_name_list->next = NULL;
			} else {
				cf_sip_name_list->tail->next = l;
				cf_sip_name_list->tail = l->tail;
			}
		}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 249 "cfparse.y"
    {
			if (cf_nis_list == NULL)
				cf_nis_list = (yyvsp[(3) - (4)].list);
			else {
				cf_nis_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_nis_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 258 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_nis_name_list == NULL) {
				cf_nis_name_list = l;
				cf_nis_name_list->tail = l;
				cf_nis_name_list->next = NULL;
			} else {
				cf_nis_name_list->tail->next = l;
				cf_nis_name_list->tail = l->tail;
			}
		}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 273 "cfparse.y"
    {
			if (cf_nisp_list == NULL)
				cf_nisp_list = (yyvsp[(3) - (4)].list);
			else {
				cf_nisp_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_nisp_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 282 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_nisp_name_list == NULL) {
				cf_nisp_name_list = l;
				cf_nisp_name_list->tail = l;
				cf_nisp_name_list->next = NULL;
			} else {
				cf_nisp_name_list->tail->next = l;
				cf_nisp_name_list->tail = l->tail;
			}
		}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 297 "cfparse.y"
    {
			if (cf_bcmcs_list == NULL)
				cf_bcmcs_list = (yyvsp[(3) - (4)].list);
			else {
				cf_bcmcs_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_bcmcs_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 306 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_bcmcs_name_list == NULL) {
				cf_bcmcs_name_list = l;
				cf_bcmcs_name_list->tail = l;
				cf_bcmcs_name_list->next = NULL;
			} else {
				cf_bcmcs_name_list->tail->next = l;
				cf_bcmcs_name_list->tail = l->tail;
			}
		}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 321 "cfparse.y"
    {
			if (cf_refreshtime == -1) {
				cf_refreshtime = (yyvsp[(3) - (4)].num);
				if (cf_refreshtime < -1 ||
				    cf_refreshtime > 0xffffffff) {
					/*
					 * refresh time should not be negative
					 * according to the lex definition,
					 * but check it for safety.
					 */
					yyerror("refresh time is out of range");
				}
				if (cf_refreshtime < DHCP6_IRT_MINIMUM) {
					/*
					 * the value MUST NOT be smaller than
					 * IRT_MINIMUM.
					 */
					yyerror("refresh time is too small "
					    "(must not be smaller than %d)",
					    DHCP6_IRT_MINIMUM);
				}
			} else {
				yywarn("multiple refresh times (ignored)");
			}
		}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 350 "cfparse.y"
    {
			struct cf_namelist *iapd;

			MAKE_NAMELIST(iapd, (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 359 "cfparse.y"
    {
			struct cf_namelist *iapd;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iapd, zero, (yyvsp[(4) - (6)].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 373 "cfparse.y"
    {
			struct cf_namelist *iana;

			MAKE_NAMELIST(iana, (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 382 "cfparse.y"
    {
			struct cf_namelist *iana;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iana, zero, (yyvsp[(4) - (6)].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 399 "cfparse.y"
    {
		struct cf_namelist *authinfo;

		MAKE_NAMELIST(authinfo, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(authinfo, &authinfolist_head))
			return (-1);
	}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 411 "cfparse.y"
    {
		struct cf_namelist *key;

		MAKE_NAMELIST(key, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(key, &keylist_head))
			return (-1);
	}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 423 "cfparse.y"
    {
		if (cfswitch_buffer((yyvsp[(2) - (3)].str))) {
			free((yyvsp[(2) - (3)].str));
			return (-1);
		}
		free((yyvsp[(2) - (3)].str));
	}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 434 "cfparse.y"
    {
		struct cf_namelist *pool;

		MAKE_NAMELIST(pool, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(pool, &addrpoollist_head))
			return (-1);
	}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 445 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 447 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 465 "cfparse.y"
    {
		struct cf_list *l;
		struct in6_addr a0, *a;

		if (inet_pton(AF_INET6, (yyvsp[(1) - (1)].str), &a0) != 1) {
			yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (1)].str));
			free((yyvsp[(1) - (1)].str));
			return (-1);
		}
		if ((a = malloc(sizeof(*a))) == NULL) {
			yywarn("can't allocate memory");
			return (-1);
		}
		*a = a0;

		MAKE_CFLIST(l, CFLISTENT_GENERIC, a, NULL);

		(yyval.list) = l;
	}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 487 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 489 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 507 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SEND, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 515 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_REQUEST, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 523 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_INFO_ONLY, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 531 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ALLOW, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 539 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DUID, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 547 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESS, (yyvsp[(2) - (3)].prefix),NULL);

			(yyval.list) = l;
		}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 555 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFIX, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 563 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFERENCE, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);

			(yyval.list) = l;
		}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 572 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SCRIPT, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 580 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DELAYEDKEY, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 588 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_RANGE, (yyvsp[(2) - (3)].range), NULL);

			(yyval.list) = l;
		}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 596 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESSPOOL, (yyvsp[(2) - (3)].pool), NULL);

			(yyval.list) = l;
		}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 607 "cfparse.y"
    {
			(yyval.list) = (yyvsp[(1) - (1)].list);
		}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 611 "cfparse.y"
    {
			(yyvsp[(1) - (3)].list)->next = (yyvsp[(3) - (3)].list);
			(yyvsp[(1) - (3)].list)->tail = (yyvsp[(3) - (3)].list)->tail;

			(yyval.list) = (yyvsp[(1) - (3)].list);
		}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 621 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_RAPID_COMMIT, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 629 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_AUTHINFO, NULL, NULL);
			l->ptr = (yyvsp[(2) - (2)].str);
			(yyval.list) = l;
		}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 637 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_PD, NULL, NULL);
			l->num = (yyvsp[(2) - (2)].num);
			(yyval.list) = l;
		}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 645 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_NA, NULL, NULL);
			l->num = (yyvsp[(2) - (2)].num);
			(yyval.list) = l;
		}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 653 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 661 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 669 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 677 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 685 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NTP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 693 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_REFRESHTIME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 701 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NIS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 709 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 717 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 725 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 733 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 741 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 749 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_AFTR, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 760 "cfparse.y"
    {
			struct dhcp6_range range0, *range;		

			memset(&range0, 0, sizeof(range0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (3)].str), &range0.min) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				free((yyvsp[(3) - (3)].str));
				return (-1);
			}
			if (inet_pton(AF_INET6, (yyvsp[(3) - (3)].str), &range0.max) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(3) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				free((yyvsp[(3) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));
			free((yyvsp[(3) - (3)].str));

			if ((range = malloc(sizeof(*range))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*range = range0;

			(yyval.range) = range;
		}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 791 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (2)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (2)].str));
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			free((yyvsp[(1) - (2)].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX this field is ignored */
			if ((yyvsp[(2) - (2)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(2) - (2)].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 818 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (3)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX */
			if ((yyvsp[(2) - (3)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(2) - (3)].num);
			if ((yyvsp[(3) - (3)].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[(3) - (3)].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 851 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (4)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (4)].str));
				free((yyvsp[(1) - (4)].str));
				return (-1);
			}
			free((yyvsp[(1) - (4)].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[(3) - (4)].num);
			if ((yyvsp[(4) - (4)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(4) - (4)].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 878 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (5)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (5)].str));
				free((yyvsp[(1) - (5)].str));
				return (-1);
			}
			free((yyvsp[(1) - (5)].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[(3) - (5)].num);
			if ((yyvsp[(4) - (5)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(4) - (5)].num);
			if ((yyvsp[(5) - (5)].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[(5) - (5)].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 911 "cfparse.y"
    {
			struct dhcp6_poolspec* pool;		

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[(1) - (2)].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			free((yyvsp[(1) - (2)].str));

			/* validate other parameters later */
			if ((yyvsp[(2) - (2)].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[(2) - (2)].num);
			pool->vltime = pool->pltime;

			(yyval.pool) = pool;
		}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 936 "cfparse.y"
    {
			struct dhcp6_poolspec* pool;		

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[(1) - (3)].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));

			/* validate other parameters later */
			if ((yyvsp[(2) - (3)].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[(2) - (3)].num);
			if ((yyvsp[(3) - (3)].num) < 0)
				pool->vltime = DHCP6_DURATION_INFINITE;
			else
				pool->vltime = (u_int32_t)(yyvsp[(3) - (3)].num);

			(yyval.pool) = pool;
		}
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 967 "cfparse.y"
    {
			(yyval.num) = -1;
		}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 971 "cfparse.y"
    {
			(yyval.num) = (yyvsp[(1) - (1)].num);
		}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 977 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 979 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 996 "cfparse.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 998 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_PREFIX, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 1009 "cfparse.y"
    {
		struct cf_list *ifl;

		MAKE_CFLIST(ifl, IACONF_PIF, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));
		(yyval.list) = ifl;
	}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 1018 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 1020 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 1038 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_ID, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 1046 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_LEN, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 1056 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 1058 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 1076 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_ADDR, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 1086 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 1088 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 1106 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_PROTO, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 1114 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_ALG, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 1122 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_RDM, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 1130 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_KEY, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 1140 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHPROTO_DELAYED; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 1141 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHPROTO_RECONFIG; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 1145 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHALG_HMACMD5; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 1149 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHRDM_MONOCOUNTER; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 1153 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 1155 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 1173 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_REALM, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 1181 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_KEYID, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 1189 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_SECRET, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 1197 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_EXPIRE, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;


/* Line 1792 of yacc.c  */
#line 3098 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



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
		      yytoken, &yylval);
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
      if (!yypact_value_is_default (yyn))
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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


/* Line 2055 of yacc.c  */
#line 1206 "cfparse.y"

/* supplement routines for configuration */
static int
add_namelist(new, headp)
	struct cf_namelist *new, **headp;
{
	struct cf_namelist *n;
	
	/* check for duplicated configuration */
	for (n = *headp; n; n = n->next) {
		if (strcmp(n->name, new->name) == 0) {
			yywarn("duplicated name: %s (ignored)",
			       new->name);
			cleanup_namelist(new);
			return (0);
		}
	}

	new->next = *headp;
	*headp = new;

	return (0);
}

/* free temporary resources */
static void
cleanup()
{
	cleanup_namelist(iflist_head);
	iflist_head = NULL;
	cleanup_namelist(hostlist_head);
	hostlist_head = NULL;
	cleanup_namelist(iapdlist_head);
	iapdlist_head = NULL;
	cleanup_namelist(ianalist_head);
	ianalist_head = NULL;
	cleanup_namelist(authinfolist_head);
	authinfolist_head = NULL;
	cleanup_namelist(keylist_head);
	keylist_head = NULL;
	cleanup_namelist(addrpoollist_head);
	addrpoollist_head = NULL;

	cleanup_cflist(cf_sip_list);
	cf_sip_list = NULL;
	cleanup_cflist(cf_sip_name_list);
	cf_sip_name_list = NULL;
	cleanup_cflist(cf_dns_list);
	cf_dns_list = NULL;
	cleanup_cflist(cf_dns_name_list);
	cf_dns_name_list = NULL;
	cleanup_cflist(cf_ntp_list);
	cf_ntp_list = NULL;
	cleanup_cflist(cf_nis_list);
	cf_nis_list = NULL;
	cleanup_cflist(cf_nis_name_list);
	cf_nis_name_list = NULL;
	cleanup_cflist(cf_nisp_list);
	cf_nisp_list = NULL;
	cleanup_cflist(cf_nisp_name_list);
	cf_nisp_name_list = NULL;
	cleanup_cflist(cf_bcmcs_list);
	cf_bcmcs_list = NULL;
	cleanup_cflist(cf_bcmcs_name_list);
	cf_bcmcs_name_list = NULL;
}

static void
cleanup_namelist(head)
	struct cf_namelist *head;
{
	struct cf_namelist *ifp, *ifp_next;

	for (ifp = head; ifp; ifp = ifp_next) {
		ifp_next = ifp->next;
		cleanup_cflist(ifp->params);
		free(ifp->name);
		free(ifp);
	}
}

static void
cleanup_cflist(p)
	struct cf_list *p;
{
	struct cf_list *n;

	if (p == NULL)
		return;

	n = p->next;
	if (p->type == DECL_ADDRESSPOOL) {
		free(((struct dhcp6_poolspec *)p->ptr)->name);
	}
	if (p->ptr)
		free(p->ptr);
	if (p->list)
		cleanup_cflist(p->list);
	free(p);

	cleanup_cflist(n);
}

#define config_fail() \
	do { cleanup(); configure_cleanup(); return (-1); } while(0)

int
cf_post_config()
{
	if (configure_keys(keylist_head))
		config_fail();

	if (configure_authinfo(authinfolist_head))
		config_fail();

	if (configure_ia(iapdlist_head, IATYPE_PD))
		config_fail();

	if (configure_ia(ianalist_head, IATYPE_NA))
		config_fail();

	if (configure_pool(addrpoollist_head))
		config_fail();

	if (configure_interface(iflist_head))
		config_fail();

	if (configure_host(hostlist_head))
		config_fail();

	if (configure_global_option())
		config_fail();

	configure_commit();
	cleanup();
	return (0);
}
#undef config_fail

void
cf_init()
{
	iflist_head = NULL;
}
