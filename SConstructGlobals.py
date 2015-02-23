#These functions and constants are used throughout all SConstructs
from SCons.Script import *
from SCons.Environment import *
from SCons.Builder import *
from DoxygenGlobals import *
from Globals import *
from SethCommonGlobals import *
from multiprocessing import cpu_count
from operator import itemgetter
import glob
import os
import shutil
import string
import subprocess
import sys
import time
import re

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
PROJECT_TARGET = (2 << 5)
PACKAGE_TARGET = (2 << 6)

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
RUN_DEBUG_ALIAS = "run_debug"
RUN_RELEASE_ALIAS = "run_release"
NET_BEANS_ALIAS = "net_beans"
VS_ALIAS = "vs_project"
CODEBLOCKS_ALIAS = "codeblocks"
ARCH_PACKAGE_ALIAS = "pkgbuild"

DELTA_ALIAS = "delta"
NIGHTLY_ALIAS = "nightly"

#Possible Args
DEFAULT = ""
CLANG_BUILD_ARG = "clang_build"
PI_BUILD_ARG = "pi_build"
ARM_BUILD_ARG = "arm_build"
ASM_JS_ARG = "asmjs"
GDB_RUN_ARG = "gdb"
VALGRIND_RUN_ARG = "valgrind"
MINGW_BUILD_ARG = "mingw"
SHARED_ARG = "shared"

POSSIBLE_ARGS = {}
POSSIBLE_ARGS[CLANG_BUILD_ARG] = "Build with clang"
POSSIBLE_ARGS[ARM_BUILD_ARG] = "Target for arm platform.  Only good with ipads right now"
POSSIBLE_ARGS[ASM_JS_ARG] = "\tBuild with emscripten"
POSSIBLE_ARGS[GDB_RUN_ARG] = "\tRuns gdb with an executable (run_xxx targets only)"
POSSIBLE_ARGS[VALGRIND_RUN_ARG] = "Runs valgrind with an executable (run_xxx and Linux only)"
POSSIBLE_ARGS[MINGW_BUILD_ARG] = "Builds using x86_64-w64-mingw32 (Linux only)"
POSSIBLE_ARGS[SHARED_ARG] = "Builds using shared libs instead of static"
POSSIBLE_ARGS[PI_BUILD_ARG] = "Builds using the Raspberry Pi Compiler (Linux only)"

compilerTypeArgs = {}
compilerTypeArgs[DEFAULT] = ""
compilerTypeArgs[CLANG_BUILD_ARG] = "clang_"
compilerTypeArgs[ASM_JS_ARG] = "asm_js_"
compilerTypeArgs[PI_BUILD_ARG] = "pi_"

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
        env['POSSIBLE_TARGETS'][RELEASE_ALIAS] = "\tBuilds release executable"
        env['POSSIBLE_TARGETS'][RUN_DEBUG_ALIAS] = "Runs the debug executable"
        env['POSSIBLE_TARGETS'][RUN_RELEASE_ALIAS] = "Runs the release executable"

    if ((PROJECT_TARGET & targetFlags) == PROJECT_TARGET):
        env['POSSIBLE_TARGETS'][NET_BEANS_ALIAS] = "Builds a netbeans project"
        env['POSSIBLE_TARGETS'][VS_ALIAS] = "Builds a visual studio project"
        env['POSSIBLE_TARGETS'][CODEBLOCKS_ALIAS] = "Builds a Code::Blocks Project"

    if ((PACKAGE_TARGET & targetFlags) == PACKAGE_TARGET):
        env['POSSIBLE_TARGETS'][ARCH_PACKAGE_ALIAS] = "Generates a PKGBUILD file"

###
# Environments
###

envClass = None

def serverBuildAdd(env):
    if (env['SERVER_BUILD']):
        #TODO when we have CI up and running
        pass

def addDebugNewLibPath(env):
    if (not env['ASM_JS_BUILD']):
        env.Append(LIBPATH = os.path.join(getDebugNewPath(env['COMMON_DIR']), libDir, env['SYSTEM'], env['LIB_TYPE']))
        env['ENV']['GLIBCXX_FORCE_NEW'] = '1'

def createBaseEnvironment (rootDir, sethCommonPath, projectName, targetFlags, args):
    serverBuild = (args.get('server_build', '0') == '1')
    clangBuild = (args.get(CLANG_BUILD_ARG, '0') == '1')
    armBuild = (args.get(ARM_BUILD_ARG, '0') == '1')
    asmJSBuild = (args.get(ASM_JS_ARG, '0') == '1')
    msvcTarget = (args.get('msvc_target', None))
    valgrindRun = (args.get(VALGRIND_RUN_ARG, '0') == '1')
    gdbRun = (args.get(GDB_RUN_ARG, '0') == '1')
    sharedBuild = (args.get(SHARED_ARG, '0') == '1')
    
    if (sys.platform == "win32"):
        mingwBuild = False
        piBuild = False
    else:
        mingwBuild = (args.get(MINGW_BUILD_ARG, '0') == '1')
        piBuild = (args.get(PI_BUILD_ARG, '0') == '1')
 
    if (gdbRun and valgrindRun):
        raise Exception("You can not have both gdb and valgrind set to 1")
    elif ((gdbRun or valgrindRun) and asmJSBuild):
        raise Exception("Can not run gdb or valgrind with emscripten builds")

    sys.path.append(os.path.join(sethCommonPath, "build/environments"))

    global envClass

    if (asmJSBuild):
        from EmscriptenCompilerGlobals import EmscriptenCompilerGlobals
        envClass = EmscriptenCompilerGlobals()
    elif (clangBuild):
        from ClangCompilerGlobals import ClangCompilerGlobals
        envClass = ClangCompilerGlobals()
    elif (msvcTarget != None):
        from MSVCCompilerGlobals import MSVCCompilerGlobals
        envClass = MSVCCompilerGlobals(msvcTarget)
    elif(piBuild):
        from ArmLinuxGnueabihfGlobals import ArmLinuxGnueabihfGlobals 
        envClass = ArmLinuxGnueabihfGlobals()
    else:
        from GCCCompilerGlobals import GCCCompilerGlobals
        envClass = GCCCompilerGlobals()

    env = envClass.getBaseEnvironment(armBuild, serverBuild, mingwBuild)

    baseEnvironment = env.Clone(
        PROJECT_ROOT = os.path.abspath("."),
        POSSIBLE_TARGETS = {},
        PROJECT_NAME = projectName,
        VALGRIND_RUN = valgrindRun,
        GDB_RUN = gdbRun,
        SHARED_BUILD = sharedBuild
    )

    if (sharedBuild):
        baseEnvironment['LIB_TYPE'] = sharedLibType
    else:
        baseEnvironment['LIB_TYPE'] = staticLibType

    addPossibleTargets(baseEnvironment, targetFlags)
    baseEnvironment['TARGET_FLAGS'] = targetFlags
    baseEnvironment['ENV']['PATH'] = os.environ['PATH'] #Look in path for tools
    baseEnvironment['ENV']['GLIBCXX_FORCE_NEW'] = 1

    baseEnvironment['BASE_DIR'] = os.path.abspath(rootDir)
    baseEnvironment['COMMON_DIR'] = os.path.abspath(sethCommonPath)

    return baseEnvironment
    
