/* flout.c - Show off that data ya got
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

#include "flout.h"
#include "parser.h"
#include "repl.h"
#include <ctype.h>
#include <getopt.h>
#include <signal.h>

static void print_version()
{
    printf("Flout %s\n", FLOUT_VERSION);
}

static void print_help()
{
    printf("Flout - Show off that data ya got\n\n"
           "Usage: flout [options]\n"
           "Options:\n"
           "    -v, --version\n"
           "        Print the current version\n\n"
           "    -h, --help\n"
           "        Print this message\n");
}

static void parse_options(int argc, char* argv[])
{
    static struct option long_options[] =
    {
        {"version",     no_argument,       NULL, 'v'},
        {"help",        no_argument,       NULL, 'h'},
        {NULL,          0,                 NULL,  0 }
    };

    while (1)
    {
        int opt = getopt_long(argc, argv, "vh", long_options, NULL);
        switch (opt)
        {
            case -1:
                return;

            case 'v':
                print_version();
                exit(EXIT_SUCCESS);

            case 'h':
                print_help();
                exit(EXIT_SUCCESS);

            default:
                exit(EXIT_FAILURE);
        }
    }
}

static void signal_callback(int signal)
{
    if (signal == SIGINT)
    {
        flout_cleanup();
        exit(EXIT_SUCCESS);
    }
}

// Referenced from common.h
void flout_cleanup(void)
{
    repl_cleanup();
    printf("\n");
}

int main(int argc, char* argv[])
{
    signal(SIGINT, signal_callback);
    parse_options(argc, argv);

    repl_run();

    flout_cleanup();
    return EXIT_SUCCESS;
}

