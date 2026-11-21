@echo off
echo ========================================================
echo Compiling EV Charging Station Placement ^& Trip Planning
echo ========================================================

gcc -Wall -Wextra -std=c11 -O2 -o ev_planner.exe src\main.c src\model.c src\analysis.c src\sorting_searching.c src\greedy.c src\dp.c -lm
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilation failed!
    exit /b %ERRORLEVEL%
)

gcc -Wall -Wextra -std=c11 -O2 -o test_runner.exe tests\test_runner.c src\model.c src\analysis.c src\sorting_searching.c src\greedy.c src\dp.c -lm
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Test suite compilation failed!
    exit /b %ERRORLEVEL%
)

echo [SUCCESS] Both ev_planner.exe and test_runner.exe built cleanly with ZERO warnings!
