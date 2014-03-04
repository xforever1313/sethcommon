'''
These flags are the same for gnu compilers, including gcc, mingw, clang, and emscripten.
For compiler specific flags, see their specific .py files
'''
import sys

from GlobalCompilerGlobals import *

class GnuCompilerGlobals(GlobalCompilerGlobals): 

    def __init__(self):
        #Compile Flags
        self.globalCCFlags += ["-pedantic-errors", "-Werror", "-Wall", "-Wcast-align", "-Wsign-compare", \
                  "-Wempty-body", "-Wcast-qual", "-Wmissing-field-initializers", "-Wtype-limits", \
                  "-fstack-protector-all", "-pthread"]

        self.globalCXXFlags += ['-std=gnu++11']

        self.globalCCDebugFlags += ["-g", "-Wswitch"]
        self.globalCCReleaseFlags += ["-O3", "-Wswitch", "-fdata-sections", "-ffunction-sections"]
        self.globalCCUnitTestFlags += ["-g"]

        self.globalCXXFlags += ['-Wold-style-cast']

        self.globalCXXDebugFlags += ['-fno-rtti']
        self.globalCXXReleaseFlags += ['-fno-rtti']
        self.globalCXXUnitTestFlags += []

        #Link Flags
        self.globalLinkerFlags += ["-Wall", "-Werror", "-std=gnu++11"]
        if(sys.platform == "win32"):
            self.globalLinkerFlags += ["-static", "-pthread"]
        elif (sys.platform == "darwin"):
            self.globalLinkerFlags += []
        else:
            self.globalLinkerFlags += ["-pthread"]


        self.globalDebugLinkerFlags += ['-g']

        self.globalReleaseLinkerFlags += []
        if (sys.platform == "darwin"):
            self.globalReleaseLinkerFlags += ["-Wl,-dead_strip"]
        else:
            self.globalReleaseLinkerFlags += ['-s', '-Wl,--gc-sections', '-Wl,--strip-all']

        self.globalUnitTestLinkerFlags += []

        #Libs
        self.globalLibs += []

        self.globalDebugLibs += []
        self.globalReleaseLibs += []
        self.globalUnitTestLibs += []

