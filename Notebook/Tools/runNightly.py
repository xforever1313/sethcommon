import time
import os
import subprocess
import sys

from jobs import *

thisDir = os.getcwd()
currentTime = time.strftime("%m_%d_%Y_%H_%M_%S")

arm = False

if (len(sys.argv) == 2):
    if (sys.argv[1] == "arm"):
        arm = True

if (sys.platform == "win32"):
    buildName = "nightWing"
elif(sys.platform == "darwin"):
    buildName = "NightMac"
else:
    buildName = "Nightix"

logDir = os.path.join(thisDir, "logs", buildName + "-" + currentTime)
os.makedirs(logDir)

i = 0
while (i < len(targetLocations)):
    logFile = os.path.join(logDir, targetNames[i] + ".log")
    os.chdir(targetLocations[i])
    
    if(sys.platform == "win32"):
        redirectString = " > " + str(logFile) + " 2>&1 "
    else:
        redirectString = " &> " + str(logFile)
    
    if (arm): 
        commandStr = "scons nightly arm_build=1 --clean" 
    else:
        commandStr = "scons nightly arm_build=0 --clean"
    print (commandStr)
    subprocess.call(commandStr, shell=True)
    
    if (arm): 
        commandStr = "scons nightly arm_build=1" # + redirectString
    else:
        commandStr = "scons nightly arm_build=0" # + redirectString
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
cleanFixtures()
buildFixtures()