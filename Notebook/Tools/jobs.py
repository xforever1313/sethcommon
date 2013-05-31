import os
import subprocess
import sys

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import getRedirectString

fitnessDir = "../fitnesse"

notebookRootDirectory = os.path.abspath("..")

#Add to these lists where to call scons
targetLocations = []
targetNames = []

#Core build
targetNames += ["OSBuild"] #Must be built first
targetLocations += [os.path.join(notebookRootDirectory,"Common", "os")]

targetNames += ['CommonClassesBuild']
targetLocations += [os.path.join(notebookRootDirectory, "Common", "common_classes")]

targetNames += ["SCSVBuild"]
targetLocations += [os.path.join(notebookRootDirectory, "Common", "SCSV")]

targetNames += ["NotebookCoreBuild"] 
targetLocations += [os.path.join(notebookRootDirectory,"Notebook-Core")]

targetNames += ["NotebookCLI"]
targetLocations += [os.path.join(notebookRootDirectory, "Notebook-CLI")]

targetNames += ["SkyCoreBuild"]
targetLocations += [os.path.join(notebookRootDirectory, "SkyWrite-Core")]

def buildFixtures(logDir):
    thisDir = os.getcwd()
    redirectString = getRedirectString(os.path.abspath(os.path.join(logDir, "fixtureBuild.log")))
    command = "python installFitnesse.py " + str(redirectString)
    print (command)
    os.chdir(fitnessDir)
    status = subprocess.call(command, shell=True)
    if (status != 0):
        raise Exception ("Failed to Install Fitnesse")
        
    os.chdir(thisDir)
        
def cleanFixtures():
    thisDir = os.getcwd()
    os.chdir(fitnessDir)
    status = subprocess.call("python cleanFixtures.py", shell=True)
    if (status != 0):
        raise Exception ("Failed to clean fixtures")
        
    os.chdir(thisDir)
        
def runFitnesseSuite(suiteName, logDir):
    redirectString = getRedirectString(os.path.abspath(os.path.join(logDir, "FitNesseLog.html")))
    thisDir = os.getcwd()
    os.chdir(fitnessDir)
    command = "java -jar fitnesse-standalone.jar -e 1 -p 5000 -c " + suiteName + "?suite " + redirectString
    print (command)
    status = subprocess.call(command, shell=True)
    if (status != 0):
        raise Exception ("TEST FAILED")
    os.chdir (thisDir)