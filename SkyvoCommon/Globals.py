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

def getRedirectString(file):
    if(sys.platform == "win32"):
        redirectString = " > " + str(file) + " 2>&1 "
    else:
        redirectString = " &> " + str(file)
    return redirectString
