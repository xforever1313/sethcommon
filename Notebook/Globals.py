#Globals.py
import sys
import os
import subprocess
import shutil
import glob

notebookRootDirectory = os.getcwd()

codeCoverageDirectory = "CodeCoverage"
objectDirectory = "obj"
executableDirectory = "bin"
testDirectory = "Test"
doxygenDirectory = "doxygen"

ignoredSrcDirectories = codeCoverageDirectory + objectDirectory + executableDirectory + doxygenDirectory +testDirectory
ignoredTestDirectories = codeCoverageDirectory + objectDirectory + executableDirectory + doxygenDirectory

#Defines
globalDefines = []
if (sys.platform == "win32"):
    globalDefines += ["_WIN32_WINNT=0x0501"]
    
globalDebugDefines = ["-DDEBUG"]
globalReleaseDefines = ["-DRELEASE"]
globalUnitTestDefines = ["-DUNIT_TEST"]
if (sys.platform != "win32"):
	globalUnitTestDefines += ["-DBOOST_TEST_DYN_LINK"]

#Compile Flags
globalCXXFlags = ["-Wall", "-pedantic"]
globalCXXDebugFlags = ["-g"]
globalCXXReleaseFlags = []
globalCXXUnitTestFlags = ["-fprofile-arcs", "-ftest-coverage"]

#Linker Flags
globalLinkerFlags = ["-Wall"]

#Linker Libs
globalLibsDebugFlags = []
globalLibsReleaseFlags = []
globalLibsUnitTestFlags = ["gcov", "boost_unit_test_framework", "boost_system"]

if (sys.platform == "win32"):
	globalLibsDebugFlags += ["debug_new"]
	globalLibsUnitTestFlags += ["debug_new"]

#Tool locations:
toolsPath = os.path.join(notebookRootDirectory, "Tools")
corePath = os.path.join(notebookRootDirectory, "Core")
toolHeaderLocation = os.path.join(notebookRootDirectory, "Tools", "src", "toolHeader.py")

#Runs the test, and coverage while we are at it :P
def testRunner(target, source, env):
    thisDir = os.getcwd()
    os.chdir(os.path.join(executableDirectory,"unit_test"))
    
    if (sys.platform == "win32"):
        status = subprocess.call("unit_test.exe")
    else:
        status = subprocess.call("./unit_test")
    os.chdir(thisDir)

    if (status == 0):
        print("Running Coverage")
        try:
            shutil.rmtree(codeCoverageDirectory)
        except:
            print("CodeCoverage directory missing, recreating")

        os.mkdir(codeCoverageDirectory)
        
        gcovString = "gcov -r -s \"" + thisDir + "\" -o \"" + os.path.join(thisDir, objectDirectory, "unit_test") + "\" "
        for file in source:
            gcovString += (os.path.join(thisDir, str(file)) + " ")
        print (gcovString)
        
        subprocess.call(gcovString)
        
        #Move all .gcov files to  CodeCoverage folder
        gcovGlob = glob.glob("*.gcov")    
        for file in gcovGlob:
            shutil.move(file, codeCoverageDirectory)
            
    else:
        raise Exception("Test Failed!")
