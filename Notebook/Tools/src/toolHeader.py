#runTestHeader.py
#This file has functions used by runTest methods

import subprocess
import os
import sys
import shutil

#Calls Buildtargets.exe which builds all targets
#buildTargetsTxt - abslocation of the buildTargets.txt file
#logPath - abs path of where to put the log file. Do NOT include the file Name
def build(buildTargetsTxt, logPath):
    numFailed = 0
    
    SConstructDir = []
    BuildName = []
    Targets = []
    
    inFile = open(buildTargetsTxt, "r")
    while (True):
        line = inFile.readline()
        if (line == ""):
            break
        splitLine = line.strip().split()
        SConstructDir.append(splitLine[0])
        BuildName.append(splitLine[1])
        Targets.append(splitLine[2])
    inFile.close()

    maxIterations = len(SConstructDir)
    i = 0
    while (i < maxIterations):
        print ("Building: " + BuildName[i])

        os.chdir(SConstructDir[i])
        logFile = os.path.join(logPath, BuildName[i] + ".log")
        if(sys.platform == "win32"):
            redirectString = " > " + str(logFile) + " 2>&1 "
        else:
            redirectString = " &> " + str(logFile)

        success = subprocess.call("scons " + Targets[i] + redirectString, shell=True)

        if (success != 0):
            numFailed = numFailed + 1

        print("\n")
        i += 1
        os.chdir(os.path.dirname(buildTargetsTxt))

    return numFailed

#Calls Buildtargets.exe which rebuilds all targets
#buildTargetsTxt - location of the Tests.txt file
#logPath - abs path of where to put the log file. Do NOT include the file Name
def rebuild(buildTargetsTxt, logPath):
    numFailed = 0
    
    SConstructDir = []
    BuildName = []
    Targets = []
    
    inFile = open(buildTargetsTxt, "r")
    while (True):
        line = inFile.readline()
        if (line == ""):
            break
        splitLine = line.strip().split()
        SConstructDir.append(splitLine[0])
        BuildName.append(splitLine[1])
        Targets.append(splitLine[2])
    inFile.close()

    maxIterations = len(SConstructDir)
    i = 0
    while (i < maxIterations):
        print ("Building: " + BuildName[i])

        os.chdir(SConstructDir[i])
        logFile = os.path.join(logPath, BuildName[i] + ".log")
        if(sys.platform == "win32"):
            redirectString = " > " + str(logFile) + " 2>&1 "
        else:
            redirectString = " &> " + str(logFile)
            
        subprocess.call("scons " + Targets[i] + " --clean ", shell=True)
        print("Building... (printing to log)")
        success = subprocess.call("scons " + Targets[i] + redirectString, shell=True)

        if (success != 0):
            numFailed = numFailed + 1

        print("\n")
        i += 1
        os.chdir(os.path.dirname(buildTargetsTxt))

    return numFailed

#Runs the tests from given file
#readerFilePath - the path to the file to be read
#readerFile - the file with the test list inside
#logPath - abs path of where to put the log file. Do NOT include the file Name
def runTests (readerFilePath, readerFile, logPath):
    numFailed = 0

    testName = []
    testPath = []

    os.chdir(readerFilePath)
    inFile = open(readerFile, "r")

    while (True):
        line = inFile.readline()
        if (line == ""):
            break
        splitLine = line.strip().split()
        testName.append(splitLine[0])
        testPath.append(splitLine[1])

    inFile.close()

    maxIterations = len(testName)
    i = 0
    while (i < maxIterations):
        print ("Testing: " + testName[i])

        os.chdir(testPath[i])
        logFile = os.path.join(logPath, testName[i] + ".log")
        if(sys.platform == "win32"):
            redirectString = " > " + str(logFile) + " 2>&1 "
        else:
            redirectString = " &> " + str(logFile)

        success = subprocess.call("scons run_test" + redirectString, shell=True)

        if (success != 0):
            numFailed = numFailed + 1

        print("\n")
        i += 1
        os.chdir(readerFilePath)

    return numFailed