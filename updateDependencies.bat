@echo off

copy /Y /V "dependencies\GDAL\bin\ms110_64_d\gdal110x.dll" x64\Debug
copy /Y /V "dependencies\GDAL\bin\ms110_64_r\gdal110x.dll" x64\Release

if %errorlevel%. equ 0. (
    echo Done
) else (
    echo Copy x y failed due to ...
    pause
)


@echo on