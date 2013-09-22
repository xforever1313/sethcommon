#These functions and constants are used throughout all SConstructs
from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *
from Globals import *
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
    
globalDebugDefines = ["-DDEBUG"]
globalReleaseDefines = ["-DRELEASE"]
globalUnitTestDefines = ["-DUNIT_TEST"]


#Compile Flags
globalCXXFlags = ["-pedantic-errors", "-Werror", "-std=gnu++11", "-Wall", "-Wdouble-promotion", "-Wclobbered", "-Wcast-align", "-Wsign-compare", "-Wempty-body", "-Wcast-qual", "-Wmissing-field-initializers", "-Wtype-limits", "-fstack-protector-all"]
globalCXXDebugFlags = ["-g", "-Wswitch-enum"]
globalCXXReleaseFlags = ["-O3", "-Wswitch-enum", "-fdata-sections", "-ffunction-sections", "-s"]
globalCXXUnitTestFlags = ["-g", "-fprofile-arcs", "-ftest-coverage"]

if (sys.platform == "win32"):
    globalCXXFlags += ["-pthread"]
else:
    globalCXXFlags += ["-pthread"]

#Linker Flags
globalLinkerFlags = ["-Wall", "-Werror", "-std=gnu++11"]
releaseLinkerFlags = ["-Wl,--gc-sections", "-Wl,--strip-all"]

if(sys.platform == "win32"):
    globalLinkerFlags += ["-static", "-pthread"]
else:
    globalLinkerFlags += ["-pthread"]

#Libs
globalLibsDebug = []
globalLibsRelease = []
globalLibsUnitTest = ["gcov", "gtest", "gmock", "boost_unit_test_framework", "boost_system"]
if (sys.platform == "win32"):
    globalLibsDebug += ["ssp", "debug_new"] #Lib SSP is still needed in mingw, but not linux
    globalLibsRelease += ["ssp"]
    globalLibsUnitTest += ["ssp", "debug_new"]

    
#Parses arguments
def parseArguments(args):
    serverBuild = (args.get('server_build', '0') == '1')
    armBuild = (args.get('arm_build', '0') == '1')
    if (armBuild):
        global armPrefix
        global binDir
        binDir += armPrefix
        global libDir
        libDir += armPrefix
        global objectDir
        objectDir += armPrefix
    return (armBuild, serverBuild)
    
###
# Environments
###

#isArm is a boolean about whether or not to use the ARM compiler
def createBaseEnvironment (rootDir, isArm, serverBuild):
    if (sys.platform == "win32"):
        env = Environment(            
            tools = ["mingw"],
            ARM = False,
            SERVER_BUILD = False
        )
    elif (not isArm):
        if (serverBuild):
            env = Environment(
                ARM = False,
                CC = "gcc-4.8",
                CXX = "g++-4.8",
                LINK = "g++-4.8",
                SERVER_BUILD = True
            )
        else:
            env = Environment(
                tools = ["default", "gcc", "g++"],
                ARM = False,
                SERVER_BUILD = False
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
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, debugDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, debugDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, debugDir))
    )
    if (envBase['SERVER_BUILD']):
        debugEnvironment.Append(CPPPATH = ['/skyvo/include'])
        debugEnvironment.Append(LIBPATH = ['/skyvo/lib'])
    return debugEnvironment
    
def createReleaseEnvironment(envBase, includePaths, libs, libPath):
    releaseEnvironment = envBase.Clone(
        CPPDEFINES = globalDefines + globalReleaseDefines,
        CPPPATH = includePaths,
        CCFLAGS = globalCXXFlags + globalCXXReleaseFlags,
        LIBS = libs + globalLibsRelease, 
        LIBPATH = libPath,
        LINKFLAGS = globalLinkerFlags + releaseLinkerFlags,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, releaseDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, releaseDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, releaseDir))
    )
    if (envBase['SERVER_BUILD']):
        releaseEnvironment.Append(CPPPATH = ['/skyvo/include'])
        releaseEnvironment.Append(LIBPATH = ['/skyvo/lib'])
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
    if (envBase['SERVER_BUILD']):
        testEnvironment.Append(CPPATH = ['/skyvo/include'])
        testEnvironment.Append(LIBPATH = ['/skyvo/lib'])
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

def getDateVersionDefine(baseDir, args):
    if (args.get('version', '0') == '0'):
        dateVersionString = getVersion(baseDir) + '+' + replaceSpacesWithUnderscores(getUserName())
    else:
        dateVersionString = args.get('version')
    return 'VERSION="\\"' + dateVersionString + '\\""'

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

    if (status == 0):
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
        gcovString = gcovCommand + " -r -s \"" + thisDir + "\" -o \"" + os.path.join(thisDir, objectDir, unitTestDir) + "\" "
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