def createDebugEnvironment(envBase, includePaths, libs, libPath):
    debugEnvironment = envBase.Clone(
        CPPPATH = includePaths,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, envBase['SYSTEM'], debugDir, envBase['LIB_TYPE'])) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, envBase['SYSTEM'], debugDir, envBase['LIB_TYPE'])),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, envBase['SYSTEM'], debugDir, envBase['LIB_TYPE'])),
        BUILD_TARGET = debugDir
    )

    envClass.extendDebugEnvironment(debugEnvironment, libs, libPath)

    if (envBase['SERVER_BUILD']):
        serverBuildAdd(debugEnvironment)

    addDebugNewLibPath(debugEnvironment)

    return debugEnvironment
    
def createReleaseEnvironment(envBase, includePaths, libs, libPath):
    releaseEnvironment = envBase.Clone(
        CPPPATH = includePaths,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, envBase['SYSTEM'], releaseDir, envBase['LIB_TYPE'])) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, envBase['SYSTEM'], releaseDir, envBase['LIB_TYPE'])),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, envBase['SYSTEM'], releaseDir, envBase['LIB_TYPE'])),
        BUILD_TARGET = releaseDir
    )

    envClass.extendReleaseEnvironment(releaseEnvironment, libs, libPath)
    
    if (envBase['SERVER_BUILD']):
        serverBuildAdd(releaseEnvironment)

    return releaseEnvironment

def createUnitTestEnvironment(envBase, includePaths, libs, libPath):
    testEnvironment = envBase.Clone(
        CPPPATH = includePaths,
        OBJPREFIX = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], objectDir, envBase['SYSTEM'], unitTestDir, envBase['LIB_TYPE'])) + '/',
        LIBDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], libDir, envBase['SYSTEM'], unitTestDir, envBase['LIB_TYPE'])),
        BINDIR = os.path.abspath(os.path.join(envBase['PROJECT_ROOT'], binDir, envBase['SYSTEM'], unitTestDir, envBase['LIB_TYPE'])),
        BUILD_TARGET = unitTestDir
    )

    envClass.extendUnitTestEnvironment(testEnvironment, libs, libPath)

    unitTestIncludePaths = [os.path.join(getCppUTestPath(envBase['COMMON_DIR']), includeDir), \
                            os.path.join(getGMockPath(envBase['COMMON_DIR']), includeDir)]

    if (envBase['MSVC_BUILD']):
        testEnvironment.Append(CPPPATH = unitTestIncludePaths)
    else:
        for p in unitTestIncludePaths:
            testEnvironment.Append(CCFLAGS = ['-isystem', p])

    unitTestLibPaths = [os.path.join(getCppUTestPath(envBase['COMMON_DIR']), libDir, testEnvironment['SYSTEM'], testEnvironment['LIB_TYPE']), \
                        os.path.join(getGMockPath(envBase['COMMON_DIR']), libDir, testEnvironment['SYSTEM'], testEnvironment['LIB_TYPE'])]

    testEnvironment.Append(LIBPATH = unitTestLibPaths)

    if (envBase['SERVER_BUILD']):
        serverBuildAdd(testEnvironment)

    addDebugNewLibPath(testEnvironment)

    RunTest = Builder(action = testRunner)
    testEnvironment.Append(BUILDERS = {"Test" : RunTest})

    return testEnvironment

###
# Create targets
###
def addGDBAndValgrindCommand(command, env):
    if (env['GDB_RUN']):
        commandList = ["gdb", command]
    elif(env['VALGRIND_RUN']):
        commandList = ["valgrind", "--leak-check=full", command]
    else:
        commandList = [command]
    return commandList

def createRunTarget(target, source, env):
    ret = None
    if (env['ASM_JS_BUILD']):
        ret = subprocess.call(["emrun", env['EXE'] + ".html", "--port=9003"], cwd = env['BINDIR'])
    elif (env['MINGW_CROSS_BUILD']):
        ret = subprocess.call(["wine",  env['EXE']], cwd = env['BINDIR'])
    else:
        ret = subprocess.call(addGDBAndValgrindCommand(env['EXE'], env), cwd = env['BINDIR'])
    
    return ret
    

