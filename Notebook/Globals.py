#Globals.py
import sys
import os
import subprocess
import shutil
import glob
import time

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
globalCXXFlags = ["-Wall", "-pedantic", "-Werror", "-std=gnu++11"]
globalCXXDebugFlags = ["-g"]
globalCXXReleaseFlags = ["-O2"]
globalCXXUnitTestFlags = ["-g", "-fprofile-arcs", "-ftest-coverage"]

#Linker Flags
globalLinkerFlags = ["-Wall", "-g", "-Werror", "-std=gnu++11"]

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

extractionLocation = ""

#Runs the test, and coverage while we are at it :P
def testRunner(target, source, env):
    thisDir = os.getcwd()
    os.chdir(os.path.join(executableDirectory,"unit_test"))
    
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
				shutil.rmtree(codeCoverageDirectory)
				#wait for it to be deleted
				time.sleep(3)
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