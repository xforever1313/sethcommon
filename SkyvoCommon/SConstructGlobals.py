#These functions and constants are used throughout all SConstructs
from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *
from Globals import *
from SkyvoCommonGlobals import *
from multiprocessing import cpu_count
from operator import itemgetter
import glob
import os
import shutil
import string
import subprocess
import sys
import time

if (sys.platform == "win32"):
    #defaults to MSVC until set to MinGW
    SCons.Defaults.DefaultEnvironment(tools = [])

SetOption('num_jobs', cpu_count())  #Sets -j to the number of cores  This happens when you import this file

###
# Compile flags
###
#Defines
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
globalCXXFlags = ["-pedantic-errors", "-Werror", "-std=gnu++11", "-Wall", "-Wcast-align", "-Wsign-compare", \
                  "-Wempty-body", "-Wcast-qual", "-Wmissing-field-initializers", "-Wtype-limits", "-fstack-protector-all"]
globalCXXDebugFlags = ["-g", "-Wswitch-enum"]
globalCXXReleaseFlags = ["-O3", "-Wswitch-enum", "-fdata-sections", "-ffunction-sections"]
globalCXXUnitTestFlags = ["-g"]

clangGlobalCXXFlags = ['-Wno-return-type-c-linkage', '-stdlib=libc++']
clangUnitTestFlags = []
if (sys.platform == "darwin"):
    clangUnitTestFlags += ['-DGTEST_USE_OWN_TR1_TUPLE=1']

gccGlobalCXXFlags = ['-Wclobbered', '-Wdouble-promotion']
gccUnitTestCXXFlags = ['-fprofile-arcs', '-ftest-coverage']

if (sys.platform == "win32"):
    globalCXXFlags += ["-pthread"]
else:
    globalCXXFlags += ["-pthread"]

#Linker Flags
globalLinkerFlags = ["-Wall", "-Werror", "-std=gnu++11"]
releaseLinkerFlags = []
if (sys.platform == "darwin"):
    releaseLinkerFlags += ["-Wl,-dead_strip"]
else:
    releaseLinkerFlags += ['-s', '-Wl,--gc-sections', '-Wl,--strip-all']

unitTestLinkerFlags = []

if(sys.platform == "win32"):
    globalLinkerFlags += ["-static", "-pthread"]
elif (sys.platform == "darwin"):
    globalLinkerFlags += []
else:
    globalLinkerFlags += ["-pthread"]

#Libs
globalLibsDebug = []
globalLibsRelease = []
globalLibsUnitTest = ["gtest", "gmock", "CppUTest"]

gccUnitTestLibs = ['gcov']
clangUnitTestLibs = []

if (sys.platform == "win32"):
    globalLibsDebug += ["ssp", "debug_new"] #Lib SSP is still needed in mingw, but not linux
    globalLibsRelease += ["ssp"]
    globalLibsUnitTest += ["ssp", "debug_new"]

#Parses arguments
def parseArguments(args):
    serverBuild = (args.get('server_build', '0') == '1')
    return serverBuild
    
###
# Environments
###

def armBuildAdd(env):
    if(env['ARM_BUILD']):
        env.Append(CCFLAGS = ['-arch', 'armv7s', '-miphoneos-version-min=7.0', '-isysroot', '/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.0.sdk'])
        env.Append(LINKFLAGS = ['-arch', 'armv7s', '-miphoneos-version-min=7.0', '-isysroot', '/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.0.sdk'])
        env.Append(LIBPATH = ['/skyvo/lib/ios'])
    elif (sys.platform == "darwin"):
        env.Append(LIBPATH = ['/skyvo/lib/x86'])

def clangBuildAdd(env):
    if (env['CLANG_BUILD']):
        if (sys.platform == "darwin"):
            env.Append(CCFLAGS = ['-isystem', '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/c++/v1'])
            env.Append(CCFLAGS = ['-isystem', '/skyvo/include'])
        else:
            env.Append(CCFLAGS = ['-isystem', '/usr/include/i386-linux-gnu/c++/4.8'])
            env.Append(CCFLAGS = ['-isystem', '/usr/include/c++/4.8.1'])

        env.Append(CCFLAGS = ['-D__STRICT_ANSI__'])

def serverBuildAdd(env):
    if (env['SERVER_BUILD']):
        env.Append(CPPPATH = ['/skyvo/include'])
        env.Append(LIBPATH = ['/skyvo/lib'])
        env.Append(CCFLAGS = ['-isystem', '/skyvo/include'])

def addPlatformFlags(env):
    serverBuildAdd(env)
    clangBuildAdd(env)
    armBuildAdd(env)

