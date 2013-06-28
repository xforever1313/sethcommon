#These functions and constants are used throughout all SConstructs
from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *
from Globals import *
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

###
# Compile flags
###
#Defines
globalDefines = []
    
globalDebugDefines = ["-DDEBUG"]
globalReleaseDefines = ["-DRELEASE"]
globalUnitTestDefines = ["-DUNIT_TEST"]


#Compile Flags
globalCXXFlags = ["-pedantic-errors", "-Werror", "-std=gnu++11", "-Wall", "-Wdouble-promotion", "-Wclobbered", "-Wcast-align", "-Wsign-compare", "-Wempty-body", "-Wcast-qual", "-Wmissing-field-initializers", "-Wtype-limits"]
globalCXXDebugFlags = ["-g", "-Wswitch-enum"]
globalCXXReleaseFlags = ["-O3", "-Wswitch-enum"]
globalCXXUnitTestFlags = ["-g", "-fprofile-arcs", "-ftest-coverage"]

if (sys.platform == "win32"):
    globalCXXFlags += ["-mthreads"]
else:
    globalCXXFlags += ["-pthread"]

#Linker Flags
globalLinkerFlags = ["-static", "-Wall", "-Werror", "-std=gnu++11"]

if(sys.platform == "win32"):
    globalLinkerFlags += ["-mthreads"]
else:
    globalLinkerFlags += ["-pthread"]

#Libs
globalLibsDebug = []
globalLibsRelease = []
globalLibsUnitTest = ["gcov", "gtest", "gmock", "boost_unit_test_framework", "boost_system"]
if (sys.platform == "win32"):
    globalLibsDebug += ["debug_new"]
    globalLibsUnitTest += ["debug_new"]

    
#Parses arguments
def parseArguments(args):
    armBuild = (args.get('arm_build', '0') == '1')
    if (armBuild):
        global armPrefix
        global binDir
        binDir += armPrefix
        global libDir
        libDir += armPrefix
        global objectDir
        objectDir += armPrefix
    return armBuild
    
###
# Environments
###
#rootDir is located in checkoutRoot/Notebook, where this file is located
#isArm is a boolean about whether or not to use the ARM compiler
def createBaseEnvironment (rootDir, isArm):
    if (sys.platform == "win32"):
        env = Environment(            
            tools = ["mingw"],
            ARM = False
        )
    elif (not isArm):
        env = Environment(
            tools = ["default", "gcc", "g++"],
            ARM = False
        )
    else:
        print ("Building for ARM")
        global globalCXXFlags
        globalCXXFlags += ["-march=armv6k"]
        globalCXXFlags.remove("-Werror")       #Warnings are not errors in the arm compiler
        gcc_location = os.environ['GCC_ARM']
        env = Environment(
            tools = ["g++", "gcc", "as", "ar", "link"],
            CC = os.path.join(gcc_location, 'bin/arm-linux-gnueabihf-gcc'),
            CXX = os.path.join(gcc_location,'bin/arm-linux-gnueabihf-g++'),
            AR = os.path.join(gcc_location, 'bin/arm-linux-gnueabihf-ar'),
            AS = os.path.join(gcc_location,'bin/arm-linux-gnueabihf-as'),
            #TODO enable this
            #RANLIB = os.path.join(gcc_location, 'bin/arm-linux-gnueabihf-gcc-ranlib'),
            GCC_ARM = gcc_location,
            ARM = True
        )        
    baseEnvironment = env.Clone(
        PROJECT_ROOT = os.path.abspath("."),
        ENV = {'PATH' : os.environ['PATH']}, #Look in path for tools
    )
    baseEnvironment['NOTEBOOK_ROOT'] = os.path.abspath(rootDir)
    return baseEnvironment
    
