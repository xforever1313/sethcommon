#runTestHeader.py
#This file has functions used by runTest methods

import subprocess
import os

#Calls Buildtargets.exe which builds all targets
#executablePath - location of buildTargets.exe
def build(executablePath):
    os.chdir(executablePath)
    return subprocess.call("buildTargets.exe")

#Calls Buildtargets.exe which rebuilds all targets
#executablePath - location of buildTargets.exe
def rebuild(executablePath):
    os.chdir(executablePath)
    return subprocess.call("buildTargets.exe clean")

#Builds the tools
#srcPath - path of the tools/src folder
def buildTools(srcPath):
    numFailed = 0
    os.chdir("src")
    buildTargetCommand = "g++ buildTargets.cpp -o bin/buildTargets.exe"
    print (buildTargetCommand)
    x = subprocess.call(buildTargetCommand)
    if (x != 0):
        print ("WARNING! Build Failed!")
        numFailed = numFailed + 1

    ccoverageCommand = "codeblocks --rebuild --target=\"Release\" ccoverage.cbp"
    print(ccoverageCommand)
    y = subprocess.call(ccoverageCommand)
    if (y != 0):
        print ("WARNING! Build Failed!")
        numFailed = numFailed + 1

    if (numFailed == 0):
        print ("All Tools build successfully!")
    else:
        print("WARNING! " + numFailed + " tools did not build")

    return numFailed

#Runs the tests from given file
#readerFilePath - the path to the file to be read
#readerFile - the file with the test list inside
def runTests (readerFilePath, readerFile):
    numFailed = 0
	
    testName = []
    testPath = []
    testExe = []

    os.chdir(readerFilePath)
    inFile = open(readerFile, "r")

    while (True):
        line = inFile.readline()
        if (line == ""):
            break
        splitLine = line.strip().split()
        testName.append(splitLine[0])
        testPath.append(splitLine[1])
        testExe.append(splitLine[2])

    inFile.close()

    maxIterations = len(testExe)
    i = 0
    while (i < maxIterations):
        print ("Testing: " + testName[i])

        os.chdir(testPath[i])
		
        success = subprocess.call(testExe[i])
        if (success != 0):
            ++numFailed

        print("\n")
        i += 1
        os.chdir(readerFilePath)

    return numFailed