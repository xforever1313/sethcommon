import getpass
import os
import string
import subprocess
import sys

#Globals.py
thisDir = os.getcwd()

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

buildDir = "build"
fitNesseDir = "fitnesse"

#Determines if the first argument is arm
def parseArguments():
    arm = False
    if (len(sys.argv) == 2):
        if (sys.argv[1] == "arm"):
            arm = True
    return arm

def replaceSpacesWithUnderscores(s):
    return string.replace(s, ' ', '_')
    
def getRevisionNumber():
    revProc = subprocess.Popen("git rev-list --count HEAD", shell=True, stdout=subprocess.PIPE)
    rawString = revProc.communicate()[0]
    return rawString.rstrip('\n')

def getUserName():
    return getpass.getuser()
    
def getVersion(baseDir):
    versionFile = open(os.path.join(baseDir, buildDir, "version.txt"))
    versionString = versionFile.readline()
    versionFile.close()
    return versionString

def getReleaseVersionNumber(baseDir):
    return getVersion(baseDir) + '+' + getRevisionNumber()

def getRedirectString(file):
    if(sys.platform == "win32"):
        redirectString = " > " + str(file) + " 2>&1 "
    else:
        redirectString = " &> " + str(file)
    return redirectString
