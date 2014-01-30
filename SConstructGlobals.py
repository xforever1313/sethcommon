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
    SCons.Defaults.DefaultEnvironment(tools = ['mingw'])

SetOption('num_jobs', cpu_count())  #Sets -j to the number of cores  This happens when you import this file

#TargetFlags
CREATE_LIB_TARGET = 1
DEBUG_RELEASE_LIB_TARGET = (2 << 0)
DOXYGEN_TARGET = (2 << 1)
CPP_CHECK_TARGET = (2 << 2)
TESTING_TARGET = (2 << 3)
EXE_TARGET = (2 << 4)

#Target Aliases
CREATE_LIB_ALIAS = "create_lib"
DEBUG_LIB_ALIAS = "debug_lib"
RELEASE_LIB_ALIAS = "release_lib"
DOXYGEN_ALIAS = "doxygen"
CPP_CHECK_ALIAS = "cpp_check"
UNIT_TEST_ALIAS = "unit_test"
RUN_TEST_ALIAS = "run_test"
DEBUG_ALIAS = "debug"
RELEASE_ALIAS = "release"

#Possible Args
CLANG_BUILD_ARG = "clang_build"
ARM_BUILD_ARG = "arm_build"
ASM_JS_ARG = "asmjs"

POSSIBLE_ARGS = {}
POSSIBLE_ARGS[CLANG_BUILD_ARG] = "Build with clang"
POSSIBLE_ARGS[ARM_BUILD_ARG] = "Target for arm platform.  Only good with ipads right now"
POSSIBLE_ARGS[ASM_JS_ARG] = "\tBuild with emscripten"

def addPossibleTargets(env, targetFlags):
    if ((CREATE_LIB_TARGET & targetFlags) == CREATE_LIB_TARGET):
        env['POSSIBLE_TARGETS'][CREATE_LIB_ALIAS] = "Creates all the libraries"

    if ((DEBUG_RELEASE_LIB_TARGET & targetFlags) == DEBUG_RELEASE_LIB_TARGET):
        env['POSSIBLE_TARGETS'][DEBUG_LIB_ALIAS] = "Creates the debug libary"
        env['POSSIBLE_TARGETS'][RELEASE_LIB_ALIAS] = "Creates the release library"

    if ((DOXYGEN_TARGET & targetFlags) == DOXYGEN_TARGET):
        env['POSSIBLE_TARGETS'][DOXYGEN_ALIAS] = "\tCreates the doxygen documentation"

    if ((CPP_CHECK_TARGET & targetFlags) == CPP_CHECK_TARGET):
        env['POSSIBLE_TARGETS'][CPP_CHECK_ALIAS] = "Runs CPP Check"

    if ((TESTING_TARGET & targetFlags) == TESTING_TARGET):
        env['POSSIBLE_TARGETS'][UNIT_TEST_ALIAS] = "Builds unit test binary"
        env['POSSIBLE_TARGETS'][RUN_TEST_ALIAS] = "Builds and runs the unit test binary"

    if ((EXE_TARGET & targetFlags) == EXE_TARGET):
        env['POSSIBLE_TARGETS'][DEBUG_ALIAS] = "\tBuilds debug executable"
        env['POSSIBLE_TARGETS'][RELEASE_ALIAS] = "\tBuilds release executable "

###
# Environments
###

envClass = None

def serverBuildAdd(env):
    if (env['SERVER_BUILD']):
        env.Append(CPPPATH = ['/skyvo/include'])
        env.Append(LIBPATH = ['/skyvo/lib'])
        env.Append(CCFLAGS = ['-isystem', '/skyvo/include'])          

