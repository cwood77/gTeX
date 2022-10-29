@echo off

rem %1 = testcasedir
rem %2 = expected exe out
rem %3 = expected doc out
rem %4 = expected dot out

setlocal

set here=%CD%
pushd .
cd %1
set afterwards=cleanup

rem RUN
rmdir /S /Q out > nul 2>&1
rmdir /S /Q act > nul 2>&1
mkdir act > nul 2>&1
%here%\bin\out\debug\gTeX.exe -reproducible > act\exe.txt 2>&1

rem CHECK
if "%2" == "none" goto skipExeCmp
set expected=%2
if "%2" == "def" set expected=expected-conout.txt
rem echo comparing EXE output against %expected%
fc act\exe.txt %expected% >nul 2>&1
if %ERRORLEVEL% NEQ 0 set afterwards=orphan
:skipExeCmp

if "%3" == "none" goto skipDocCmp
set expected=%3
if "%3" == "def" set expected=expected-docout.txt
rem echo comparing doc output against %expected%
fc out\raw\text\out.txt %expected% >nul 2>&1
if %ERRORLEVEL% NEQ 0 set afterwards=orphan
:skipDocCmp

if "%4" == "none" goto skipDotCmp
set expected=%4
if "%4" == "def" set expected=expected-dotout.dot
rem echo comparing dot output against %expected%
fc out\raw\text\out.dot %expected% >nul 2>&1
if %ERRORLEVEL% NEQ 0 set afterwards=orphan
:skipDotCmp

goto %afterwards%

:orphan
echo    FAILED!
echo    (leaving 'act' folder around if you want to analyze it)
goto end

:cleanup
rem CLEANUP
rmdir /S /Q out > nul 2>&1
rmdir /S /Q act > nul 2>&1

:end
popd
