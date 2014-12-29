'''
These compile flags are for MSVC compilers
'''
import os
import sys

from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *

from GlobalCompilerGlobals import *

###
# Change this if your path to the MSVC binaries is different!
###
msvcPath = '"C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin"'
x86Path = msvcPath
x64Path = os.path.join(msvcPath, 'x86_amd64')
armPath = os.path.join(msvcPath, 'x86_arm')

class MSVCCompilerGlobals(GlobalCompilerGlobals):

    def __init__(self, target):
        if (target == 'x86'):
            self.compilerPath = x86Path
        elif(target == 'x64'):
            self.compilerPath = x64Path
        elif (target == 'arm'):
            self.compilerPath = armPath
        else:
            raise Exception('Invalid msvc target: ' + target)
        self.target = target

        #Strip the quotes from msvcPath
        os.environ['PATH'] = msvcPath[1:-1] + ';' + os.environ['PATH']
        
        #CppDefines
        self.globalDefines += ["WIN32", "_LIB", "_UNICODE", "UNICODE", '_USE_MATH_DEFINES', '_MT', '_MBCS', '_CRT_SECURE_NO_WARNINGS']

        self.globalDebugDefines += ["_DEBUG"]
        self.globalReleaseDefines += []
        self.globalUnitTestDefines += []

        #Compile Flags
        self.includePaths = ['C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include', \
                             'C:\Program Files (x86)\Windows Kits\8.1\Include\um', \
                             'C:\Program Files (x86)\Windows Kits\8.1\Include\shared']

        self.globalCCFlags += ['/EHsc', '/WX', '/W3', '/nologo', '/FS', '/GS', '/sdl', '/fp:precise'] + \
                              ['/Zc:wchar_t', '/Zc:forScope', '/Oy-']
        if (target == 'x86')or (target=='arm'):
            self.globalCCFlags += ['/Gd']

        self.globalCXXFlags += []
            
        
        self.globalCCDebugFlags += ['/Zc:wchar_t', '/Od', \
                                     '/RTC1', '/Gd', '/MDd', '/Gm']

        if (target == 'x86'):
            self.globalCCDebugFlags += ['/ZI']
        elif ((target == 'x64') or (target=='arm')):
            self.globalCCDebugFlags += ['/Zi']
                                     
        self.globalCCReleaseFlags += ['/GL', '/O2', '/Oi', '/MD', '/Gm-', '/Gy']
        self.globalCCUnitTestFlags += [] + self.globalCCDebugFlags

        #AR Flags
        self.globalARFlags = ['/NOLOGO']

        #Linker Flags
        self.globalLinkerFlags += ['/NXCOMPAT', \
                                   '/MACHINE:' + self.target.upper(), '/NOLOGO']

        self.globalDebugLinkerFlags += ['/DEBUG']
        self.globalReleaseLinkerFlags += ['/RELEASE', '/OPT:REF', '/OPT:ICF', '/SAFESEH']
        self.globalUnitTestLinkerFlags += ['/DEBUG']

        #Libs
        self.globalLibs += ["winmm.lib", "kernel32.lib", "user32.lib", "gdi32.lib", "winspool.lib", \
                            "comdlg32.lib", "advapi32.lib", "shell32.lib", "ole32.lib", \
                            "oleaut32.lib", "uuid.lib", "odbc32.lib", "odbccp32.lib"]

        self.globalDebugLibs += ['debug_new']
        self.globalReleaseLibs += []
        self.globalUnitTestLibs += ['debug_new']

        #LibPath
        self.globalLibPaths = ['C:\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\winv6.3\\um\\' + target]
        if (target == 'x86'):
            self.globalLibPaths += ['C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\VC\\lib']
        elif (target == 'x64'):
            self.globalLibPaths += ['C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\VC\\lib\\amd64']
        elif (target == 'arm'):
            self.globalLibPaths += ['C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\VC\\lib\\arm']

    def getBaseEnvironment(self, armBuild, serverBuild, mingwBuild):

        print ("Building for MSVC-12 Win32 " + self.target)

        #Ensure this is the right path for your system:
        env = Environment(
            CC = os.path.join(self.compilerPath, 'cl.exe'),
            CXX = os.path.join(self.compilerPath, 'cl.exe'),
            LINK = os.path.join(self.compilerPath, 'link.exe'),
            AR = os.path.join(self.compilerPath, 'lib.exe'),
            SERVER_BUILD = False,
            ARM_BUILD = False,
            CLANG_BUILD = False,
            ASM_JS_BUILD = False,
            MSVC_BUILD = True,
            MINGW_CROSS_BUILD = False,
            SYSTEM = "msvc_12_" + self.target
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

