'''
These compile flags are for GCC or MinGW.  These are ADDITIONAL flags in addition to
flags defined in GnuCompilerGlobals
'''
import sys

from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *

from GnuCompilerGlobals import *

class GCCCompilerGlobals(GnuCompilerGlobals):

    def __init__(self):
        GnuCompilerGlobals.__init__(self)
        #CppDefines
        self.globalDefines += []

        self.globalDebugDefines += []
        self.globalReleaseDefines += []
        self.globalUnitTestDefines += []

        #Compile Flags
        self.globalCCFlags += ['-Wclobbered', '-Wdouble-promotion']

        self.globalCCDebugFlags += []
        self.globalCCReleaseFlags += []
        self.globalCCUnitTestFlags += ['-fprofile-arcs', '-ftest-coverage']

        self.globalCXXFlags += []

        self.globalCXXDebugFlags += []
        self.globalCXXReleaseFlags += []
        self.globalCXXUnitTestFlags += []

        #Linker Flags
        self.globalLinkerFlags += []

        self.globalDebugLinkerFlags += []
        self.globalReleaseLinkerFlags += []
        self.globalUnitTestLinkerFlags += []

        #Libs
        self.globalLibs += []

        self.globalDebugLibs += []
        self.globalReleaseLibs += []
        self.globalUnitTestLibs += ['gcov']

        if (sys.platform == "win32"):
            self.globalDebugLibs += ["ssp"] #Lib SSP is still needed in mingw, but not linux
            self.globalReleaseLibs += ["ssp"]
            self.globalUnitTestLibs += ["ssp"]

        #debug_new must come last
        self.globalDebugLibs += ['debug_new']
        self.globalReleaseLibs += []
        self.globalUnitTestLibs += ['debug_new']

    def getBaseEnvironment(self, armBuild, serverBuild):
        if (sys.platform == "win32"):
            print "Building for mingw32"
            env = Environment(            
                tools = ["mingw"],
                SERVER_BUILD = False,
                ARM_BUILD = False,
                CLANG_BUILD = False,
                ASM_JS_BUILD = False,
                MSVC_BUILD = False,
                SYSTEM = "mingw32"
            )

            self.globalDefines += ['MINGW']

        elif (serverBuild):
            print "Building for gcc x86 on Skyvo's Sever"
            env = Environment(
                CC = "gcc-4.8",
                CXX = "g++-4.8",
                LINK = "g++-4.8",
                SERVER_BUILD = True,
                ARM_BUILD = False,
                CLANG_BUILD = False,
                ASM_JS_BUILD = False,
                MSVC_BUILD = False,
                SYSTEM = "gccx86"
            )
            self.globalDefines += ['GCC']

        else:
            print "Building for gcc x86"
            env = Environment(
                tools = ["default", "gcc", "g++"],
                SERVER_BUILD = False,
                ARM_BUILD = False,
                CLANG_BUILD = False,
                ASM_JS_BUILD = False,
                MSVC_BUILD = False,
                SYSTEM = "gccx86"
            )
            self.globalDefines += ['GCC']
        
        return env

    def extendDebugEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalDebugDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags + self.globalCXXDebugFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCDebugFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalDebugLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalDebugLibs)
        envBase.Append(LIBPATH = libPath)

    def extendReleaseEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalReleaseDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags + self.globalCXXReleaseFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCReleaseFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalReleaseLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalReleaseLibs)
        envBase.Append(LIBPATH = libPath)

    def extendUnitTestEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalUnitTestDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags + self.globalCXXUnitTestFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCUnitTestFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalUnitTestLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalUnitTestLibs)
        envBase.Append(LIBPATH = libPath)