def addSharedLibPaths(env):
    if ((sys.platform == "win32") or env['MINGW_CROSS_BUILD']):
        for libPath in env['LIBPATH']:
            os.environ['PATH'] += ';' + libPath
    else:
        try:
            dummy = os.environ['LD_LIBRARY_PATH']
        except KeyError:
            os.environ['LD_LIBRARY_PATH'] = ""

        for libPath in env['LIBPATH']:
            os.environ['LD_LIBRARY_PATH'] += ":" + libPath

def createExe(env, exeName, sourceFiles):
    exe = os.path.join(env['BINDIR'], exeName)
    exeTarget = env.Program(target = exe, source = sourceFiles)

    runEnvironment = env.Clone(EXE = exe)
    run = Builder(action = createRunTarget)
    runEnvironment.Append(BUILDERS = {"Run" : run})
    
    #Funky phony name so there's no multiple targets
    runTarget = runEnvironment.Run(target = os.path.join(env['BINDIR'], "phonyRunTarget.target"),
                                   source = [])

    Depends(runTarget, exeTarget)

    if (env['SHARED_BUILD']):
        addSharedLibPaths(env)

    Clean(exeTarget, [os.path.join(env['BINDIR'])])
    return (exeTarget, runTarget)
    
def createUnitTestExe(env, sourceFiles, coverageFiles):
    (testExeTarget, ignoreThis) = createExe(env, "unit_test", sourceFiles)
    runTestTarget = env.Test(target = profilingDataFile, source = coverageFiles)
    AlwaysBuild(runTestTarget)
    Execute(Delete(Glob(os.path.join(testOutputDir, '*')))) #Remove old test outputs    
    Depends(runTestTarget, testExeTarget)
    return (testExeTarget, runTestTarget)

def createStaticLib(env, libName, sourceFiles):
    staticLibTarget = env.StaticLibrary(target = os.path.join(env['LIBDIR'], libName), source = sourceFiles)
    return staticLibTarget

def createSharedLib(env, libName, sourceFiles):
    if (((sys.platform == "win32") or env['MINGW_CROSS_BUILD']) and not env['MSVC_BUILD']):
        env['LIBSUFFIX'] = ".dll.a"

    try:
        env['LIBS'].remove("debug_new") #Do not want debug new getting linked in to .dll
    except:
        pass #May not be in all environments, so this may throw a "not found" exception

    sharedLibTarget = env.SharedLibrary(target = os.path.join(env['LIBDIR'], libName), source = sourceFiles)
    return sharedLibTarget

def createLib(env, sourceFiles, libName):
    if (env['SHARED_BUILD']):
        return createSharedLib(env, sourceFiles, libName)
    else:
        return createStaticLib(env, sourceFiles, libName)

def createDoxygenTarget(env, includePaths, projectBrief):
    doxygenEnvironment = env.Clone(
        CPPPATH = includePaths, 
        PROJECT_DESCRIPTION = projectBrief
    )

    doxygenFiles =  getFilesInPathRecursivelyWithExtension(includeDir, '.h')
    doxygenFiles += getFilesInPathRecursivelyWithExtension(srcDir, '.cpp')
    doxygenFiles += getFilesInPathRecursivelyWithExtension(testDir, '.h') #Get mocks, and only mocks
    doxygenFiles += [os.path.join(env['COMMON_DIR'], 'DoxygenGlobals.py'),
                     os.path.join(env['COMMON_DIR'], 'SConstructGlobals.py')]

    Doxygen = Builder(action = doxgyenBuilder)
    doxygenEnvironment.Append(BUILDERS = {"Doxygen" : Doxygen})
    createDir(docDir)
    target = doxygenEnvironment.Doxygen(target = [doxyFile, doxygenData], source = doxygenFiles)
    Clean(target, os.path.join(env['PROJECT_ROOT'], doxygenDir))
    return target

def createCppCheckTarget(env, sources):
    cppCheck = Builder(action = cppCheckBuilder)
    env.Append(BUILDERS = {"cppCheck" : cppCheck})
    target = env.cppCheck(target = cppCheckData, source = sources)
    return target

def getCompiledObjectsWithDateVersionObject(env, sources, dateVersionFile, args):
    if (env['SHARED_BUILD'] and not env['MINGW_CROSS_BUILD']):
        compiledObjects = env.SharedObject(sources)
    else:
        compiledObjects = env.Object(sources)
    
    dateVersionEnvironment = env.Clone()
    dateVersionEnvironment.Append(CPPDEFINES = getDateVersionDefine(env['BASE_DIR'], args))

    if (env['SHARED_BUILD'] and not env['MINGW_CROSS_BUILD']):
        dateVersionObject = dateVersionEnvironment.SharedObject(dateVersionFile)
    else:
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

#Source is only passed in so we know if we need to rebuild if any sources change 
def doxgyenBuilder(target, source, env): 
    print "Generating Doxyfile..."
    generateDoxyFile(env['PROJECT_NAME'], env['PROJECT_DESCRIPTION'], env['CPPPATH'], str(target[0]))
    print "Running Doxygen..."
    status = subprocess.call("doxygen Doxyfile " + getRedirectString(str(target[1])), shell=True)
    if (status != 0):
        sys.stderr.write("** Error with doxygen! Please refer to " + os.path.abspath(str(target[0])) + " for more information **\n")
    return status

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
        sys.stderr.write("** CPP Check detected errors.  Please refer to " + os.path.abspath(str(target[0])) + " for more details **\n")
    return status
   
