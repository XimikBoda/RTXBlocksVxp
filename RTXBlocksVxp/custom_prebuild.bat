REM custom_prebuild.bat 
REM this is for call the ResEditor.exe to save the project resource, to generate ResID.h
REM  for avoid ResID.h is modified after build, this will effect debug

echo %MRE_SDK%\models\Model01_QVGA_MRE3.0\MoDIS\Release > RTXBlocksVxp.ini
%MRE_SDK%\tools\ResEditor\CmdShell.exe SAVE RTXBlocksVxp.vcproj
if %errorlevel% == 0 (
 echo prebuild OK.
 exit 0
) else (
 echo prebuild error
 echo error code: %errorlevel%
 exit 1
)
