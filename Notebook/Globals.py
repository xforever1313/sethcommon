import os
import sys

#Globals.py
codeCoverageDir = "CodeCoverage"
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

    
def getRedirectString(file):
    if(sys.platform == "win32"):
        redirectString = " > " + str(file) + " 2>&1 "
    else:
        redirectString = " &> " + str(file)
    return redirectString