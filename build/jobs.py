import os
import subprocess
import sys

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import getRedirectString

rootDir = os.path.abspath("..")

#Add to these lists where to call scons
targetLocations = []
targetNames = []

targetNames += ['gmock']
targetLocations += [os.path.join(rootDir, "Common", "gmock")]

targetNames += ["CppUTest"]
targetLocations += [os.path.join(rootDir, "Common", "CppUTest")]

targetNames += ["OSBuild"]
targetLocations += [os.path.join(rootDir,"Common", "os")]

targetNames += ["MathBuild"] 
targetLocations += [os.path.join(rootDir,"Common", "math")]

targetNames += ['CommonClassesBuild']
targetLocations += [os.path.join(rootDir, "Common", "common_classes")]

targetNames += ["SCSVBuild"]
targetLocations += [os.path.join(rootDir, "Common", "SCSV")]

targetNames += ['sqlite']
targetLocations += [os.path.join(rootDir, "Common", "sqlite")]