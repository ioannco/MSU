#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cassert>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <exception>
#include <cstdarg>
#include <cstring>

#define __shell__(command)  assert (::bash (command) != -1)
#define __protect__(call)  { if ((call) == -1) std::cerr << __PRETTY_FUNCTION__ << #call <<  strerror (errno) << std::endl; }

enum class build_mode 
{
    debug,
    release
};

void print_help ();
void build (build_mode mode);
void clean ();
int execute (const std::string & filename, const std::vector <std::string> &args);
int bash (const std::string & command);

int main (int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Incorrect params. Use -h or --help to get help.\n";
        return 0;
    }
    
    std::string flag = argv[1];

    if (flag == "-a" || flag == "--all")
    {
        build (build_mode::debug);
        build (build_mode::release);
    }
    else if (flag == "-d" || flag == "--debug")
        build (build_mode::debug);
    else if (flag == "-r" || flag == "--release")
        build (build_mode::release);
    else if (flag == "-c" || flag == "--clean")
        clean();
    else
        print_help ();
    
    return 0;
}

void print_help ()
{
    std::cout << "Usage: \n-c || --clean   : Clean build folders \n-a || --all     : Build all \n-d || --debug   : Build debug only \n-r || --release : Build release only \n-h || --help    : Get help\n";
}

void build (build_mode mode)
{
    std::string dir_name = mode == build_mode::debug ? "debug" : "release",
        cmake_build_type = dir_name;
    cmake_build_type[0] = char (::toupper (cmake_build_type[0]));

    if (!::opendir (dir_name.c_str()))
    {
        __shell__ ("mkdir " + dir_name);
    }

    __shell__ ("cmake -D CMAKE_BUILD_TYPE=" + cmake_build_type  + " -B " + dir_name);
    __shell__ ("make --directory=" + dir_name);
}

void clean ()
{
    __shell__ ("rm -rf debug");
    __shell__ ("rm -rf release");
}

int execute (const std::string &filename, const std::vector <std::string> &args)
{
    auto pid = ::fork();
    assert (pid >= 0);

    if (!pid)
    {
        std::vector <char *> args_copy;
        for (auto && arg : args)
            args_copy.push_back (const_cast<char *> (arg.c_str()));
        args_copy.push_back (nullptr);

        __protect__ (execvp (filename.c_str (), const_cast<char * const *> (args_copy.data())));

        for (auto && arg : args_copy)
            std::cerr << arg << " ";
        std::cerr << std::endl;
        return 1;
    }
    else
        return ::wait (nullptr);

}

int bash (const std::string & command)
{
    std::vector <std::string> args;

    for (auto && i = 0u; i < command.size(); i++)
    {
        args.emplace_back ("");

        while (i < command.size() && command[i] != ' ' && command[i] != '\t' && command[i] != '\n')
            args[args.size() - 1] += command[i++];
    }

    return ::execute (args[0], args);
}