def createBaseEnvironment (rootDir, skyvoCommonPath, projectName, targetFlags, args):
    serverBuild = (args.get('server_build', '0') == '1')
    clangBuild = (args.get(CLANG_BUILD_ARG, '0') == '1')
    armBuild = (args.get(ARM_BUILD_ARG, '0') == '1')
    asmJSBuild = (args.get(ASM_JS_ARG, '0') == '1')
    msvcTarget = (args.get('msvc_target', None))

    sys.path.append(os.path.join(skyvoCommonPath, "build/environments"))

    global envClass

    if (asmJSBuild):
        from EmscriptenCompilerGlobals import EmscriptenCompilerGlobals
        envClass = EmscriptenCompilerGlobals()
    elif (clangBuild):
        from ClangCompilerGlobals import ClangCompilerGlobals
        envClass = ClangCompilerGlobals()
    elif (msvcTarget != None):
        from MSVCCompilerGlobals import MSVCCompilerGlobals
        envClass = MSVCCompilerGlobals()
    else:
        from GCCCompilerGlobals import GCCCompilerGlobals
        envClass = GCCCompilerGlobals()

    env = envClass.getBaseEnvironment(armBuild, serverBuild)

    baseEnvironment = env.Clone(
        PROJECT_ROOT = os.path.abspath("."),
        POSSIBLE_TARGETS = {},
        PROJECT_NAME = projectName
    )
    addPossibleTargets(baseEnvironment, targetFlags)
    baseEnvironment['TARGET_FLAGS'] = targetFlags
    baseEnvironment['ENV']['PATH'] = os.environ['PATH'] #Look in path for tools

    baseEnvironment['BASE_DIR'] = os.path.abspath(rootDir)
    baseEnvironment['COMMON_DIR'] = os.path.abspath(skyvoCommonPath)
    return baseEnvironment
    
def createDebugEnvironment(envBase, includePaths, libs, libPath):
    debugEnvironment = envBase.Clone(
        CPPPATH = includePaths,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, envBase['SYSTEM'], debugDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, envBase['SYSTEM'], debugDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, envBase['SYSTEM'], debugDir))
    )

    envClass.extendDebugEnvironment(debugEnvironment, libs, libPath)

    if (envBase['SERVER_BUILD']):
        serverBuildAdd(debugEnvironment)

    return debugEnvironment
    
def createReleaseEnvironment(envBase, includePaths, libs, libPath):
    releaseEnvironment = envBase.Clone(
        CPPPATH = includePaths,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, envBase['SYSTEM'], releaseDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, envBase['SYSTEM'], releaseDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, envBase['SYSTEM'], releaseDir))
    )

    envClass.extendReleaseEnvironment(releaseEnvironment, libs, libPath)
    
    if (envBase['SERVER_BUILD']):
        serverBuildAdd(releaseEnvironment)

    return releaseEnvironment

def createUnitTestEnvironment(envBase, includePaths, libs, libPath):
    testEnvironment = envBase.Clone(
        CPPPATH = includePaths,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, envBase['SYSTEM'], unitTestDir)) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, envBase['SYSTEM'], unitTestDir)),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, envBase['SYSTEM'], unitTestDir))
    )

    envClass.extendUnitTestEnvironment(testEnvironment, libs, libPath)

    unitTestIncludePaths = [os.path.join(getCppUTestPath(envBase['COMMON_DIR']), includeDir), \
                            os.path.join(getGMockPath(envBase['COMMON_DIR']), includeDir)]

    if (envBase['MSVC_BUILD']):
        testEnvironment.Append(CPPPATH = unitTestIncludePaths)
    else:
        for p in unitTestIncludePaths:
            testEnvironment.Append(CCFLAGS = ['-isystem', p])

    unitTestLibPaths = [os.path.join(getCppUTestPath(envBase['COMMON_DIR']), libDir, testEnvironment['SYSTEM']), \
                        os.path.join(getGMockPath(envBase['COMMON_DIR']), libDir, testEnvironment['SYSTEM'])]

    testEnvironment.Append(LIBPATH = unitTestLibPaths)

    if (envBase['SERVER_BUILD']):
        serverBuildAdd(testEnvironment)

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

def getHelpMessage(env):
    ret = '\nSConstruct file for ''' + env['PROJECT_NAME']
    ret += ':\n\nTargets:\n'
    
    for target in sorted(env['POSSIBLE_TARGETS'].keys()):
        ret += '\t' + target + '\t' + env['POSSIBLE_TARGETS'][target]
        ret += '\n'

    ret += '\nArguments:\n'

    for arg in sorted(POSSIBLE_ARGS.keys()):
        ret += '\t' + arg + '=1\t' + POSSIBLE_ARGS[arg]
        ret += '\n'
    
    ret += '\n'
    return ret

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
    if (env['ASM_JS_BUILD']):
        status = subprocess.call("node unit_test.js", cwd=cwdDir, shell=True)
    elif (sys.platform == "win32"):
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
            
    return status

def generateNetBeansFiles(projectName, env, possibleTargets, additionalArgs, sourceFiles):
    b = Builder(action = generateNetBeansFileBuilder)
    env.Append(BUILDERS = {"net_beans" : b})

def generateNetBeansFilesBuilder(target, source, env):
    pass

    
