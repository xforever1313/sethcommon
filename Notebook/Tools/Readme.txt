About Tools:

buildTargets.exe - 
==What==
Using Code::Blocks, this rebuilds and updates all of the targets specified in projects.txt.

==How to run==
Simply press "buildTargets.exe"  All given targets will be rebuilt and ran.

==How to add targets==
A target builds a certain executable.  Usual targets include "Debug", "release" and "Unit_test"

You need to add targets and code::block projects to projects.txt

The first column is the path RELATIVE to THIS directory to the code::blocks project.
The second column is the name of the code::blocks project
The third column is the target to be built (e.g. debug, release, unit_test)

Note, these ARE case sensitive.  If you mess up the target, the program will crash.

----------------------------------------------
runTests.bat - test runner

==What==
Runs tests listed in tests.txt

==How to run==
To run the tests, simply press "runTests.bat".  All the tests will then run. 

==How to add tests==
Inside of tests.txt, simply add the path to the test executable RELATIVE to THIS directory on a new line.  Then, on the same line, put the name of the executable after a tab.  For example:

../path/to/testFile		testName.exe

==What the bat file does==
The bat opens tests.txt, and reads in the path and the file name. It runs the test from the test's executable's point of view, not this directory's.  When all the tests are completed, the program waits 3 seconds for any remaining programs to finish.  It will then print "Press any key to continue."  Pressing any key will then terminate the program.

This is a temporary solution until a more robust testing platform can be made.