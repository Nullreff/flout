/* lexer.l - Command line instruction lexer
 *
 * Copyright (C) 2014 Ryan Mendivil <ryan@nullreff.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Flout nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

%{
#include <stdio.h>
#include "parser.h"
#include "repl.h"
#define YY_INPUT(buff, res, buffsize) (res = read_input(buff, buffsize))
#define read_input(buff,buffsize) repl_read(buff,buffsize)
%}
%option noyywrap
%option nounput
%option noinput
%%
[ \t\0]           ;
\n                { return LINE_BREAK;  }
#.*$              { return COMMENT;     }
==                { return COMPARE_EQUAL; }
!=                { return COMPARE_NOT_EQUAL; }
&&                { return AND; }
\|\|              { return OR; }
\{                { return OPEN_BRACE; }
\}                { return CLOSE_BRACE; }
\(                { return OPEN_PAREN; }
\)                { return CLOSE_PAREN; }
\|                { return PIPE; }
\"(\\\"|[^"])+\"  { yylval.string  = strdup(yytext);           return STRING;  }
-?[0-9]+          { yylval.integer = strtol(yytext, NULL, 10); return INTEGER; }
[a-zA-Z0-9]+      { yylval.string  = strdup(yytext);           return TOKEN;   }
%%

