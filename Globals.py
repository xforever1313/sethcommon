import atexit
import getpass
import os
import string
import subprocess
import sys
import time

#Globals.py
thisDir = os.getcwd()

codeCoverageDir = "CodeCoverage"
apiDir = "api"
binDir = "bin"
docDir = "doc"
doxygenDir = os.path.join(docDir, "doxygen")
includeDir = "include"
libDir = "lib"
objectDir = "obj"
srcDir = "src"
testDir = "test"
testOutputDir = "testOutput"
armPrefix = "-arm"

debugDir = "debug"
releaseDir = "release"
unitTestDir = "unit_test"

buildDir = "build"
fitNesseDir = "fitnesse"

reportDir = "reports"
profilingDataFile = os.path.join(reportDir, "profiling_data.txt")
cppCheckData = os.path.join(reportDir, 'cppcheck_data.txt')
doxygenData = os.path.join(reportDir, 'doxygen_data.txt')

def createDir(dir):
    if (not os.path.exists(dir)):
        os.mkdir(dir)

#Determines if the first argument is arm
def parseArguments():
    arm = False
    if (len(sys.argv) == 2):
        if (sys.argv[1] == "arm"):
            arm = True
    return arm

def replaceSpacesWithUnderscores(s):
    return string.replace(s, ' ', '_')

def getFilesInDirectory(directory):
    ret = []
    for root, directories, files in os.walk(directory):
        for filename in files:
            filepath = os.path.join(root, filename)
            ret.append(filepath)
    return ret

def getDirectoryStructure(directory):
    r = []                                                                                                            
    subdirs = [x[0] for x in os.walk(directory)]                                                                            
    for subdir in subdirs:                                                                                            
        files = os.walk(subdir).next()[2]                                                                             
        if (len(files) > 0):                                                                                          
            for file in files:   
                if (subdir not in r):                                                                              
                    r.append(subdir)                                                                         
    return r  

def getRevisionNumber():
    revProc = subprocess.Popen("git rev-list --count HEAD", shell=True, stdout=subprocess.PIPE)
    rawString = revProc.communicate()[0]
    return rawString.strip()

def getUserName():
    return getpass.getuser()
    
def getVersion(baseDir):
    versionFile = open(os.path.join(baseDir, buildDir, "version.txt"))
    versionString = versionFile.readline()
    versionFile.close()
    return versionString.strip()

def getReleaseVersionNumber(baseDir):
    return getVersion(baseDir) + '+' + getRevisionNumber()

def getRedirectString(file):
    if(sys.platform == "win32"):
        redirectString = " > " + str(file) + " 2>&1 "
    else:
        redirectString = " &> " + str(file)
    return redirectString

def printEndTime(startTime):
    print "Time ran: " + str(time.time() - startTime) + " seconds."

def timeProgram():
    startTime = time.time()
    atexit.register(printEndTime, startTime)
