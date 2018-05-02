#include <functional>
#include <string.h>

#include <iostream>

#include "utils.hh"
#include "c_array.hh"
#include "req.hh"
#include "which_compiler.h"
#include "get_path.hh"

iz::c_array<char> get_path(const char* file_name, const char* file_dir)
{
        /* Require compiler to be known. */
        req(which_compiler());

        req(file_name != NULL, "Null file_name");
        req(file_dir  != NULL, "Null file_dir");

        iz::c_array<char> path;
        unsigned name_len, dir_len, path_index;

        name_len = strlen(file_name);
        dir_len  = strlen(file_dir);

        // GCC => file_dir + file_name.
        if (which_compiler() == GCC) {
                char* file_dir_slashed;

                // Make sure dir is preceded by '/'.
                if (file_dir[dir_len - 1] != '/') {
                        ++dir_len;

                        file_dir_slashed = new char[dir_len + 1];
                        strcpy(file_dir_slashed, file_dir);
                        strcat(file_dir_slashed, "/");

                } else {
                        file_dir_slashed = new char[dir_len + 1];
                        strcpy(file_dir_slashed, file_dir);
                }

                path_index = 0;
                path.realloc(dir_len + name_len + 1);

                path.map([&](char& elem) {
                                if (path_index >= dir_len)
                                        elem = file_name[path_index++ - dir_len];
                                else
                                        elem = file_dir_slashed[path_index++];
                        });
        } else {
                path_index = 0;
                path.realloc(name_len + 1);

                path.map([&](char& elem) {
                                elem = file_name[path_index++];
                        });
        }

        return path;
}
