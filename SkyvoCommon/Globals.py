import os
import sys

#Globals.py
codeCoverageDir = "CodeCoverage"
profilingDataFile = os.path.join(codeCoverageDir, "profiling_data.txt")
apiDir = "api"
binDir = "bin"
docDir = "doc"
doxygenDir = os.path.join(docDir, "doxygen")
includeDir = "include"
libDir = "lib"
objectDir = "obj"
srcDir = "src"
testDir = "Test"
testOutputDir = "testOutput"
armPrefix = "-arm"

debugDir = "debug"
releaseDir = "release"
unitTestDir = "unit_test"

fitNesseDir = "fitnesse"

#Determines if the first argument is arm
def parseArguments():
    arm = False
    if (len(sys.argv) == 2):
        if (sys.argv[1] == "arm"):
            arm = True
    return arm
    

def getCommonClassesPath(env):
    return os.path.join(getCommonPath(env), "common_classes")

def getCommonPath(env):
    return os.path.join(env['NOTEBOOK_ROOT'], "Common")

def getNotebookCorePath(env):
    return os.path.join(env['NOTEBOOK_ROOT'], "Notebook-Core")
    
def getSCSVPath(env):
    return os.path.join(getCommonPath(env), "SCSV")
    
def getSkyvoOSPath(env):
    return os.path.join(getCommonPath(env), "os")

def getCppUTestPath(env):
    return os.path.join(env['NOTEBOOK_ROOT'], fitNesseDir, "fixtures", "CppUTest")


def getRedirectString(file):
    if(sys.platform == "win32"):
        redirectString = " > " + str(file) + " 2>&1 "
    else:
        redirectString = " &> " + str(file)
    return redirectString
