#runTests.py
#Description: Runs all of the tests located in 

import sys
import os

absPath = os.getcwd()
headerPath = os.path.join(absPath, "src")
sys.path.append(headerPath)
import toolHeader

logPath = os.path.join(absPath, "logs")
numFailed = toolHeader.runTests(absPath, "tests.txt", logPath)

if (numFailed == 0):
    print("All Tests Pass!")
else:
    print(str(numFailed) + " Tests Failed!")

input("Press Any Key to continue")