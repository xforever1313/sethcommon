@echo off
for /f "tokens=1,2" %%G in (tests.txt) do start /D%%G /wait /B %%H

PING -n 1 -w 3000 1.1.1.1 > nul
pause
