#runTestHeader.py
#This file has functions used by runTest methods

import subprocess
import os

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