#Runs the test, and coverage while we are at it :P
def testRunner(target, source, env):
    Execute(Delete(Glob(os.path.join(env['OBJPREFIX'], '*.gcda')))) #remove old .gcna files.

    thisDir = os.getcwd()
    cwdDir = env['BINDIR']
    if (env['ASM_JS_BUILD']):
        status = subprocess.call(["node" , "unit_test.js"], cwd=cwdDir)
    elif (env['MINGW_CROSS_BUILD']):
        status = subprocess.call(["wine", "./unit_test"], cwd=cwdDir)
    elif (sys.platform == "win32"):
        status = subprocess.call(addGDBAndValgrindCommand(os.path.join(cwdDir, "unit_test.exe"), env), cwd=cwdDir)
    else:
        status = subprocess.call(addGDBAndValgrindCommand("./unit_test", env), cwd=cwdDir)

    if ((status == 0) and not env['CLANG_BUILD'] and not env['MSVC_BUILD'] and not env['MINGW_CROSS_BUILD']):
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
        gcovString = gcovCommand + " -r -s \"" + thisDir + "\" -o \"" + \
                     os.path.join(thisDir, objectDir, env['SYSTEM'], unitTestDir, env['LIB_TYPE']) + "\" "
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

###
# Project Generation
###
def createProjectTargets(env, includePath, exeName):
    netBeansTarget = generateNetBeansFiles(env, includePath, exeName)
    Alias(NET_BEANS_ALIAS, netBeansTarget)

    vsTarget = generateVSFiles(env, includePath, exeName)
    Alias(VS_ALIAS, vsTarget)

    codeBlocksTarget = generateCodeBlocksFiles(env, includePath, exeName)
    Alias(CODEBLOCKS_ALIAS, codeBlocksTarget)

    return (netBeansTarget, vsTarget, codeBlocksTarget)

###
# Visual Studio project generation
###
def generateVSFiles(env, includePath, exeName):
    VSEnv = env.Clone(CPPPATH = includePath,
                      EXE_NAME = exeName)

    b = Builder(action = generateVSFilesBuilder)
    VSEnv.Append(BUILDERS = {"vs" : b} )

    projectFolder = "vsproject"

    sources = ["SConstruct", os.path.join(env['COMMON_DIR'], 'SConstructGlobals.py')]
    targets = [os.path.join(projectFolder, VSEnv['PROJECT_NAME'] + '.vcxproj'), 
              os.path.join(projectFolder, VSEnv['PROJECT_NAME'] + '.vcxproj.filters')]

    ret = VSEnv.vs(target = targets, source = sources)
    Clean(ret, projectFolder)
    return ret

def generateVSFilters(includeFiles, sourceFiles, testIncludeFiles, testSourceFiles, testFiles):
    cwd = os.getcwd()
    first = cwd.rfind(os.sep) + 1
    last = len(cwd)
    project = cwd[first:last]
    ret = '<?xml version="1.0" encoding="utf-8"?>'
    ret += '<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">'
    ret += '<ItemGroup>'

    filePaths = getDirectoryStructure(srcDir)
    for d in filePaths:
        ret += '<Filter Include="' + d + '">\n'
        ret += '</Filter>\n'

    filePaths = getDirectoryStructure(includeDir)
    for d in filePaths:
        ret += '<Filter Include="' + d + '">\n'
        ret += '</Filter>\n'
        
    filePaths = getDirectoryStructure(testDir)
    for d in filePaths:
        ret += '<Filter Include="' + d + '">\n'
        ret += '</Filter>\n'
    ret += '</ItemGroup>\n'
    
    ret += '<ItemGroup>\n'
    for f in includeFiles:
        ret += '<ClInclude Include="' + os.path.abspath(f) + '">\n'
        ret += '<Filter>' + os.path.dirname(f) + '</Filter>\n'
        ret += '</ClInclude>\n'
    ret += '</ItemGroup>\n'

    ret += '<ItemGroup>\n'
    for f in sourceFiles:
        ret += '<ClCompile Include="' + os.path.abspath(f) + '">\n'
        ret += '<Filter>' + os.path.dirname(f) + '</Filter>\n'
        ret += '</ClCompile>\n'
    ret += '</ItemGroup>\n'

    ret += '<ItemGroup>\n'
    for f in testIncludeFiles:
        ret += '<ClInclude Include="' + os.path.abspath(f) + '">\n'
        ret += '<Filter>' + os.path.dirname(f) + '</Filter>\n'
        ret += '</ClInclude>\n'
    ret += '</ItemGroup>\n'

    ret += '<ItemGroup>\n'
    for f in testSourceFiles:
        ret += '<ClCompile Include="' + os.path.abspath(f) + '">\n'
        ret += '<Filter>' + os.path.dirname(f) + '</Filter>\n'
        ret += '</ClCompile>\n'
    ret += '</ItemGroup>\n'

    ret += '<ItemGroup>\n'
    for f in testFiles:
        ret += '<ClCompile Include="' + os.path.abspath(f) + '">\n'
        ret += '<Filter>' + os.path.dirname(f) + '</Filter>\n'
        ret += '</ClCompile>\n'
    ret += '</ItemGroup>\n'

    ret += '</Project>'

    return ret

