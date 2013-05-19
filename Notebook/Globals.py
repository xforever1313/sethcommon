from SCons.Script.Main import *
from SCons.Environment import *

import os

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

def getCommonPath(env):
    return os.path.join(env['NOTEBOOK_ROOT'], "Common")

def getCorePath(env):
    return os.path.join(env['NOTEBOOK_ROOT'], "Core")

def getSCSVPath(env):
    return os.path.join(getCommonPath(env), "SCSV")
    
def getSkyvoOSPath(env):
    return os.path.join(getCommonPath(env), "os")

def getCommonClassesPath(env):
    return os.path.join(getCommonPath(env), "common_classes")