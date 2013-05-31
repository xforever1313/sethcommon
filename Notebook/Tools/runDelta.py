import time
import os
import subprocess
import sys

from jobs import *

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import getRedirectString

thisDir = os.getcwd()
currentTime = time.strftime("%m_%d_%Y_%H_%M_%S")

arm = False

if (len(sys.argv) == 2):
    if (sys.argv[1] == "arm"):
        arm = True

if (sys.platform == "win32"):
    buildName = "Welta"
elif(sys.platform == "darwin"):
    buildName = "Melta"
else:
    buildName = "Lelta"

logDir = os.path.join(thisDir, "logs", buildName + "-" + currentTime)
os.makedirs(logDir)

i = 0
while (i < len(targetLocations)):
    logFile = os.path.join(logDir, targetNames[i] + ".log")
    os.chdir(targetLocations[i])
    
    redirectString = getRedirectString(logFile)
    
    if (arm): 
        commandStr = "scons delta arm_build=1" + redirectString
    else:
        commandStr = "scons delta arm_build=0" + redirectString
    print(commandStr)
    status = subprocess.call(commandStr, shell=True)

    os.chdir(thisDir)
    if (status != 0):
        break
    i += 1
    
if (status != 0):
    print("A compile error occured!")
    exit(status)

#Tests
buildFixtures(logDir)
runFitnesseSuite("NotebookTests.SmokeTests", logDir)
