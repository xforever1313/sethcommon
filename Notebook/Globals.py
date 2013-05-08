from SCons.Script.Main import *
from SCons.Environment import *

import os

#Globals.py
codeCoverageDir = "CodeCoverage"
binDir = "bin"
docDir = "doc"
includeDir = "include"
libDir = "lib"
objectDir = "obj"
srcDir = "src"
testDir = "Test"

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
