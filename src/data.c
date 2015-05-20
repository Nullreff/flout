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
    return (Value){TYPE_EMPTY, 0, {0}};
}

Value value_scope(void)
{
    return (Value){TYPE_SCOPE, 0, {0}};
}

Value value_integer(int value)
{
    return (Value){TYPE_INTEGER, 1, {.integer = value}};
}

Value value_string(char* value)
{
    return (Value){TYPE_STRING, 1, {.string = value}};
}

Value value_list(ValueList* list)
{
    if (list == NULL)
        return value_empty();

    unsigned int count = list->count;
    Value* data = malloc(sizeof(Value) * count);
    CHECK_OOM(data);
    for (unsigned int i = 0; list != NULL; list = list->next)
        data[i++] = list->value;
    return (Value){TYPE_LIST, count, {.list = data}};
}

static void value_print_helper(Value value)
{
    switch (value.type)
    {
        case TYPE_EMPTY:
            repl_print("()");
            break;

        case TYPE_SCOPE:
            repl_print("(@)");
            break;

        case TYPE_INTEGER:
            repl_print("%d", value.data.integer);
            break;

        case TYPE_STRING:
            repl_print("%s", value.data.string);
            break;

        case TYPE_LIST:
            if (value.count == 0)
                repl_print("()");
            else
            {
                repl_print("( ");
                for (unsigned int i = 0; i < value.count; i++)
                {
                    value_print_helper(value.data.list[i]);
                    repl_print(" ");
                }
                repl_print(")");
            }
            break;

        default:
            ERROR("Unknown type\n");
    }
}

void value_print(Value value)
{
    value_print_helper(value);
    repl_print("\n");
}

bool value_equals(Value a, Value b)
{
    if (a.type != b.type)
        return false;

    switch (a.type)
    {
        case TYPE_EMPTY:
           return true;

        case TYPE_SCOPE:
           return true;

        case TYPE_INTEGER:
            return a.data.integer == b.data.integer;

        case TYPE_STRING:
            return strcmp(a.data.string, b.data.string) == 0;

        case TYPE_LIST:
            for (unsigned int i = 0; i < a.count; i++)
                if (!value_equals(a.data.list[i], b.data.list[i]))
                    return false;
            return true;

        default: ERROR("Unknown type\n");
    }
}

ValueList* valuelist_init(Value value, ValueList* next)
{
    ValueList* list = malloc(sizeof(ValueList));
    CHECK_OOM(list);
    list->next = next;
    list->value = value;
    list->count = next != NULL ? next->count + 1 : 1;
    return list;
}