def addSystemDefines(env):
    system = ""
    if (env['SYSTEM'] == "mingw32"):
        system = "MINGW"
    elif (env['SYSTEM'] == "clangArm" or env['SYSTEM'] == "clangx86"):
        system = "CLANG"
    else:
        system = "GCC"
    env.Append(CPPDEFINES = [system])

def createBaseEnvironment (rootDir, args):
    serverBuild = parseArguments(args)
    clangBuild = (args.get('clang_build', '0') == '1')
    armBuild = (args.get('arm_build', '0') == '1')

    if (sys.platform == "win32"):
        print "Building for mingw32"
        env = Environment(            
            tools = ["mingw"],
            SERVER_BUILD = False,
            ARM_BUILD = False,
            CLANG_BUILD = False,
            SYSTEM = "mingw32"
        )
    elif(clangBuild):
        if (armBuild):
            print "Building for clang arm"
            env = Environment(
                CC = "clang",
                CXX = "clang++",
                LINK = "clang++",
                SERVER_BUILD = serverBuild,
                ARM_BUILD = True,
                CLANG_BUILD = True,
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
                SYSTEM = "clangx86"
            ) 
    else:
        print "Building for gcc x86"
        if (serverBuild):
            env = Environment(
                CC = "gcc-4.8",
                CXX = "g++-4.8",
                LINK = "g++-4.8",
                SERVER_BUILD = True,
                ARM_BUILD = False,
                CLANG_BUILD = False,
                SYSTEM = "gccx86"
            )
        else:
            env = Environment(
                tools = ["default", "gcc", "g++"],
                SERVER_BUILD = False,
                ARM_BUILD = False,
                CLANG_BUILD = False,
                SYSTEM = "gccx86"
            )
    baseEnvironment = env.Clone(
        PROJECT_ROOT = os.path.abspath("."),
        ENV = {'PATH' : os.environ['PATH']} #Look in path for tools
    )
    baseEnvironment['BASE_DIR'] = os.path.abspath(rootDir)
    return baseEnvironment
    
def createDebugEnvironment(envBase, includePaths, libs, libPath):
    debugEnvironment = envBase.Clone(
        CPPDEFINES = globalDefines + globalDebugDefines,
        CPPPATH = includePaths,
        CCFLAGS = globalCXXFlags + globalCXXDebugFlags,
        LIBS = libs + globalLibsDebug, 
        LIBPATH = libPath,
        LINKFLAGS = globalLinkerFlags,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, envBase['SYSTEM'], debugDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, envBase['SYSTEM'], debugDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, envBase['SYSTEM'], debugDir))
    )
    addPlatformFlags(debugEnvironment)

    if (debugEnvironment['CLANG_BUILD']):
        debugEnvironment.Append(CCFLAGS = clangGlobalCXXFlags)
    else:
        debugEnvironment.Append(CCFLAGS = gccGlobalCXXFlags)

    addSystemDefines(debugEnvironment)

    return debugEnvironment
    
def createReleaseEnvironment(envBase, includePaths, libs, libPath):
    releaseEnvironment = envBase.Clone(
        CPPDEFINES = globalDefines + globalReleaseDefines,
        CPPPATH = includePaths,
        CCFLAGS = globalCXXFlags + globalCXXReleaseFlags,
        LIBS = libs + globalLibsRelease, 
        LIBPATH = libPath,
        LINKFLAGS = globalLinkerFlags + releaseLinkerFlags,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, envBase['SYSTEM'], releaseDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, envBase['SYSTEM'], releaseDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, envBase['SYSTEM'], releaseDir))
    )
    addPlatformFlags(releaseEnvironment)

    if (releaseEnvironment['CLANG_BUILD']):
        releaseEnvironment.Append(CCFLAGS = clangGlobalCXXFlags)
    else:
        releaseEnvironment.Append(CCFLAGS = gccGlobalCXXFlags)

    addSystemDefines(releaseEnvironment)

    return releaseEnvironment
    
def createUnitTestEnvironment(envBase, includePaths, libs, libPath):
    testEnvironment = envBase.Clone(
        CPPDEFINES = globalDefines + globalUnitTestDefines,
        CPPPATH = includePaths + [os.path.join(getCppUTestPath(envBase['BASE_DIR']), includeDir)],
        CCFLAGS = globalCXXFlags + globalCXXUnitTestFlags,
        LIBS = libs + globalLibsUnitTest, 
        LIBPATH = libPath,
        LINKFLAGS = globalLinkerFlags + unitTestLinkerFlags,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, envBase['SYSTEM'], unitTestDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, envBase['SYSTEM'], unitTestDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, envBase['SYSTEM'], unitTestDir))
    )
    testEnvironment.Append(LIBPATH = os.path.join(getCppUTestPath(envBase['BASE_DIR']), libDir, testEnvironment['SYSTEM']))
    addPlatformFlags(testEnvironment)

    if(testEnvironment['CLANG_BUILD']):
        testEnvironment.Append(CCFLAGS = clangGlobalCXXFlags + clangUnitTestFlags)
        testEnvironment.Append(LIBS = clangUnitTestLibs)
    else:
        testEnvironment.Append(CCFLAGS = gccGlobalCXXFlags + gccUnitTestCXXFlags)
        testEnvironment.Append(LIBS = gccUnitTestLibs)

    RunTest = Builder(action = testRunner)
    testEnvironment.Append(BUILDERS = {"Test" : RunTest})

    addSystemDefines(testEnvironment)
    return testEnvironment

