/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     TIDENTIFIER = 258,
     TINTEGER = 259,
     TDOUBLE = 260,
     TYINT = 261,
     TYDOUBLE = 262,
     TYFLOAT = 263,
     TYCHAR = 264,
     TYBOOL = 265,
     TYVOID = 266,
     TYSTRING = 267,
     TEXTERN = 268,
     TLITERAL = 269,
     TCEQ = 270,
     TCNE = 271,
     TCLT = 272,
     TCLE = 273,
     TCGT = 274,
     TCGE = 275,
     TEQUAL = 276,
     TLPAREN = 277,
     TRPAREN = 278,
     TLBRACE = 279,
     TRBRACE = 280,
     TCOMMA = 281,
     TDOT = 282,
     TSEMICOLON = 283,
     TLBRACKET = 284,
     TRBRACKET = 285,
     TQUOTATION = 286,
     TPLUS = 287,
     TMINUS = 288,
     TMUL = 289,
     TDIV = 290,
     TAND = 291,
     TOR = 292,
     TXOR = 293,
     TMOD = 294,
     TNEG = 295,
     TNOT = 296,
     TSHIFTL = 297,
     TSHIFTR = 298,
     TIF = 299,
     TELSE = 300,
     TFOR = 301,
     TWHILE = 302,
     TRETURN = 303,
     TSTRUCT = 304
   };
#endif
/* Tokens.  */
#define TIDENTIFIER 258
#define TINTEGER 259
#define TDOUBLE 260
#define TYINT 261
#define TYDOUBLE 262
#define TYFLOAT 263
#define TYCHAR 264
#define TYBOOL 265
#define TYVOID 266
#define TYSTRING 267
#define TEXTERN 268
#define TLITERAL 269
#define TCEQ 270
#define TCNE 271
#define TCLT 272
#define TCLE 273
#define TCGT 274
#define TCGE 275
#define TEQUAL 276
#define TLPAREN 277
#define TRPAREN 278
#define TLBRACE 279
#define TRBRACE 280
#define TCOMMA 281
#define TDOT 282
#define TSEMICOLON 283
#define TLBRACKET 284
#define TRBRACKET 285
#define TQUOTATION 286
#define TPLUS 287
#define TMINUS 288
#define TMUL 289
#define TDIV 290
#define TAND 291
#define TOR 292
#define TXOR 293
#define TMOD 294
#define TNEG 295
#define TNOT 296
#define TSHIFTL 297
#define TSHIFTR 298
#define TIF 299
#define TELSE 300
#define TFOR 301
#define TWHILE 302
#define TRETURN 303
#define TSTRUCT 304




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 12 "grammar.y"
{
	NBlock* block;
	NExpression* expr;
	NStatement* stmt;
	NIdentifier* ident;
	NVariableDeclaration* var_decl;
	NArrayIndex* index;
	std::vector<shared_ptr<NVariableDeclaration>>* varvec;
	std::vector<shared_ptr<NExpression>>* exprvec;
	std::string* string;
	int token;
	double test;
}
/* Line 1529 of yacc.c.  */
#line 161 "grammar.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

