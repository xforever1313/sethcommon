'''
These compile flags are for MSVC compilers
'''

import sys

from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *

from GlobalCompilerGlobals import *

class MSVCCompilerGlobals(GlobalCompilerGlobals):

    def __init__(self):
        #CppDefines
        self.globalDefines += ["WIN32", "_LIB", "_UNICODE", "UNICODE", '_USE_MATH_DEFINES']

        self.globalDebugDefines += ["_DEBUG"]
        self.globalReleaseDefines += []
        self.globalUnitTestDefines += []

        #Compile Flags
        self.includePaths = ['C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include', \
                             'C:\Program Files (x86)\Windows Kits\8.1\Include\um', \
                             'C:\Program Files (x86)\Windows Kits\8.1\Include\shared']

        self.globalCCFlags += ['/EHsc', '/WX', '/W3', '/nologo', '/FS']
        self.globalCXXFlags += []

        self.globalCCDebugFlags += ['/GS', '/Zc:wchar_t', '/ZI', '/Od', '/sdl', \
                                    '/fp:precise', '/RTC1', '/Gd', '/Oy', '/MDd']

        self.globalCCReleaseFlags += []
        self.globalCCUnitTestFlags += [] + self.globalCCDebugFlags

        #AR Flags
        self.globalARFlags = ['/NOLOGO']

        #Linker Flags
        self.globalLinkerFlags += ['/NXCOMPAT', '/DYNAMICBASE', '/INCREMENTAL', \
                                   '/MACHINE:X86', '/SUBSYSTEM:CONSOLE', \
                                   '/NOLOGO']

        self.globalDebugLinkerFlags += ['/DEBUG']
        self.globalReleaseLinkerFlags += []
        self.globalUnitTestLinkerFlags += []

        #Libs
        self.globalLibs += ["kernel32.lib", "user32.lib", "gdi32.lib", "winspool.lib", \
                            "comdlg32.lib", "advapi32.lib", "shell32.lib", "ole32.lib", \
                            "oleaut32.lib", "uuid.lib", "odbc32.lib", "odbccp32.lib"]

        self.globalDebugLibs += []
        self.globalReleaseLibs += []
        self.globalUnitTestLibs += []

        #LibPath
        self.globalLibPaths = ['C:\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\winv6.3\\um\\x86']

    def getBaseEnvironment(self, armBuild, serverBuild):
        #Ensure this is the right path for your system:
        compilerPath = '"C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin"'
        env = Environment(
            CC = os.path.join(compilerPath, "cl.exe"),
            CXX = os.path.join(compilerPath, "cl.exe"),
            LINK = os.path.join(compilerPath, "link.exe"),
            AR = os.path.join(compilerPath, "lib.exe"),
            SERVER_BUILD = False,
            ARM_BUILD = False,
            CLANG_BUILD = False,
            ASM_JS_BUILD = False,
            MSVC_BUILD = True,
            SYSTEM = "MSVC"
        )
        self.globalDefines += ['MSVC']
            
        return env

    def extendDebugEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalDebugDefines)
        envBase.Append(CPPPATH = self.includePaths)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCDebugFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalDebugLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalDebugLibs)
        envBase.Append(LIBPATH = libPath + self.globalLibPaths)
        envBase.Append(ARFLAGS = self.globalARFlags)

    def extendReleaseEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalReleaseDefines)
        envBase.Append(CPPPATH = self.includePaths)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCReleaseFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalReleaseLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalReleaseLibs)
        envBase.Append(LIBPATH = libPath + self.globalLibPaths)
        envBase.Append(ARFLAGS = self.globalARFlags)

    def extendUnitTestEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalUnitTestDefines)
        envBase.Append(CPPPATH = self.includePaths)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCUnitTestFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalUnitTestLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalUnitTestLibs)
        envBase.Append(LIBPATH = libPath + self.globalLibPaths)
        envBase.Append(ARFLAGS = self.globalARFlags)

