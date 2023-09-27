@REM python update_build_version.py

@REM call copytiles.bat

call compile.bat

@if ERRORLEVEL 1 exit /b 1

dir build\*_CODE.bin

call run.bat

