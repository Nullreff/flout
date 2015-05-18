/* data.h - Data structures
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

#include "data.h"
#include "repl.h"

Value value_empty(void)
{
    return (Value){TYPE_EMPTY, {}};
}

Value value_scope(void)
{
    return (Value){TYPE_SCOPE, {}};
}

Value value_integer(int value)
{
    return (Value){TYPE_INTEGER, {.integer = value}};
}

Value value_string(char* value)
{
    return (Value){TYPE_STRING, {.string = value}};
}

void value_print(Value value)
{
    switch (value.type)
    {
        case TYPE_EMPTY:
            repl_print("()\n");
            break;

        case TYPE_SCOPE:
            repl_print("(@)\n");
            break;

        case TYPE_INTEGER:
            repl_print("(%d)\n", value.data.integer);
            break;

        case TYPE_STRING:
            repl_print("(%s)\n", value.data.string);
            break;

        default: ERROR("Unknown type\n");
    }
}

bool value_equals(Value a, Value b)
{
    if (a.type != b.type)
        return false;

    switch (a.type)
    {
        case TYPE_EMPTY:   return true;
        case TYPE_SCOPE:   return true;
        case TYPE_INTEGER: return a.data.integer == b.data.integer;
        case TYPE_STRING:  return strcmp(a.data.string, b.data.string);
        default: ERROR("Unknown type\n");
    }
}

