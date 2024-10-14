/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PYTHON_TAB_H_INCLUDED
# define YY_YY_PYTHON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    FOR = 258,                     /* FOR  */
    WHILE = 259,                   /* WHILE  */
    IN = 260,                      /* IN  */
    IF = 261,                      /* IF  */
    ELSE = 262,                    /* ELSE  */
    ELIF = 263,                    /* ELIF  */
    TRY = 264,                     /* TRY  */
    EXCEPT = 265,                  /* EXCEPT  */
    FINALLY = 266,                 /* FINALLY  */
    ID = 267,                      /* ID  */
    NEWLINE = 268,                 /* NEWLINE  */
    INDENT = 269,                  /* INDENT  */
    DEDENT = 270,                  /* DEDENT  */
    INT = 271,                     /* INT  */
    FLOAT = 272,                   /* FLOAT  */
    COMPLEX = 273,                 /* COMPLEX  */
    TRUE = 274,                    /* TRUE  */
    FALSE = 275,                   /* FALSE  */
    STR = 276,                     /* STR  */
    NONE = 277,                    /* NONE  */
    PLUS_ASSIGN = 278,             /* PLUS_ASSIGN  */
    MINUS_ASSIGN = 279,            /* MINUS_ASSIGN  */
    MULT_ASSIGN = 280,             /* MULT_ASSIGN  */
    DEGREE_ASSIGN = 281,           /* DEGREE_ASSIGN  */
    DIV_ASSIGN = 282,              /* DIV_ASSIGN  */
    MOD_ASSIGN = 283,              /* MOD_ASSIGN  */
    AND = 284,                     /* AND  */
    OR = 285,                      /* OR  */
    NOT = 286,                     /* NOT  */
    FLOOR_DIV = 287,               /* FLOOR_DIV  */
    UPLUS = 288,                   /* UPLUS  */
    UMINUS = 289,                  /* UMINUS  */
    DEGREE = 290,                  /* DEGREE  */
    LESSER_EQUAL = 291,            /* LESSER_EQUAL  */
    GREATER_EQUAL = 292,           /* GREATER_EQUAL  */
    NOT_EQUAL = 293,               /* NOT_EQUAL  */
    EQUAL = 294                    /* EQUAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PYTHON_TAB_H_INCLUDED  */
