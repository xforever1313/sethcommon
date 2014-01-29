'''
These flags are for the emscripten C++ compiler.  These flags are in addition to those
defined in GnuCompilerGlobals.py
'''
import sys
from multiprocessing import cpu_count

from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *

from GnuCompilerGlobals import *

class EmscriptenCompilerGlobals(GnuCompilerGlobals):
    
    def __init__(self):
        GnuCompilerGlobals.__init__(self)

        #CPPDefines
        self.globalDefines += []

        self.globalDebugDefines += []
        self.globalReleaseDefines += []
        self.globalUnitTestDefines += ['GTEST_USE_OWN_TR1_TUPLE=1']
        
        #Compile Flags
        self.globalCCFlags += ['-s', 'ASM_JS=1', '-O2', '-Wno-warn-absolute-paths', \
                               '-s', 'DISABLE_EXCEPTION_CATCHING=0', '-Wno-return-type-c-linkage']
        
        self.globalCXXFlags += ['-stdlib=libc++']
        
        self.globalCCDebugFlags += []
        self.globalCCReleaseFlags += ['-O2']
        self.globalCCUnitTestFlags += []
        
        #Linker Flags
        self.globalLinkerFlags += ['-s', 'ASM_JS=1', '-O2', '-Wno-warn-absolute-paths', \
                                   '-s', 'DISABLE_EXCEPTION_CATCHING=0']
        
        self.globalDebugLinkerFlags += []
        self.globalReleaseLinkerFlags += []
        self.globalUnitTestLinkerFlags += []
    
    def getBaseEnvironment(self, armBuild, serverBuild):
        print "Building for ASM.js"
        env = Environment(
            tools = ["mingw"],
            CC = "emcc",
            CXX = "emcc",
            LINK = "emcc",
            AR = "emar",
            RANLIB = "emranlib",
            SERVER_BUILD = serverBuild,
            ARM_BUILD = False,
            CLANG_BUILD = True,
            ASM_JS_BUILD = True,
            MSVC_BUILD = False,
            SYSTEM = "asmjs"
        )
        env['ENV']['emscripten'] = os.environ['emscripten']
        env['ENV']['HOME'] = os.environ['USERPROFILE']
        env['ENV']['NUMBER_OF_PROCESSORS'] = cpu_count()
        
        self.globalDefines += ['ASM_JS']
        self.globalDefines += ['CLANG']
    
        return env
    
    def extendDebugEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalDebugDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCDebugFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalDebugLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalDebugLibs)
        envBase.Append(LIBPATH = libPath)
        envBase['PROGSUFFIX'] = '.html'
    
    def extendReleaseEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalReleaseDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCReleaseFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalReleaseLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalReleaseLibs)
        envBase.Append(LIBPATH = libPath)
        envBase['PROGSUFFIX'] = '.html'
    
    def extendUnitTestEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalUnitTestDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCUnitTestFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalUnitTestLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalUnitTestLibs)
        envBase.Append(LIBPATH = libPath)
        envBase['PROGSUFFIX'] = '.js'
