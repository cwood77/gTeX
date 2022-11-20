@echo off

rmdir /S /Q out > nul 2>&1

echo --- sanity
bin\out\debug\gTeX.exe > nul

fc out\debug\text\out.txt testdata.txt-out-expected >nul 2>&1
if %ERRORLEVEL% NEQ 0 goto fail

fc out\release\text\out.txt testdata.txt-out-expected >nul 2>&1
if %ERRORLEVEL% NEQ 0 goto fail

echo --- missing label
rem                      dir              exeOut docOut dotOut htmlOut
call tst\.testcase.bat tst\extraLabel     def    none   none   none

echo --- extra label
rem                      dir              exeOut docOut dotOut htmlOut
call tst\.testcase.bat tst\missingLabel   def    none   none   none

echo --- macros
rem                      dir              exeOut docOut dotOut htmlOut
call tst\.testcase.bat tst\macro          none   def    none   none

echo --- counterVar
rem                      dir              exeOut docOut dotOut htmlOut
call tst\.testcase.bat tst\counterVar     none   def    none   none

echo --- inlineAndMerge
rem                      dir              exeOut docOut dotOut htmlOut
call tst\.testcase.bat tst\inlineAndMerge none   def    def   none

echo --- style
rem                      dir              exeOut docOut dotOut htmlOut
call tst\.testcase.bat tst\style          none   def    none   def

echo test done :)

goto end

:fail
echo sanity test FAILED!

:end
