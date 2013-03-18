from toolHeader import *

import time
import os
import subprocess

notebookRootDirectory = os.path.abspath(os.path.join("..",".."))
thisDir = os.getcwd()
currentTime = time.strftime("%m_%d_%Y_%H_%M_%S")

if (sys.platform == "win32"):
    buildName = "nightWing"
elif(sys.platform == "darwin"):
    buildName = "NightMac"
else:
    buildName = "Nightix"

logDir = os.path.join(thisDir, "logs", buildName + "-" + currentTime)
os.mkdir(logDir)

#Add to these lists where to call "scons nightly"
targetLocations = []
targetNames = []

#Core build
targetNames += ["CoreBuild"]
targetLocations += [os.path.join(notebookRootDirectory,"Core")]

i = 0
while (i < len(targetLocations)):
    logFile = os.path.join(logDir, targetNames[i] + ".log")
    os.chdir(targetLocations[i])
    
    if(sys.platform == "win32"):
        redirectString = " > " + str(logFile) + " 2>&1 "
    else:
        redirectString = " &> " + str(logFile)

    commandStr = "scons nightly --clean"
    print (commandStr)
    #subprocess.call(commandStr, shell=True)
    
    commandStr = "scons nightly " + redirectString
    print(commandStr)
    status = subprocess.call(commandStr, shell=True)

    os.chdir(thisDir)
    if (status != 0):
        break
    i += 1
    
if (status != 0):
    print("A compile error occured!")
    exit(status)