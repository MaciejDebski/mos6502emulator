@ECHO OFF
pushd %~dp0
echo.CD=%CD%
XCOPY /Y ".\x64\Release\mos6502emu.lib" "..\emu_test\lib\mos6502emu\"
XCOPY /Y ".\mos6502emu\mos6502emu.h" "..\emu_test\lib\mos6502emu\"