def getVSProjectXML(env, includeFiles, sourceFiles, testIncludeFiles, testSourceFiles, testFiles):
    ret = '''<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="12.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">    
    '''
    targetKeys = env['POSSIBLE_TARGETS'].keys()
    targetKeys.remove(NET_BEANS_ALIAS)
    targetKeys.remove(VS_ALIAS)
    targetKeys.remove(CODEBLOCKS_ALIAS)

    for compiler in sorted(compilerTypeArgs):
        for target in sorted(targetKeys):
            targetName = compilerTypeArgs[compiler] + target
            ret += '<ProjectConfiguration Include ="' + targetName + '|win32">\n'
            ret += '<Configuration>' + targetName + '</Configuration>\n'
            ret += '<Platform>win32</Platform>\n'
            ret += '</ProjectConfiguration>\n'

    ret += '</ItemGroup>\n'
    ret += '''  <PropertyGroup Label="Globals">
    <Keyword>MakeFileProj</Keyword>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  '''
    for compiler in sorted(compilerTypeArgs):
        for target in sorted(targetKeys):
            targetName = compilerTypeArgs[compiler] + target
            ret += '  <PropertyGroup Condition="\'$(Configuration)|$(Platform)\'==\'' + targetName + '|win32\'" Label="Configuration">\n'
            ret += '''
    <ConfigurationType>Makefile</ConfigurationType>
    <PlatformToolset>v120</PlatformToolset>
  </PropertyGroup>
            '''
  
    ret += '''
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
    '''

    for compiler in sorted(compilerTypeArgs):
        for target in sorted(targetKeys):
            targetName = compilerTypeArgs[compiler] + target
             
            ret += '<ImportGroup Label="PropertySheets" Condition="\'$(Configuration)|$(Platform)\'==\'' + targetName + '|win32\'">\n'
            ret += '<Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists(\'$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\')" Label="LocalAppDataPlatform" />\n'
            ret += '</ImportGroup>\n'

    ret += '<PropertyGroup Label="UserMacros" />\n'
    for compiler in sorted(compilerTypeArgs):
        for target in sorted(targetKeys):
            targetName = compilerTypeArgs[compiler] + target

            ret += '<PropertyGroup Condition="\'$(Configuration)|$(Platform)\'==\'' + targetName + '|win32\'">\n'
            
            if (compiler != ''):
                command = 'cd .. &amp; scons ' + target + ' ' + compiler + '=1'
            else:
                command = 'cd .. &amp; scons ' + target
                
            ret += '<NMakeBuildCommandLine>' + command + '</NMakeBuildCommandLine>\n'
            ret += '<NMakeCleanCommandLine>' + command + ' --clean</NMakeCleanCommandLine>'
            
            ret += '<NMakeReBuildCommandLine>' + command + '--clean; &amp; ' + command + '</NMakeReBuildCommandLine>\n'

            if (target == DEBUG_ALIAS):
                ret += '<NMakeOutput>' + os.path.join('..', binDir, env['SYSTEM'], DEBUG_ALIAS, env['LIB_TYPE'], env['EXE_NAME'] + '-d') + '</NMakeOutput>\n'
            elif(target == RELEASE_ALIAS):
                ret += '<NMakeOutput>' + os.path.join('..', binDir, env['SYSTEM'], RELEASE_ALIAS, env['LIB_TYPE'], env['EXE_NAME']) + '</NMakeOutput>\n'
            elif(target == UNIT_TEST_ALIAS):
                ret += '<NMakeOutput>' + os.path.join('..', binDir, env['SYSTEM'], UNIT_TEST_ALIAS, env['LIB_TYPE'], 'unit_test') + '</NMakeOutput>\n'
            else:
                ret += '<NMakeOutput></NMakeOutput>\n'

            ret += '<IncludePath>$(VC_IncludePath);$(WindowsSDK_IncludePath);'
            for path in env['CPPPATH']:
                ret += path + ';'
            ret += '</IncludePath>\n'

            ret += '<OutDir>.</OutDir>\n'
            ret += '<IntDir>.</IntDir>\n'
            ret += '<NMakeIncludeSearchPath>'
            for path in env['CPPPATH']:
                ret += os.path.abspath(path) + ';'
            ret += '</NMakeIncludeSearchPath>\n'

            ret += '</PropertyGroup>\n'

    ret += '<ItemDefinitionGroup>\n</ItemDefinitionGroup>\n<ItemGroup>\n'
    
    for f in includeFiles:
        ret += '<ClInclude Include="' + os.path.abspath(f) + '" />\n'
    ret += '</ItemGroup>\n'

    ret += '<ItemGroup>\n'
    for f in sourceFiles:
        ret += '<ClCompile Include="' + os.path.abspath(f) + '" />\n'
    ret += '</ItemGroup>\n'

    ret += '<ItemGroup>\n'
    for f in testFiles:
        ret += '<ClCompile Include="' + os.path.abspath(f) + '" />\n'
    ret += '</ItemGroup>\n'

    ret += '<ItemGroup>\n'
    for f in testIncludeFiles:
        ret += '<ClInclude Include="' + os.path.abspath(f) + '" />\n'
    ret += '</ItemGroup>\n'

    ret += '<ItemGroup>\n'
    for f in testSourceFiles:
        ret += '<ClCompile Include="' + os.path.abspath(f) + '" />\n'
    ret += '</ItemGroup>\n'

    ret += '<ItemGroup>\n<None Include="' + os.path.abspath('SConstruct') + '" />\n</ItemGroup>\n'


    ret += '<Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />\n'
    ret += '<ImportGroup Label="ExtensionTargets">\n</ImportGroup>\n'
    ret += '</Project>'

    return ret

def generateVSFilesBuilder(target, source, env):
    includeFiles = getFilesInDirectory(includeDir)
    sourceFiles = getFilesInDirectory(srcDir)
    testIncludeFiles = getFilesInDirectory(os.path.join(testDir, includeDir))
    testSourceFiles = getFilesInDirectory(os.path.join(testDir, srcDir))
    testFiles = getFilesInDirectory(testDir)
    
    for f in testIncludeFiles:
        testFiles.remove(f)

    for f in testSourceFiles:
        testFiles.remove(f)

    projectXML = open(str(target[0]), "w")
    projectXML.write(getVSProjectXML(env, includeFiles, sourceFiles, testIncludeFiles, testSourceFiles, testFiles))
    projectXML.close()

    filtersXML = open(str(target[1]), "w")
    filtersXML.write(generateVSFilters(includeFiles, sourceFiles, testIncludeFiles, testSourceFiles, testFiles))
    filtersXML.close()

