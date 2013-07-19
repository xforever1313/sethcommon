import subprocess
import os
os.chdir("bin/unit_test")
numRan = 0
run = True
while(run):
    try:
        status = subprocess.call("unit_test.exe", shell=True)
        if (status == 0):
            numRan += 1
        else:
            raise Exception("FAIL")
    except KeyboardInterrupt:
        print ("Num ran : " + str(numRan))
        run = False 