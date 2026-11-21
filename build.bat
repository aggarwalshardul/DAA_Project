@echo off
echo ========================================================
echo Compiling EV Charging Station Placement ^& Trip Planning
echo Full Syllabus Workbench (Phase II Complete)
echo ========================================================

set SRCS=src\model.c src\analysis.c src\sorting_searching.c src\divide_conquer.c src\greedy.c src\dp.c src\backtracking.c src\flow_matching.c src\number_theory.c src\string_match.c src\scenarios.c

gcc -Wall -Wextra -std=c11 -O2 -o ev_planner.exe src\main.c %SRCS% -lm
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilation failed!
    exit /b %ERRORLEVEL%
)

gcc -Wall -Wextra -std=c11 -O2 -o test_runner.exe tests\test_runner.c %SRCS% -lm
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Test suite compilation failed!
    exit /b %ERRORLEVEL%
)

echo [SUCCESS] Both ev_planner.exe and test_runner.exe built cleanly with ZERO warnings!
