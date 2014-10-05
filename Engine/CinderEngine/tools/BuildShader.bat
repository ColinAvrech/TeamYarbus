@echo off
set /p shaderName=Shader Name:
set /p type= Shader Type(vs, frag, geom, cl):

copy /y template\ShaderTemplate.%type% ..\..\Resources\Shaders\%shaderName%.%type%

cd ..\premake
.\premake5 vs2013
pause