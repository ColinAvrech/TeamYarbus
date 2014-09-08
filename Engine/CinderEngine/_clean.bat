@echo off
cd premake
del *.vcxproj*
cd ..
del*.sdf
del *.opensdf
del *.sln
del *.suo
rmdir /S /Q obj
rmdir /S /Q bin
pause