###
# Create targets
###

def createExe(env, exeName, sourceFiles):
    exeTarget = env.Program(target = os.path.join(env['BINDIR'], exeName), source = sourceFiles)
    return exeTarget
    
def createUnitTestExe(env, sourceFiles, coverageFiles):
    testExeTarget = env.Program(target = os.path.join(env['BINDIR'], "unit_test"), source = sourceFiles)
    runTestTarget = env.Test(target = profilingDataFile, source = coverageFiles)
    AlwaysBuild(runTestTarget)
    Execute(Delete(Glob(os.path.join(testOutputDir, '*')))) #Remove old test outputs
    Depends(runTestTarget, testExeTarget)
    return (testExeTarget, runTestTarget)

def createStaticLib(env, libName, sourceFiles):
    staticLibTarget = env.StaticLibrary(target = os.path.join(env['LIBDIR'], libName), source = sourceFiles)
    return staticLibTarget

def createSharedLib(env, sourceFiles, libName):
    sharedLibTarget = env.SharedLibrary(target = os.path.join(env['LIBDIR'], libName), source = sourceFiles)
    return sharedLibTarget

def createDoxygenTarget(env, doxygenFiles):
    Doxygen = Builder(action = doxgyenBuilder)
    env.Append(BUILDERS = {"Doxygen" : Doxygen})
    createDir(docDir)
    target = env.Doxygen(target = doxygenData, source = doxygenFiles)
    Clean(target, os.path.join(env['PROJECT_ROOT'], doxygenDir))
    return target

def createCppCheckTarget(env, sources):
    cppCheck = Builder(action = cppCheckBuilder)
    env.Append(BUILDERS = {"cppCheck" : cppCheck})
    target = env.cppCheck(target = cppCheckData, source = sources)
    AlwaysBuild(target)
    return target

def createApiMoveTarget(env, apiFiles):
    readMe = os.path.join(env['PROJECT_ROOT'], apiDir, 'readme.txt')
    apiMove = Builder(action = apiBuilder)
    env.Append(BUILDERS = {"Api" : apiMove})
    target = env.Api(target = readMe, source = apiFiles)
    Depends(target, apiFiles)
    Clean(target, os.path.join(env['PROJECT_ROOT'], apiDir))
    return target

def getCompiledObjectsWithDateVersionObject(env, sources, dateVersionFile, args):
    compiledObjects = env.Object(sources)
    
    dateVersionEnvironment = env.Clone()
    dateVersionEnvironment.Append(CPPDEFINES = getDateVersionDefine(env['BASE_DIR'], args))
    dateVersionObject = dateVersionEnvironment.Object(dateVersionFile)
    Depends(dateVersionObject, compiledObjects)
    Depends(dateVersionObject, getVersionFile(env, args))
    return compiledObjects + dateVersionObject
    
###
# Other helper functions
###
def getVersionFile(env, args):
    if (args.get('version_file', '0') == '0'): #If version file not specified, use the default one
        versionFile = os.path.join(env['BASE_DIR'], buildDir, 'version.txt')
    else:
        versionFile = args.get('version_file')
    return versionFile

def getDateVersionValue(baseDir, args):
    if (args.get('version', '0') == '0'):
        dateVersionString = getVersion(baseDir) + '+' + replaceSpacesWithUnderscores(getUserName())
    else:
        dateVersionString = args.get('version')
    return dateVersionString

    
def getDateVersionDefine(baseDir, args):

    return 'VERSION="\\"' + getDateVersionValue(baseDir, args) + '\\""'

def filterSourceFiles(sourceFiles, blackListedFiles):
    for file in blackListedFiles:
        sourceFiles.remove(file)
    return sourceFiles
   
def parseGcovOutput(gcovOutput):
    lines = gcovOutput.split('\n')
    coverageTupples = []
    totalCoverage = ""
    i = 0
    while (i < len(lines)):
        if (lines[i].find("File ") == 0):
            fileName = lines[i][6:]
            fileName = os.path.basename(fileName.rstrip("'\r\n"))
            i += 1
            linesExecuted = lines[i][15:]
            linesExecuted = linesExecuted[0:linesExecuted.find('%')] #strip all alpabet characters, only use number
            coverageTupples += [(fileName, float(linesExecuted))]
        elif(lines[i].find("Lines executed:") == 0):
            totalCoverage = lines[i]
        i += 1
    coverageTupples = sorted(coverageTupples, key=itemgetter(1, 0))
    
    returnString = "\nCoverage Data:\n"
    for tupple in coverageTupples:
        returnString += "[" + str(tupple[1]) + "%] " + tupple[0] + "\n"
    returnString += "\n" + totalCoverage + "\n"
    return returnString

