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

        self.globalCCDebugFlags += ['-O0']
        self.globalCCReleaseFlags += ['-O3']
        self.globalCCUnitTestFlags += ['-O0', '-fprofile-arcs', '-ftest-coverage']

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
            self.addMingwFlags()


    def addMingwFlags(self):
        self.globalDebugLibs += ["ssp"] #Lib SSP is still needed in mingw, but not linux
        self.globalReleaseLibs += ["ssp"]
        self.globalUnitTestLibs += ["ssp"]

        if (sys.platform == "win32"):
            self.globalDebugLibs += ["debug_new"] #debug_new must come last
            self.globalUnitTestLibs += ["debug_new"]

    def getBaseEnvironment(self, armBuild, serverBuild, mingwBuild):
        if (sys.platform == "win32"):
            print ("Building for mingw")
            env = Environment(            
                tools = ["mingw"],
                SERVER_BUILD = False,
                ARM_BUILD = False,
                CLANG_BUILD = False,
                ASM_JS_BUILD = False,
                MSVC_BUILD = False,
                MINGW_CROSS_BUILD = False,
                STATIC_LINK_FLAGS = ['-static'],
                DYNAMIC_LIBS = ['gcc_eh'],
                SYSTEM = "mingw"
            )

            self.globalDefines += ['MINGW']

        elif(mingwBuild):
            print ("Building for mingw on linux")
            env = Environment(
                tools = ['mingw'],
                CC = "x86_64-w64-mingw32-gcc",
                CXX = "x86_64-w64-mingw32-g++",
                LINK = "x86_64-w64-mingw32-g++",
                AR = "x86_64-w64-mingw32-ar",
                AS = "x86_64-w64-mingw32-as",
                RANLIB = "x86_64-w64-mingw32-ranlib",
                CPP = "x86_64-w64-mingw32-cpp",

                SERVER_BUILD = False,
                ARM_BUILD = False,
                CLANG_BUILD = False,
                ASM_JS_BUILD = False,
                MSVC_BUILD = False,
                MINGW_CROSS_BUILD = True,
                SYSTEM = "mingw",
                STATIC_LINK_FLAGS = ['-static'],
                DYNAMIC_LIBS = ['gcc_eh'],
                SHLIBSUFFIX= ".dll"
            )

            self.globalDefines += ["MINGW", "WIN32"]

            self.globalDefines.remove("LINUX")
            self.addMingwFlags()

        elif (serverBuild):
            print ("Building for gcc x86 on the build sever")
            env = Environment(
                CC = "gcc-4.8",
                CXX = "g++-4.8",
                LINK = "g++-4.8",
                SERVER_BUILD = True,
                ARM_BUILD = False,
                CLANG_BUILD = False,
                ASM_JS_BUILD = False,
                MSVC_BUILD = False,
                MINGW_CROSS_BUILD = False,
                STATIC_LINK_FLAGS = [],
                DYNAMIC_LIBS = [],
                SYSTEM = "gccx86"
            )
            self.globalDefines += ['GCC']

        else:
            print ("Building for gcc x86")
            env = Environment(
                tools = ["default", "gcc", "g++"],
                SERVER_BUILD = False,
                ARM_BUILD = False,
                CLANG_BUILD = False,
                ASM_JS_BUILD = False,
                MSVC_BUILD = False,
                MINGW_CROSS_BUILD = False,
                STATIC_LINK_FLAGS = [],
                DYNAMIC_LIBS = [],
                SYSTEM = "gccx86"
            )
            self.globalDefines += ['GCC']
        
        return env

    def addMapFlag(self, env, mapName):
        if (not os.path.exists(env['BINDIR'])):
            os.makedirs(env['BINDIR']) #This is here so during shared lib builds, the map file is still made
        env.Append(LINKFLAGS = ['-Wl,-Map,' + os.path.join(env['BINDIR'], mapName)])

    def addStaticDynamicLibFlags(self, env):
        if (env['SHARED_BUILD']):
            env.Append(LIBS = env['DYNAMIC_LIBS'])
        else:
            env.Append(LINKFLAGS = env['STATIC_LINK_FLAGS'])
        
    def extendDebugEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalDebugDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags + self.globalCXXDebugFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCDebugFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalDebugLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalDebugLibs)
        envBase.Append(LIBPATH = libPath)
        self.addMapFlag(envBase, envBase['PROJECT_NAME'] + '.map')
        self.addStaticDynamicLibFlags(envBase)

    def extendReleaseEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalReleaseDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags + self.globalCXXReleaseFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCReleaseFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalReleaseLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalReleaseLibs)
        envBase.Append(LIBPATH = libPath)
        self.addMapFlag(envBase, envBase['PROJECT_NAME'] + '.map')
        self.addStaticDynamicLibFlags(envBase)

    def extendUnitTestEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalUnitTestDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags + self.globalCXXUnitTestFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCUnitTestFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalUnitTestLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalUnitTestLibs)
        envBase.Append(LIBPATH = libPath)
        self.addMapFlag(envBase, 'unit_test.map')
        self.addStaticDynamicLibFlags(envBase)
