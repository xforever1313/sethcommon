'''
These compile flags are for the GCC compile for the Rasberry Pi.  
These are ADDITIONAL flags in addition to flags defined in GnuCompilerGlobals
'''
import sys

from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *

from GnuCompilerGlobals import *

class ArmLinuxGnueabihfGlobals(GnuCompilerGlobals):

    def __init__(self):
        GnuCompilerGlobals.__init__(self)
        #CppDefines
        self.globalDefines += ['GCC']

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

    def getBaseEnvironment(self, armBuild, serverBuild, mingwBuild):
        print ("Building for arm-linux-gnueabihf")
        env = Environment(
            CC = "arm-linux-gnueabihf-gcc",
            CXX = "arm-linux-gnueabihf-g++",
            LINK = "arm-linux-gnueabihf-g++",
            AR = "arm-linux-gnueabihf-ar",
            AS = "arm-linux-gnueabihf-as",
            RANLIB = "arm-linux-gnueabihf-ranlib",
            SERVER_BUILD = False,
            ARM_BUILD = False,
            CLANG_BUILD = False,
            ASM_JS_BUILD = False,
            MSVC_BUILD = False,
            MINGW_CROSS_BUILD = False,
            SYSTEM = "arm-linux-gnuabihf"
        )
        
        return env

    def addMapFlag(self, env, mapName):
        if (not os.path.exists(env['BINDIR'])):
            #This is here so during shared lib builds, the map file is still made
            os.makedirs(env['BINDIR']) 
        env.Append(LINKFLAGS = ['-Wl,-Map,' + os.path.join(env['BINDIR'], mapName)])

    def extendDebugEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalDebugDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags + self.globalCXXDebugFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCDebugFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalDebugLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalDebugLibs)
        envBase.Append(LIBPATH = libPath)
        self.addMapFlag(envBase, envBase['PROJECT_NAME'] + '.map')

    def extendReleaseEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalReleaseDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags + self.globalCXXReleaseFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCReleaseFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalReleaseLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalReleaseLibs)
        envBase.Append(LIBPATH = libPath)
        self.addMapFlag(envBase, envBase['PROJECT_NAME'] + '.map')

    def extendUnitTestEnvironment(self, envBase, libs, libPath):
        envBase.Append(CPPDEFINES = self.globalDefines + self.globalUnitTestDefines)
        envBase.Append(CXXFLAGS = self.globalCXXFlags + self.globalCXXUnitTestFlags)
        envBase.Append(CCFLAGS = self.globalCCFlags + self.globalCCUnitTestFlags)
        envBase.Append(LINKFLAGS = self.globalLinkerFlags + self.globalUnitTestLinkerFlags)
        envBase.Append(LIBS = libs + self.globalLibs + self.globalUnitTestLibs)
        envBase.Append(LIBPATH = libPath)
        self.addMapFlag(envBase, 'unit_test.map')