###
# Netbean Project Generation
###
def generateNetBeansFiles(env, includePath, exeName):

    netEnv = env.Clone(CPPPATH = includePath, 
                       EXE_NAME = exeName)

    b = Builder(action = generateNetBeansFilesBuilder)
    netEnv.Append(BUILDERS = {"net_beans" : b})
    
    projectFolder = "nbproject"

    sources = ["SConstruct", os.path.join(env['COMMON_DIR'], 'SConstructGlobals.py')]
    targets = [os.path.join(projectFolder, 'project.xml'), 
               os.path.join(projectFolder, 'configurations.xml')]

    ret = netEnv.net_beans(target = targets, source = [sources])
    Clean(ret, projectFolder)
    return ret

def getProjectXML(env):
    projectXML = '''<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://www.netbeans.org/ns/project/1">
    <type>org.netbeans.modules.cnd.makeproject</type>
    <configuration>
        <data xmlns="http://www.netbeans.org/ns/make-project/1">
            <name>''' + env['PROJECT_NAME'] + '''</name>
            <c-extensions>c</c-extensions>
            <cpp-extensions>cpp</cpp-extensions>
            <header-extensions>h</header-extensions>
            <sourceEncoding>UTF-8</sourceEncoding>
            <make-dep-projects/>
            <sourceRootList>
                <sourceRootElem>.</sourceRootElem>
            </sourceRootList>
            <confList>
    '''

    for target in sorted(env['POSSIBLE_TARGETS'].keys()):
        projectXML += '<confElem>\n<name>' + target + '</name>\n'
        projectXML += '<type>0</type>\n</confElem>\n'

    projectXML += '''
            </confList>
            <formatting>
                <project-formatting-style>false</project-formatting-style>
            </formatting>
        </data>
    </configuration>
</project>
    '''

    return projectXML

def getConfigurationsXml(env):
    configureXML = '''<?xml version="1.0" encoding="UTF-8"?>
<configurationDescriptor version="90">
  <logicalFolder name="root" displayName="root" projectFiles="true" kind="ROOT">
    <df root="." name="0">
    </df>
  </logicalFolder>
    <sourceFolderFilter>^(nbproject|obj|lib)$</sourceFolderFilter>
        <sourceRootList>
            <Elem>.</Elem>
        </sourceRootList>
    <projectmakefile>SConstruct</projectmakefile>
  <confs>
    '''
    targetKeys = env['POSSIBLE_TARGETS'].keys()
    targetKeys.remove(NET_BEANS_ALIAS)
    targetKeys.remove(VS_ALIAS)
    targetKeys.remove(CODEBLOCKS_ALIAS)

    for compiler in sorted(compilerTypeArgs):
        for target in sorted(targetKeys):    
            targetName = compilerTypeArgs[compiler] + target
            targetCommand = target
            if (compiler != ''):
                targetCommand += ' ' + compiler + '=1'
            if (env['SERVER_BUILD']):
                targetCommand += ' server_build=1'
            configureXML += '<conf name = "' + targetName + '" type = "0">'
            configureXML += '''
            <toolsSet>
        <compilerSet>default</compilerSet>
        <dependencyChecking>false</dependencyChecking>
        <rebuildPropChanged>false</rebuildPropChanged>
      </toolsSet>
      <codeAssistance>
      </codeAssistance>
      <makefileType>
        <makeTool>
          <buildCommandWorkingDir>.</buildCommandWorkingDir>
         '''
            configureXML += '<buildCommand>${MAKE} -f SConstruct '
            configureXML += targetCommand + '</buildCommand>\n'
            configureXML += '<cleanCommand>${MAKE} -f SConstruct '
            configureXML += targetCommand + ' --clean</cleanCommand>\n'
            if (target == DEBUG_ALIAS):
                configureXML += '<executablePath>' + os.path.join('bin', env['SYSTEM'], DEBUG_ALIAS, env['LIB_TYPE'], env['EXE_NAME'] + '-d') + '</executablePath>\n'
            elif(target == RELEASE_ALIAS):
                configureXML += '<executablePath>' + os.path.join('bin', env['SYSTEM'], RELEASE_ALIAS, env['LIB_TYPE'], env['EXE_NAME']) + '</executablePath>\n'
            elif(target == UNIT_TEST_ALIAS):
                configureXML += '<executablePath>' + os.path.join('bin', env['SYSTEM'], UNIT_TEST_ALIAS, env['LIB_TYPE'], 'unit_test') + '</executablePath>\n'
            else:
                configureXML += '<executablePath></executablePath>\n'


            configureXML += '\n</makeTool>\n</makefileType>\n'

            configureXML += '\n<cTool>\n<incDir>\n'
            for include in env['CPPPATH']:
                configureXML += '<pElem>' + include + '</pElem>\n'
            configureXML += '</incDir>\n</cTool>\n'

            configureXML += '\n<ccTool>\n<incDir>\n'
            for include in env['CPPPATH']:
                configureXML += '<pElem>' + include + '</pElem>\n'
            configureXML += '</incDir>\n</ccTool>\n'

            configureXML += '</conf>\n'
    
    configureXML += '</confs>\n</configurationDescriptor>'

    return configureXML

def generateNetBeansFilesBuilder(target, source, env):
    projectXML = open(str(target[0]), "w")
    projectXML.write(getProjectXML(env))
    projectXML.close()

    configureXML = open(str(target[1]), "w")
    configureXML.write(getConfigurationsXml(env))
    configureXML.close()

###
# Codeblocks Project Generation
###

