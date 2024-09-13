/*
 * image-duplicate-finder - Command-line tool for finding image duplicates
 * Copyright (C) 2024 Marcin Owsiany <marcin@owsiany.pl>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <map>
#include <memory>
#include <vector>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pic.h"
#include "similar.h"


static gchar **program = NULL;
static gdouble threshold = 99.0;
static gboolean verbose = false;

static GOptionEntry entries[] =
{
    {"program", 'p', G_OPTION_FLAG_NONE, G_OPTION_ARG_STRING_ARRAY, &program, "Program to run with each group of similar images.", "PROGRAM"},
    {"threshold", 't', G_OPTION_FLAG_NONE, G_OPTION_ARG_DOUBLE, &threshold, "Images more similar than this value are considered duplicates.", "VALUE"},
    {"verbose", 'v', G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE, &verbose, "Verbose mode.", NULL},
    NULL
};

int main(int argc, char const *argv[])
{
    GError *error = NULL;

    GOptionContext *context = g_option_context_new("- find duplicate images");
    g_option_context_add_main_entries(context, entries, NULL);
    if (!g_option_context_parse(context, &argc, const_cast<char ***>(&argv), &error)) {
      g_print("option parsing failed: %s\n", error->message);
      exit(1);
    }
    g_option_context_free(context);

    std::map<std::string, std::unique_ptr<pic>> pics;
    for (size_t i = 1; i < argc; i++)
    {
        std::string name(argv[i]);
        pics[name] = std::unique_ptr<pic>(new pic(argv[i]));
    }
    if (verbose)
        fprintf(stderr, "there are %d pics\n", pics.size());

    for (auto a = pics.begin(); a != pics.end(); ++a) {
        auto b = a;
        b++;
        for (; b != pics.end(); ++b) {
            double similarity = a->second->compare(*b->second);
            if (verbose)
                fprintf(stderr, "%s vs %s: %f\n",
                    a->second->name.c_str(),
                    b->second->name.c_str(),
                    similarity);
            if (similarity < threshold)
                continue;
            a->second->equivalent.insert(b->second->equivalent.begin(), b->second->equivalent.end());
            for (auto const &f: a->second->equivalent) {
                pics[f]->equivalent.insert(a->second->equivalent.begin(), a->second->equivalent.end());
            }
        }
    }
    if (verbose)
        fprintf(stderr, "\n");

    std::set<std::string> printed;
    for (auto const &f: pics) {
        if (f.second->equivalent.size() < 2)
            // skip this pic if not similar to any other one
            continue;
        if (printed.find(f.second->name) != printed.end())
            // skip this pic if it was already printed (when processing a similar image)
            continue;
        std::vector<const char *> cmd;
        if (!program || !*program) {
            cmd.push_back("echo");
        } else {
            for (size_t i = 0; program[i]; i++) {
                cmd.push_back(program[i]);
            }
        }
        
        for (auto const &e: f.second->equivalent) {
            cmd.push_back(e.c_str());
            printed.insert(e);
        }
        cmd.push_back(NULL);
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            execvp(const_cast<char *>(cmd[0]), const_cast<char **>(&(cmd[0])));
            perror("execv");
            exit(1);
        } else {
            int status;
            wait(&status);
            if (!WIFEXITED(status) || WEXITSTATUS(status)!=0) {
                fprintf(stderr, "subprocess failed, aborting\n");
                exit(1);
            }
        }
    }

    return 0;
}

