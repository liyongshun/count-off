@echo off
set working_path=%~dp0
set project=%1

echo try build %working_path%
echo *******************************************************************************
echo start generate cmake project...

if not exist build md build

cd build
cmake -DCMAKE_BUILD_TYPE=Debug  -Dproj_name=%project% ..

echo *******************************************************************************
echo start build cmake project...

cmake --build .

if %errorlevel% neq 0 (
    echo build %working_path% fail
    pause
    exit 1
)

echo *******************************************************************************
echo start run tests...

.\debug\%project% --gtest_color=yes

if  %errorlevel% neq 0 (
    echo build %working_path% fail
    pause
    exit 1
)

cd ..

echo *******************************************************************************
echo build %working_path% succ
