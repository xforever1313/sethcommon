#These functions and constants are used throughout all SConstructs
from SCons.Script.Main import *
from SCons.Environment import *
from SCons.Builder import *
from Globals import *
import os
import sys
import shutil
import glob
import time
import subprocess

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
if (sys.platform != "win32"):
	globalUnitTestDefines += ["-DBOOST_TEST_DYN_LINK"]

#Compile Flags
globalCXXFlags = ["-Wall", "-pedantic", "-Werror", "-std=gnu++11", "-mthreads", "-Wdouble-promotion"]
globalCXXDebugFlags = ["-g", "-Wswitch-enum"]
globalCXXReleaseFlags = ["-O3", "-Wswitch-enum"]
globalCXXUnitTestFlags = ["-g", "-fprofile-arcs", "-ftest-coverage"]

#Linker Flags
globalLinkerFlags = ["-Wall", "-Werror", "-std=gnu++11", "-mthreads"]

#Libs
globalLibsDebug = []
globalLibsRelease = []
globalLibsUnitTest = ["gcov", "boost_unit_test_framework", "boost_system"]
if (sys.platform == "win32"):
	globalLibsDebug += ["debug_new"]
	globalLibsUnitTest += ["debug_new"]

###
# Environments
###
#rootDir is located in checkoutRoot/Notebook, where this file is located
#isArm is a boolean about whether or not to use the ARM compiler
def createBaseEnvironment (rootDir, isArm):
    if (sys.platform == "win32"):
        env = Environment(tools = ["mingw"])
    elif (not isArm):
        env = Environment(tools = ["default", "gcc", "g++"])
    else:
        #TODO make one for ARM
        env = Environment()
    baseEnvironment = env.Clone(
        ENV = {'PATH' : os.environ['PATH']}, #Look in path for tools
        PROJECT_ROOT = os.path.abspath(".")
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
    
def createUnitTestExe(env, sourceFiles, coverageFiles):
    testExeTarget = env.Program(target = os.path.join(env['BINDIR'], "unit_test"), source = sourceFiles)
    runTestTarget = env.Test(target = os.path.join(env['BINDIR'], "run_test"), source = coverageFiles)
    return (testExeTarget, runTestTarget)

def createStaticLib(env, libName, sourceFiles):
    staticLibTarget = env.StaticLibrary(target = os.path.join(env['LIBDIR'], libName), source = sourceFiles)
    return staticLibTarget

def createSharedLib(env, sourceFiles, libName):
    sharedLibTarget = env.SharedLibrary(target = os.path.join(env['LIBDIR'], libName), source = sourceFiles)
    return sharedLibTarget

###
# Other helper functions
###
    
def filterSourceFiles(sourceFiles, blackListedFiles):
    for file in blackListedFiles:
        sourceFiles.remove(file)
    return sourceFiles
    
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
				shutil.rmtree(codeCoverageDir)
				#wait for it to be deleted
				time.sleep(3)
			except:
				print("CodeCoverage directory missing, recreating")

			os.mkdir(codeCoverageDir)
			
			gcovString = "gcov -r -s \"" + thisDir + "\" -o \"" + os.path.join(thisDir, objectDir, unitTestDir) + "\" "
			for file in source:
				gcovString += (os.path.join(thisDir, str(file)) + " ")
			print (gcovString)
			
			subprocess.call(gcovString)
			
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