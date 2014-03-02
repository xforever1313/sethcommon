Seth Common

------------------

Seth Common (formally Skyvo Common) is a collection of libraries and scripts that I use for any C++ project.

This repo includes source code, binaries, and scripts the aid in development in ANY C++ project. Some features include, but a not limited to:

    Advanced build scripts.
        Has the ability to run unit tests, and then automatically run gcov (GCC only)
        Has the ability to auto-generate netbeans project
        Has the ability to run doxygen and CppCheck from the command line 
    Build targets for Windows, Linux, MacOS, iOS, and emscripten.
    Compiler support for GCC, MinGW, and clang. MSVC support is limited
    Cross-platform OS calls.
    Forks of other helpful C++ libraries, all in one convenient spot.
    Mathematical functions
    And more! 

Building:

In order to build, you will need to install Python27, SCons, and a C++11 compiler. GCC4.8 and Clang3.3 are both C++11 feature complete.
Third Party Libraries:

Seth Common not only includes libraries written by me, but also libraries written by other by other developers that aid in writing software. The following list shows the information of who wrote each project in the Common folder. For a what each of these projects do, see doc/ProjectDescriptions.txt
Source Code:

    common_classes:
        Written by me 

    CppUTest:
        Website: http://cpputest.github.io/
        This was NOT written by me. Although some modifications were made to be compatible with other libraries. See Common/CppUTest/Changes.txt 

    date_version:
        Written by me. 

    debug_new:
        Website: http://sourceforge.net/projects/nvwa/
        This was NOT written by me.
        See the Readme in the directory for more information. No code was modified, although an SConstruct was added to conform to the build system. 

    gmock:
        Website: http://code.google.com/p/gmock/
        This was NOT written by me. It was written by Google.
        No code was modified, although an SConstruct was added to conform to the build system, and files deemed unneeded were removed. 

    Math:
        Written by me. 

    OS:
        Written by me. 

    rapidjson:
        Website: http://code.google.com/p/rapidjson/
        This was NOT written by me.
        No code was modified, although files deemed unneeded were removed. 

    rapidxml:
        Website: http://rapidxml.sourceforge.net/
        This was NOT written by me.
        No code was modified. 

    SCSV:
        Written by me. 

Tools:

    sdedit:
        Website: http://sdedit.sourceforge.net/
        This was NOT written by me. 

    umlet:
        Website: http://umlet.com/
        This was NOT written by me 
 