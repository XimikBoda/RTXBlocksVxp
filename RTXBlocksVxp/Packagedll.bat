%MRE_SDK%\tools\DllPackage.exe %cd%\RTXBlocksVxp.vcproj
if %errorlevel% == 0 (
 echo postbuild OK.
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)