#Extracts libs to the given location
def libExtractor(extractionLocation, libsToExtract):
    #save the current dir
    thisDir = os.getcwd()
    #change to the destination folder
    os.chdir(extractionLocation)
    print "Extracting Libs..."
    for lib in libsToExtract:
        commandStr = "ar x " + str(lib)
        print commandStr
        status = subprocess.call(commandStr, shell=True)
        if (status != 0):
            raise Exception("Extraction Error")
            
    #return to the old directory
    os.chdir(thisDir)

#make a directory to dump all test output files
def createTestOutputFolder(env):
    testOutputPath = os.path.join(env['PROJECT_ROOT'], "testOutput")
    if (os.path.exists(testOutputPath) == False):
        os.mkdir(testOutputPath)

###
# Builders
###    

#Copies the API include files (given as source) to project_root/api
def apiBuilder(target, source, env):
    readmeText='''
    DO NOT EDIT THESE FILES.  
    These files are only to make a convient place to host the api for this project.
    If you wish to edit these files, please go into include/api and rebuild.  
    When you rebuild, these files get updated
    '''
    readme = open(os.path.join(env['OBJPREFIX'], 'apireadme.txt'), 'w')
    readme.write(readmeText)
    readme.close()
    
    for file in source:
        Execute(Copy(os.path.join(env['PROJECT_ROOT'], apiDir), file))
        
    Execute(Copy(os.path.join(env['PROJECT_ROOT'], apiDir, 'readme.txt'), 'apireadme.txt'))
    Execute(Delete('apireadme.txt'))

#Source is only passed in so we know if we need to rebuild if any sources change
def doxgyenBuilder(target, source, env):
    print "Running Doxygen..."
    return subprocess.call("doxygen Doxyfile " + getRedirectString(str(target[0])), shell=True)

def cppCheckBuilder(target, source, env):
    sources = ""
    includeCommandString = ""
    for src in source:
        sources += (" " + str(src))
    for include in env['CPPPATH']:
        includeCommandString += (" -I" + include)
    commandStr = 'cppcheck  --error-exitcode=13 --enable=warning --enable=style --enable=information -q -j ' + str(cpu_count()) + \
	' ' + includeCommandString + sources + ' ' + getRedirectString(str(target[0]))
    print commandStr
    status = subprocess.call(commandStr, shell=True)
    
    if (status == 0):
        print "\ncppcheck ran and no errors were found!"
    else:
        sys.stderr.write("** CPP Check detected errors.  Please refer to " + str(target[0]) + " for more details\n")
    return status
   
#Runs the test, and coverage while we are at it :P
def testRunner(target, source, env):
    thisDir = os.getcwd()
    cwdDir = env['BINDIR']
    if (sys.platform == "win32"):
        status = subprocess.call("unit_test.exe", cwd=cwdDir, shell=True)
    else:
        status = subprocess.call("./unit_test", cwd=cwdDir, shell=True)

    if ((status == 0) and not env['CLANG_BUILD']):
        print("Running Coverage")
        try:
            Execute(Delete(codeCoverageDir)) #Remove old code coverage
            time.sleep(3)
        except:
            print("CodeCoverage directory missing, recreating")

        os.mkdir(codeCoverageDir)
        if (env['SERVER_BUILD']):
            gcovCommand = 'gcov-4.8'
        else:
            gcovCommand = 'gcov'
        gcovString = gcovCommand + " -r -s \"" + thisDir + "\" -o \"" + os.path.join(thisDir, objectDir, env['SYSTEM'], unitTestDir) + "\" "
        for file in source:
            gcovString += (os.path.join(thisDir, str(file)) + " ")
        print (gcovString)
        
        gcovProc = subprocess.Popen(gcovString, shell=True, stdout=subprocess.PIPE)
        gcovOutput = gcovProc.communicate()[0]
        parsedGcovString = parseGcovOutput(gcovOutput)
        
        #Save coverage data, and print it
        outFile = open(profilingDataFile, "w")
        outFile.write(parsedGcovString)
        outFile.close()
        
        print parsedGcovString
        
        #Move all .gcov files to  CodeCoverage folder
        gcovGlob = glob.glob("*.gcov")    
        for file in gcovGlob:
            shutil.move(file, codeCoverageDir)
            
    elif (status != 0):
        raise Exception("Test Failed!")
