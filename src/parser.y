/* parser.y - Command line instruction parser
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

%error-verbose
%{
#include <ctype.h>
#include "parser.h"

#define PARSE_ERROR(...) repl_print_error(__VA_ARGS__); YYABORT;
#define PARSE_ERROR_IF(CONDITION, ...) if (CONDITION) { repl_print_error(__VA_ARGS__); YYABORT; }
#define PARSE_ERROR_FREE(VAR, ...) repl_print_error(__VA_ARGS__); free(VAR); YYABORT;
#define yyerror command_error

// See lexer.l
int yylex(void);
void command_error(const char* message)
{
    repl_print_error("%s\n", message);
}
%}

%code requires {
    #include "repl.h"
    #include "data.h"
}

%union {
    int integer;
    char* string;
    char* token;
    Value value;
}

/* Symbols */
%token LINE_BREAK "line break"
%token COMMENT "comment"
%token COMPARE_EQUAL "=="
%token COMPARE_NOT_EQUAL "!="
%token OPEN_BRACE "{"
%token CLOSE_BRACE "}"
%token PIPE "|"

/* Data */
%token <integer> INTEGER "integer"
%token <string> STRING "string"
%token <string> TOKEN "token"
%type <value> value "value"
%type <value> expression "expression"
%type <value> block "block"
%type <value> statement "statement"

%start input

%%
input: /* empty */
     | input line
;

line: LINE_BREAK
    | statement LINE_BREAK         { value_print($1); }
    | COMMENT LINE_BREAK
    | statement COMMENT LINE_BREAK { value_print($1); }
;

statement: expression            { $$ = $1; }
         | statement PIPE block  { $$ = $1.type != TYPE_EMPTY ? $3 : value_empty(); }
;

block: OPEN_BRACE statement CLOSE_BRACE  { $$ = $2; }
;

expression: value                         { $$ = $1; }
          | value COMPARE_EQUAL value     { $$ = value_equals($1, $3) ? value_scope() : value_empty(); }
          | value COMPARE_NOT_EQUAL value { $$ = value_equals($1, $3) ? value_empty() : value_scope(); }
;

value: INTEGER  { $$ = value_integer($1); }
     | STRING   { $$ = value_string($1); }
;
%%