def createDebugEnvironment(envBase, includePaths, libs, libPath):
    debugEnvironment = envBase.Clone(
        CPPDEFINES = globalDefines + globalDebugDefines,
        CPPPATH = includePaths,
        CCFLAGS = globalCXXFlags + globalCXXDebugFlags,
        LIBS = libs + globalLibsDebug, 
        LIBPATH = libPath,
        LINKFLAGS = globalLinkerFlags,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, debugDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, debugDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, debugDir))
    )
    return debugEnvironment
    
def createReleaseEnvironment(envBase, includePaths, libs, libPath):
    releaseEnvironment = envBase.Clone(
        CPPDEFINES = globalDefines + globalReleaseDefines,
        CPPPATH = includePaths,
        CCFLAGS = globalCXXFlags + globalCXXReleaseFlags,
        LIBS = libs + globalLibsRelease, 
        LIBPATH = libPath,
        LINKFLAGS = globalLinkerFlags,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, releaseDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, releaseDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, releaseDir))
    )
    return releaseEnvironment
    
def createUnitTestEnvironment(envBase, includePaths, libs, libPath):
    testEnvironment = envBase.Clone(
        CPPDEFINES = globalDefines + globalUnitTestDefines,
        CPPPATH = includePaths,
        CCFLAGS = globalCXXFlags + globalCXXUnitTestFlags,
        LIBS = libs + globalLibsUnitTest, 
        LIBPATH = libPath,
        LINKFLAGS = globalLinkerFlags,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, unitTestDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, unitTestDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, unitTestDir))
    )
    
    RunTest = Builder(action = testRunner)
    testEnvironment.Append(BUILDERS = {"Test" : RunTest})
    return testEnvironment

###
# Create targets
###
    
def createExe(env, exeName, sourceFiles):
    exeTarget = env.Program(target = os.path.join(env['BINDIR'], exeName), source = sourceFiles)
    return exeTarget
    
def createUnitTestExe(env, sourceFiles, coverageFiles, armBuild):
    testExeTarget = env.Program(target = os.path.join(env['BINDIR'], "unit_test"), source = sourceFiles)
    runTestTarget = env.Test(target = profilingDataFile, source = coverageFiles)
    AlwaysBuild(runTestTarget)
    Execute(Delete(Glob(os.path.join(testOutputDir, '*')))) #Remove old test outputs
    if (not armBuild):
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
    if (not os.path.exists(docDir)):
        os.mkdir(docDir)
    target = env.Doxygen(target = os.path.join(env['PROJECT_ROOT'], doxygenDir, "html/index.html"), source = doxygenFiles)
    Clean(target, os.path.join(env['PROJECT_ROOT'], doxygenDir))
    return target

def createApiMoveTarget(env, apiFiles):
    readMe = os.path.join(env['PROJECT_ROOT'], apiDir, 'readme.txt')
    apiMove = Builder(action = apiBuilder)
    env.Append(BUILDERS = {"Api" : apiMove})
    target = env.Api(target = readMe, source = apiFiles)
    Depends(target, apiFiles)
    Clean(target, os.path.join(env['PROJECT_ROOT'], apiDir))
    return target
    
###
# Other helper functions
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
    status = subprocess.call("doxygen Doxyfile", shell=True)
    if (status != 0):
        raise Error("***Doxygen failed***")

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
   
#Runs the test, and coverage while we are at it :P
def testRunner(target, source, env):
    thisDir = os.getcwd()
    os.chdir(os.path.join(binDir, unitTestDir))
    
    if (sys.platform == "win32"):
        status = subprocess.call("unit_test.exe")
    else:
        status = subprocess.call("./unit_test")
    os.chdir(thisDir)

    #todo make this work on linux
    if (status == 0):
        if(sys.platform == "win32"):
            print("Running Coverage")
            try:
                Execute(Delete(codeCoverageDir)) #Remove old code coverage
                time.sleep(3)
            except:
                print("CodeCoverage directory missing, recreating")

            os.mkdir(codeCoverageDir)
            
            gcovString = "gcov -r -s \"" + thisDir + "\" -o \"" + os.path.join(thisDir, objectDir, unitTestDir) + "\" "
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
            
    else:
        raise Exception("Test Failed!")
        
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
