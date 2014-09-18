@echo off
set /p systemName=System Name:

md ..\source\Engine\System\systemName
copy /y template\SystemTemplate.cpp ..\source\Objects_Game\%systemName%.cpp
copy /y template\SystemTemplate.h ..\include\Objects_Game\%systemName%.h

pause