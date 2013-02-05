#runTestHeader.py
#This file has functions used by runTest methods

import subprocess
import os

#Calls Buildtargets.exe which builds all targets
#executablePath - location of buildTargets.exe
#logPath - abs path of where to put the log file, NO file name
def build(executablePath, logPath):
    os.chdir(executablePath)
    return subprocess.call("buildTargets.exe unclean default " + str(logPath), shell=True)

#Calls Buildtargets.exe which rebuilds all targets
#executablePath - location of buildTargets.exe
#logPath - abs path of where to put the log file, along with the filename
def rebuild(executablePath, logPath):
    os.chdir(executablePath)
    return subprocess.call("buildTargets.exe clean default " + str(logPath), shell=True)

#Builds the tools
#srcPath - path of the tools/src folder
#logPath - abs path of where to put the log file
def buildTools(srcPath, logPath):
    numFailed = 0
    os.chdir(srcPath)
	
    buildTargetLog = os.path.join(logPath, "buildTarget.log")
    buildTargetCommand = "g++ buildTargets.cpp -o bin/buildTargets.exe > " + buildTargetLog
    print (buildTargetCommand)
    x = subprocess.call(buildTargetCommand, shell=True)
    if (x != 0):
        print ("WARNING! Build Failed!")
        numFailed = numFailed + 1

    coverageLog = os.path.join(logPath, "coverage.log")
    ccoverageCommand = "codeblocks --rebuild --target=\"Release\" ccoverage.cbp > " + coverageLog
    print(ccoverageCommand)
    y = subprocess.call(ccoverageCommand, shell=True)
    if (y != 0):
        print ("WARNING! Build Failed!")
        numFailed = numFailed + 1

    if (numFailed == 0):
        print ("All Tools build successfully!")
    else:
        print("WARNING! " + str(numFailed) + " tools did not build")

    return numFailed

#Runs the tests from given file
#readerFilePath - the path to the file to be read
#readerFile - the file with the test list inside
#logPath - abs path of where to put the log file. Do NOT include the file Name
def runTests (readerFilePath, readerFile, logPath):
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
        logFile = os.path.join(logPath, testName[i] + ".log")

        success = subprocess.call(testExe[i] + " > " + str(logFile), shell=True)

        if (success != 0):
            numFailed = numFailed + 1

        print("\n")
        i += 1
        os.chdir(readerFilePath)

    return numFailed