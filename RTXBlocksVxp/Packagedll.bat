%MRE_SDK%\tools\DllPackage.exe %cd%\RTXBlocksVxp.vcproj
if %errorlevel% == 0 (
 echo postbuild OK.
  copy RTXBlocksVxp.vpp ..\..\..\MoDIS_VC9\WIN32FS\DRIVE_E\RTXBlocksVxp.vpp /y
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)

