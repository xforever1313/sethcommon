#runTests.py
#Description: Runs all of the tests located in 

import sys
import os

absPath = os.getcwd()
headerPath = os.path.join(absPath, "src")
sys.path.append(headerPath)
import toolHeader

numFailed = toolHeader.runTests(absPath, "tests.txt")

if (numFailed == 0):
    print("All Tests Pass!")
else:
    print(numFailed + " Tests Failed!")

input("Press Any Key to continue")