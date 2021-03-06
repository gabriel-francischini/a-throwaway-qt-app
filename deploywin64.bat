@echo off

:: We can't be sure if folder directory tree
:: is in a dirty state or not, so clean it
echo Clearing previous worktree:

:: If worktree is clean, build/ may not exist so ignore errors
:: CMAKE will recreate this folder in the configuration fase
:: echo     Erasing build/ folder...
:: rmdir /s /q build 2>NUL

:: If worktree is clean, deploy64/ may not exist so ignore errors
echo     Erasing deploy64/ folder...
rmdir /s /q deploy64 2>NUL

:: Recreate the folder
echo     Recreating deploy64/ folder...
mkdir deploy64

:: Clearing step done
echo Worktree cleared.

echo Configuring environment variables for CMAKE...
if not defined QT_HOME (
   set qt_home=C:\Qt\5.12.0
   echo Providing default value to QT_HOME
)

if not defined QT_ARCH (
   set qt_arch=msvc2017_64
   echo Providing default value to QT_ARCH
)

echo QT_HOME is "%QT_HOME%"
echo QT_ARCH is "%QT_ARCH%"

:: To configure CMAKE, we'll need to have echo on
@echo on
@echo Configuring CMAKE...
:: VS15/2017 is the only that supports C++17
cmake -H. -Bbuild ^
      -G "Visual Studio 15 2017 Win64" ^
      -Dtoolchain="desktop" ^
      -Dqt_home="%QT_HOME%" ^
      -Dqt_arch="%QT_ARCH%" ^
      -DCMAKE_BUILD_TYPE="Release"
@echo off

:: With MSVC, build type is defined at build time
echo Building project...
cmake --build build --config Release

:: Deploys app.exe
echo Deploying app.exe target...
echo     Creating deploy folder...
mkdir deploy64\app

echo     Copying target executable...
copy /B build\Release\app.exe /B deploy64\app\app.exe

echo     Running windeployqt.exe over copied file...
"%QT_HOME%\%QT_ARCH%\bin\windeployqt.exe" --qmldir src\ ^
                                          --verbose 0^
                                          --release^
                                          deploy64\app\app.exe
echo     Copying QML files subtree...
mkdir deploy64\app\gui
xcopy src\gui deploy64\app\gui /E

echo     Copying MSVC runtime dependencies...
xcopy "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\redist\x64\Microsoft.VC140.CRT" ^
      deploy64\app /E

:: On Windows 7, this may be necessary
:: see https://stackoverflow.com/questions/31811597/visual-c-2015-redistributable-dlls-for-app-local-deployment
:: see https://blogs.msdn.microsoft.com/vcblog/2015/03/03/introducing-the-universal-crt/
xcopy "C:\Program Files (x86)\Windows Kits\10\Redist\ucrt\DLLs\x64" ^
      deploy64\app /E


:: For some reason, this CRT file needs to be deployed,
:: see https://github.com/dotnet/cli/issues/195
::del deploy64\app\concrt140.dll

:: Copy remaining Qt's .dlls'
copy /B "C:\Qt\5.12.0\msvc2017_64\plugins\platforms\qwindows.dll" ^
     /B deploy64\app\qwindows.dll

:: ZIPs app folder, '-ep' is for true zip format
cd deploy64
"C:\Program Files (x86)\WinRAR\WinRAR.exe" a -afzip ^
            "app.zip" ^
            "app"
cd ..

:: Deploys unit_tests.exe for test
:: in the development machine.
:: DO NOT DEPLOY THIS FOLDER TREE.
echo Deploying unit_tests.exe target...
echo     Creating deploy folder...
mkdir deploy64\unit_tests

echo     Copying target executable...
copy /B build\Release\unit_tests.exe /B deploy64\unit_tests\unit_tests.exe

echo     Running windeployqt.exe over copied file...
"%QT_HOME%\%QT_ARCH%\bin\windeployqt.exe" --qmldir src\ ^
                                          --verbose 0^
                                          --release^
                                          deploy64\unit_tests\unit_tests.exe
echo     Copying QML files subtree...
mkdir deploy64\unit_tests\gui
xcopy src\gui deploy64\unit_tests\gui /E
