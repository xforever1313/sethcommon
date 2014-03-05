'''
These flags are for Clangs compiler.  These are ADDITIONAL flags in addition to flags
defined in GnuCompilerGlobals
'''
import sys
from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *

from GnuCompilerGlobals import *

class ClangCompilerGlobals(GnuCompilerGlobals):

    def __init__(self):
        GnuCompilerGlobals.__init__(self)

        #CPPDefines
        self.globalDefines += ['__STRICT_ANSI__']
        self.globalUnitTestDefines += ['GTEST_USE_OWN_TR1_TUPLE=1']
        
        #Compile Flags
        self.globalCCFlags += []
        if (sys.platform == "darwin"):
            self.globalCCFlags += ['-isystem', '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/c++/v1']
            self.globalCCFlags += ['stdlib=libc++']
        elif (sys.platform != "win32"):
            self.globalCCFlags += ['-stdlib=libc++']
        
        self.globalCCDebugFlags += ['-O0']
        self.globalCCReleaseFlags += ['-O3']
        self.globalCCUnitTestFlags += ['-O0']
        
        self.globalCCFlags += ['-Wno-return-type-c-linkage']
        
        
        #Link Flags
        self.globalLinkerFlags += []
        self.globalDebugLinkerFlags += []
        self.globalReleaseLinkerFlags += []
        self.globalUnitTestLinkerFlags += []
        
        #Arm Flags for IPhone
        self.globalArmCCFlags = ['-arch', 'armv7s', '-miphoneos-version-min=7.0', '-isysroot', \
                                 '/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.0.sdk'] 
        
        self.globalArmLinkFlags = ['-arch', 'armv7s', '-miphoneos-version-min=7.0', '-isysroot', \
                                   '/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.0.sdk']

        #Libs
        self.globalLibs += ['c++abi', 'c++']

        self.globalDebugLibs += []
        self.globalReleaseLibs += []
        self.globalUnitTestLibs += []
            
    def getBaseEnvironment(self, armBuild, serverBuild):
        if (armBuild):
            print "Building for clang arm"
            env = Environment(
                CC = "clang",
                CXX = "clang++",
                LINK = "clang++",
                SERVER_BUILD = serverBuild,
                ARM_BUILD = True,
                CLANG_BUILD = True,
                ASM_JS_BUILD = False,
                MSVC_BUILD = False,
                SYSTEM = "clangArm"    
            )
        else:
            print "Building for clang x86"
            env = Environment(
                CC = "clang",
                CXX = "clang++",
                LINK = "clang++",
                SERVER_BUILD = serverBuild,
                ARM_BUILD = False,
                CLANG_BUILD = True,
                ASM_JS_BUILD = False,
                MSVC_BUILD = False,
                SYSTEM = "clangx86"
            )
        self.globalDefines += ['CLANG']
    
        return env
    
    def extendDebugEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalDebugDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCDebugFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalDebugLinkerFlags)
        envBase.Append(LIBS = libs + self.globalDebugLibs + self.globalLibs)
        envBase.Append(LIBPATH = libPath)
    
        if (envBase['ARM_BUILD']):
            envBase.Append(CCFLAGS = self.globalArmCCFlags)
            envBase.Append(LINKFLAGS = self.globalArmLinkFlags)
    
    def extendReleaseEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalReleaseDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCReleaseFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalReleaseLinkerFlags)
        envBase.Append(LIBS = libs + self.globalReleaseLibs + self.globalLibs)
        envBase.Append(LIBPATH = libPath)
    
        if (envBase['ARM_BUILD']):
            envBase.Append(CCFLAGS = self.globalArmCCFlags)
            envBase.Append(LINKFLAGS = self.globalArmLinkFlags)
    
    def extendUnitTestEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalUnitTestDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCUnitTestFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalUnitTestLinkerFlags)
        envBase.Append(LIBS = libs + self.globalUnitTestLibs + self.globalLibs)
        envBase.Append(LIBPATH = libPath)
    
        if (envBase['ARM_BUILD']):
            envBase.Append(CCFLAGS = self.globalArmCCFlags)
            envBase.Append(LINKFLAGS = self.globalArmLinkFlags)
