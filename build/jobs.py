import os
import subprocess
import sys

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import getRedirectString

rootDir = os.path.abspath("..")

#Add to these lists where to call scons
targets = {}

#numbers in key give order

targets['1gmock'] = os.path.join(rootDir, "Common", "gmock")

targets["2CppUTest"] = os.path.join(rootDir, "Common", "CppUTest")

targets["3DebugNew"] = os.path.join(rootDir, "Common", "debug_new")

targets["4OSBuild"] = os.path.join(rootDir,"Common", "os")

targets["5MathBuild"] = os.path.join(rootDir,"Common", "math")

targets['6CommonClassesBuild'] = os.path.join(rootDir, "Common", "common_classes")

targets["7SCSVBuild"] = os.path.join(rootDir, "Common", "SCSV")
