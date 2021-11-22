#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cassert>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define __shell__(a)  assert (::system (a) != -1)

class mstring 
    : public std::string
{
public:
    using std::string::basic_string;

    operator const char * ()
    {
        return c_str ();
    }

};

enum class build_mode 
{
    debug,
    release
};

void print_help ();
void build (build_mode mode);
void clean ();

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
    mstring dir_name = mode == build_mode::debug ? "debug" : "release",
        cmake_build_type = dir_name;
    cmake_build_type[0] = ::toupper (cmake_build_type[0]);

    int dir = opendir (dir_name);
    if (!dir < 1)
    {
        dir = mkdir (dir_name, 666);  
    } 
    

}

void clean ()
{
    assert (::system ("rm -rf release && rm -rf debug") != -1);
}
