'''
These flags are for Clangs compiler.  These are ADDITIONAL flags in addition to flags
defined in GnuCompilerGlobals
'''
import sys

from GnuCompilerGlobals import *

class ClangCompilerGlobals(GnuCompilerGlobals):

    def __init__(self):
        GnuCompilerGlobals.__init__(self)

        #CPPDefines
        self.globalUnitTestDefines += ['GTEST_USE_OWN_TR1_TUPLE=1']
        
        #Compile Flags
        self.globalCCFlags += []
        if (sys.platform = "darwin"):
            self.globalCCFlags += ['-isystem', '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/c++/v1']
        else if (sys.platform != "win32"):
            self.globalCCFlags += ['-isystem', '/usr/include/i386-linux-gnu/c++/4.8']
            self.globalCCFlags += ['-isystem', '/usr/include/c++/4.8.1']   
        
        self.globalCCDebugFlags += []
        self.globalCCReleaseFlags += []
        self.globalCCUnitTestFlags += []
        
        self.globalCXXFlags += ['-Wno-return-type-c-linkage', '-stdlib=libc++']
        
        
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
                SYSTEM = "clangx86"
            )
        globalDefines += ['CLANG']
    
        return env
    
    def extendDebugEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalDebugDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCDebugFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalDebugLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalDebugLibs)
        envBase.Append(LIBPATH = libPath)
    
        if (envBase['ARM_BUILD']):
            envBase.Append(CCFLAGS = self.globalArmCCFlags)
            envBase.Append(LINKFLAGS = self.globalArmLinkFlags)
    
    def extendReleaseEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalReleaseDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCReleaseFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalReleaseLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalReleaseLibs)
        envBase.Append(LIBPATH = libPath)
    
        if (envBase['ARM_BUILD']):
            envBase.Append(CCFLAGS = self.globalArmCCFlags)
            envBase.Append(LINKFLAGS = self.globalArmLinkFlags)
    
    def extendUnitTestEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalUnitTestDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCUnitTestFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalUnitTestLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalUnitTestLibs)
        envBase.Append(LIBPATH = libPath)
    
        if (envBase['ARM_BUILD']):
            envBase.Append(CCFLAGS = self.globalArmCCFlags)
            envBase.Append(LINKFLAGS = self.globalArmLinkFlags)
