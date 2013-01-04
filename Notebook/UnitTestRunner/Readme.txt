Instructions run tests:

==How to run==
To run the tests, simply press "runTests.bat".  All the tests will then run. 

==How to add tests==
Inside of tests.txt, simply add the path to the test executable RELATIVE to THIS directory on a new line.  Then, on the same line, put the name of the executable after a tab.  For example:

../path/to/testFile		testName.exe

==What the bat file does==
The bat opens tests.txt, and reads in the path and the file name. It runs the test from the test's executable's point of view, not this directory's.  When all the tests are completed, the program waits 3 seconds for any remaining programs to finish.  It will then print "Press any key to continue."  Pressing any key will then terminate the program.

This is a temporary solution until a more robust testing platform can be made.