from toolHeader import *

import time
import os
import subprocess

from jobs import *

thisDir = os.getcwd()
currentTime = time.strftime("%m_%d_%Y_%H_%M_%S")

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
    
    if(sys.platform == "win32"):
        redirectString = " > " + str(logFile) + " 2>&1 "
    else:
        redirectString = " &> " + str(logFile)
    
    commandStr = "scons delta " #+ redirectString
    print(commandStr)
    status = subprocess.call(commandStr, shell=True)

    os.chdir(thisDir)
    if (status != 0):
        break
    i += 1
    
if (status != 0):
    print("A compile error occured!")
    exit(status)