def generateCBPFile(env, files):
    projectString = '''<?xml version = "1.0" encoding="UTF-8" standalone="yes" ?>
<CodeBlocks_project_file>
    <FileVersion major="1" minor="6" />
    <Project>
        <Option title = "''' + env['PROJECT_NAME'] + '''" />
        <Option makefile="SConstruct" />
        <Option makefile_is_custom = "1" />
        <Option pch_mode = "2" />
        <Option compiler = "gcc" />
        <MakeCommands>
            <Build command = "" />
            <CompileFile command = "" />
            <Clean command = "" />
            <DistClean command = "" />
            <AskRebuildNeeded command = "" />
            <SilentBuild command = "" />
        </MakeCommands>
        <Build>\n'''

    targetKeys = env['POSSIBLE_TARGETS'].keys()
    targetKeys.remove(NET_BEANS_ALIAS)
    targetKeys.remove(VS_ALIAS)
    targetKeys.remove(CODEBLOCKS_ALIAS)

    for compiler in sorted(compilerTypeArgs):
        for target in sorted(targetKeys):
            targetName = compilerTypeArgs[compiler] + target
            targetCommand = target
            if (compiler != ''):
                targetCommand += ' ' + compiler + '=1'
            if (env['SERVER_BUILD']):
                targetCommand += ' server_build=1'

            projectString += '<Target title = "' + targetName + '">\n'

            # Setup Executable paths
            if (target == DEBUG_ALIAS):
                exeDir = os.path.join(binDir, env['SYSTEM'], DEBUG_ALIAS, env['LIB_TYPE'])
                projectString += '    <Option output="' + os.path.join(exeDir, env['EXE_NAME'] + '-d') + '" prefix_auto="1" extension_auto="1" />\n'
                projectString += '    <Option working_dir="' + exeDir + '" />\n'

            elif (target == RELEASE_ALIAS):
                exeDir = os.path.join(binDir, env['SYSTEM'], RELEASE_ALIAS, env['LIB_TYPE'])
                projectString += '    <Option output="' + os.path.join(exeDir, env['EXE_NAME']) + '" prefix_auto="1" extension_auto="1" />\n'
                projectString += '    <Option working_dir="' + exeDir + '" />\n'

            elif (target == UNIT_TEST_ALIAS):
                exeDir = os.path.join(binDir, env['SYSTEM'], UNIT_TEST_ALIAS, env['LIB_TYPE'])
                projectString += '    <Option output="' + os.path.join(exeDir, 'unit_test') + '" prefix_auto="1" extension_auto="1" />\n'
                projectString += '    <Option working_dir="' + exeDir + '" />\n'

            else:
                projectString += '    <Option output = "" />\n'
                projectString += '    <Option working_dir = "" />\n'

            # Other options
            projectString += '    <Option object_output = "' + os.path.join(objectDir, env['SYSTEM'], target, env['LIB_TYPE']) + '" />\n'
            projectString += '    <Option type="1" />\n'
            projectString += '    <Option compiler="gcc" />\n'

            # Makefile commands
            projectString += '    <MakeCommands>\n'
            projectString += '        <Build command = "scons ' + targetCommand + '" />\n'
            projectString += '        <CompileFile command = "scons ' + targetCommand + ' $file" />\n'
            projectString += '        <Clean command = "scons ' + targetCommand + ' --clean" />\n'
            projectString += '        <DistClean command = "scons -f SConstruct distclean ' + targetCommand + '" />\n'
            projectString += '        <AskRebuildNeeded command = "scons -q ' + targetCommand + '" />\n'
            projectString += '        <SilentBuild command = "scons ' + targetCommand + ' &gt; $(CMD_NULL)" />\n'
            projectString += '    </MakeCommands>\n'
            projectString += '</Target>\n'

    projectString += '</Build>\n'
    # Add files
    for f in files:
        projectString += '<Unit filename="' + f + '" />\n'

    projectString += '<Unit filename="SConstruct" />\n'

    projectString += '<Extensions>\n'

    #Add include paths
    projectString += '<code_completion>\n'
    for path in env['CPPPATH']:
        projectString += '    <search_path add = "' + os.path.abspath(path) + '" />\n'
    projectString += "</code_completion>\n"

    projectString += '''
    </Extensions>
    </Project>
</CodeBlocks_project_file>
    '''

    return projectString

def generateCodeBlocksFilesBuilder(target, source, env):
    files = []
    files += getFilesInDirectory(includeDir)
    files += getFilesInDirectory(srcDir)
    files += getFilesInDirectory(testDir)

    cbpFile = open(str(target[0]), "w")
    cbpFile.write(generateCBPFile(env, files))
    cbpFile.close()

def generateCodeBlocksFiles(env, includePath, exeName):
    CBEnv = env.Clone(CPPPATH = includePath,
                      EXE_NAME = exeName)

    b = Builder(action = generateCodeBlocksFilesBuilder)
    CBEnv.Append(BUILDERS = {"cbproject" : b})

    sources = ["SConstruct", os.path.join(env['COMMON_DIR'], 'SConstructGlobals.py')]
    targets = [CBEnv['PROJECT_NAME'] + '.cbp']

    ret = CBEnv.cbproject(target = targets, source = sources)

    return ret


