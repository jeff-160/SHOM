@echo off
setlocal enabledelayedexpansion

goto :main

:print
    powershell -command "Write-host %1 -ForegroundColor %2"
    exit /b

:main
    set "params=%*"
    cd /d "%~dp0" && ( if exist "%temp%\getadmin.vbs" del "%temp%\getadmin.vbs" ) && fsutil dirty query %systemdrive% 1>nul 2>nul || (  echo Set UAC = CreateObject^("Shell.Application"^) : UAC.ShellExecute "cmd.exe", "/c cd ""%~sdp0"" && %~s0 %params%", "", "runas", 1 >> "%temp%\getadmin.vbs" && "%temp%\getadmin.vbs" && exit /B )

    for /f "tokens=3 delims=\" %%a in ("!cd!") do set "username=%%a"
    set "NEWUSER=C:\Users\%username%"
    set "dir=%NEWUSER%\SHOM"

    call :print "Copying objects to %dir%" Yellow
    rmdir /s /q "%dir%" > nul 2> nul
    xcopy %cd% "%dir%" /E /I /Y > nul
    if %errorlevel% neq 0 (
        call :print "Failed to copy objects" Red
        goto :end
    ) else (
        call :print "Objects copied successfully" Green
    )
    echo. 

    cd /d "%dir%"

    call :print "Adding path to system environment variables" Yellow
    echo %PATH% | find /i "%cd%" > nul
    if errorlevel 1 (
        setx /m PATH "%PATH%;%cd%" > nul
        call :print "%cd% added to system environment variables" Green
    ) else (
        call :print "%cd% already exists in system environment variables" Yellow
    )
    echo.

:end
    pause