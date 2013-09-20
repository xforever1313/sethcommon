import time
import os
import subprocess
import sys

from jobs import *

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import getRedirectString, timeProgram

thisDir = os.getcwd()
currentTime = time.strftime("%m_%d_%Y_%H_%M_%S")

logDir = os.path.join(thisDir, "logs", sys.argv[1] + "-" + currentTime)
os.makedirs(logDir)

args = ""

for arg in sys.argv[1:]:
    args += arg
    args += ' '

i = 0
while (i < len(targetLocations)):
    logFile = os.path.join(logDir, targetNames[i] + ".log")
    
    redirectString = getRedirectString(logFile)
    print("Running scons " + args + "for " + targetNames[i])
    
    commandStr = "scons " + args + " " + redirectString
    status = subprocess.call(commandStr, shell=True, cwd=targetLocations[i])
    
    if (status != 0):
        break
    i += 1
    
if (status != 0):
    print("A compile error occured!")
    exit(status)