###
# Package Builds
###
def pkgbuildGenerator(target, source, env):
    outFile = open(str(target[0]), 'w')

    outFile.write('# This is an autogenerated file.\n\n')
    outFile.write('# Maintainer: ' + env['MAINTAINER'] + ' ' + env['MAINTAINER_EMAIL'] + '\n')
    outFile.write('pkgname=' + env['PKGNAME'] + '\n')
    outFile.write('pkgver=' + env['PKGVER'] + '\n')
    outFile.write('pkgrel=' + env['PKGREL'] + '\n')
    outFile.write('epoch=' + env['EPOCH'] + '\n')
    outFile.write('pkgdesc="' + env['PKGDESC'] + '"\n')
    outFile.write("arch=('" + env['ARCH'] + "')\n")
    outFile.write('url="' + env['URL'] + '"\n')
    outFile.write("license=('" + env['LICENSE'] + "')\n")

    if (env['GROUPS'] != ""):
        outFile.write("groups=('" + env['GROUPS'] + "')\n")
    else:
        outFile.write("groups=()\n")

    if (env['DEPENDS'] != ""):
        outFile.write('depends=("' + env['DEPENDS'] + '")\n')
    else:
        outFile.write('depends=()\n')

    if (env['MAKE_DEPENDS'] != ""):
        outFile.write('makedepends=("' + env['MAKE_DEPENDS'] + '")\n')
    else:
        outFile.write('makedepends=()\n')

    if (env['CHECK_DEPENDS'] != ""):
        outFile.write('checkdepends=("' + env['CHECK_DEPENDS'] + '")\n')
    else:
        outFile.write('checkdepends=()\n')

    if (env['OPT_DEPENDS'] != ""):
        outFile.write('optdepends=("' + env['OPT_DEPENDS'] + '")\n')
    else:
         outFile.write('optdepends=()\n')

    if (env['PROVIDES'] != ""):
        outFile.write('provides=("' + env['PROVIDES'] + '")\n')
    else:
        outFile.write('provides=()\n')

    if (env['CONFLICTS'] != ""):
        outFile.write('conflicts=("' + env['CONFLICTS'] + '")\n')
    else:
        outFile.write('conflicts=()\n')

    if (env['REPLACES'] != ""):
        outFile.write('replaces=("' + env['REPLACES'] + '")\n')
    else:
        outFile.write('replaces=()\n')

    if (env['BACKUP'] != ""):
        outFile.write('backup=("' + env['BACKUP'] + '")\n')
    else:
        outFile.write('backup=()\n')

    if (env['OPTIONS'] != ""):
        outFile.write('options=("' + env['OPTIONS'] + '")\n')
    else:
        outFile.write('options=()\n')

    outFile.write('install=' + env['INSTALL'] + '\n')
    outFile.write('changelog=' + env['CHANGELOG'] + '\n')

    if (env['PKG_SOURCE'] != ""):
        outFile.write('source=("' + env['PKG_SOURCE'] + '")\n')
    else:
         outFile.write('source=()\n')

    if (env['NO_EXTRACT'] != ""):
        outFile.write('noextract=("' + env['NO_EXTRACT'] + '")\n')
    else:
        outFile.write('noextract=()\n')

    if (env['MD5'] != ""):
        outFile.write("md5sums=('" + env['MD5'] + "')\n\n")
    else:
        outFile.write("md5sums=()\n\n")

    outFile.write("prepare() {\n")
    outFile.write(env['PREPARE_FUNC'] + '\n')
    outFile.write("}\n\n")

    outFile.write("build() {\n")
    outFile.write(env['BUILD_FUNC'] + '\n')
    outFile.write("}\n\n")

    outFile.write("check() {\n")
    outFile.write(env['CHECK_FUNC'] + '\n')
    outFile.write("}\n\n")

    outFile.write("package() {\n")
    outFile.write(env['PACKAGE_FUNC'] + '\n')
    outFile.write("}\n\n")

    outFile.close()

    return subprocess.call(['makepkg', '-p', str(target[0])])

def createArchPackageTarget(env,
                            sources,
                            maintainer = "",
                            maintainer_email = "",
                            pkgname = "",
                            pkgver = "",
                            pkgrel = "",
                            epoch = "",
                            pkgdesc = "",
                            arch = "any",
                            url = "",
                            license = "unknown",
                            groups = "",
                            depends = "",
                            makedepends = "",
                            checkdepends = "",
                            optdepends = "",
                            provides = "",
                            conflicts = "",
                            replaces = "",
                            backup = "",
                            options = "",
                            install = "",
                            changelog = "",
                            source = "",
                            noextract = "",
                            md5sums = "",
                            prepare_func = 'echo "No Prepare"',
                            build_func = 'echo "No Build"',
                            check_func = 'echo "No Check"',
                            package_func = 'echo "No Package"'):

    archEnv = env.Clone(
        MAINTAINER = maintainer,
        MAINTAINER_EMAIL = maintainer_email,
        PKGNAME = pkgname,
        PKGVER = pkgver,
        PKGREL = pkgrel,
        EPOCH = epoch,
        PKGDESC = pkgdesc,
        ARCH = arch,
        URL = url,
        LICENSE = license,
        GROUPS = groups,
        DEPENDS = depends,
        MAKE_DEPENDS = makedepends,
        CHECK_DEPENDS = checkdepends,
        OPT_DEPENDS = optdepends,
        PROVIDES = provides,
        CONFLICTS = conflicts,
        REPLACES = replaces,
        BACKUP = backup,
        OPTIONS = options,
        INSTALL = install,
        CHANGELOG = changelog,
        PKG_SOURCE = source,
        NO_EXTRACT = noextract,
        MD5 = md5sums,
        PREPARE_FUNC = prepare_func,
        BUILD_FUNC = build_func,
        CHECK_FUNC = check_func,
        PACKAGE_FUNC = package_func)

    pkgBuilder = Builder (action = pkgbuildGenerator)

    archEnv.Append(BUILDERS = {'pkgbuild' : pkgBuilder})

    packageTarget = pkgname + '-' + pkgver + '-' + pkgrel + '-' + arch + '.pkg.tar.xz'
    target = archEnv.pkgbuild(target = ['PKGBUILD-' + arch, packageTarget], source=sources)

    Clean(target, 'pkg')
    return target

