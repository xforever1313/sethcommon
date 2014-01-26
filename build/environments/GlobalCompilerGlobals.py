'''
These compile flags are for ALL compilers
'''
import sys

class GlobalCompilerGlobals():

    #CppDefines
    globalDefines = []

    if (sys.platform == "win32"):
        globalDefines += ["WIN_32"]
    elif (sys.platform == "darwin"):
        globalDefines += ["DARWIN"]
    else:
        globalDefines += ["LINUX"]


    globalDebugDefines = ["DEBUG"]
    globalReleaseDefines = ["RELEASE"]
    globalUnitTestDefines = ["UNIT_TEST", "UT_NEW_MACROS_DISABLED", "UT_NEW_OVERRIDES_DISABLED"]

    #Compile Flags
    globalCCFlags = []
    globalCXXFlags = []

    globalCCDebugFlags = []
    globalCCReleaseFlags = []
    globalCCUnitTestFlags = []

    #Link Flags
    globalLinkerFlags = []

    globalDebugLinkerFlags = []
    globalReleaseLinkerFlags = []
    globalUnitTestLinkerFlags = []

    #Libs
    globalLibs = []

    globalDebugLibs = []
    globalReleaseLibs = []
    globalUnitTestLibs = ['gmock', 'CppUTest']

