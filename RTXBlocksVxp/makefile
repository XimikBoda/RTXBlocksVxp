GCC = $(GCC_ARM_NONE_EABI)\arm-none-eabi-gcc.exe
GPP = $(GCC_ARM_NONE_EABI)\arm-none-eabi-g++

INCLUDES = -I "$(MRE_SDK)\include" -I "$(CURDIR)\include" -I "$(CURDIR)\ResID" -I "$(CURDIR)"
DEFINES = -D _MINIGUI_LIB_ -D _USE_MINIGUIENTRY -D _NOUNIX_ -D _FOR_WNC -D __MRE_SDK__ -D   __MRE_VENUS_NORMAL__  -D  __MMI_MAINLCD_240X320__ -D MRE

SHARED_PARAM = -c -fpic -march=armv5te -fvisibility=hidden -O2 -mlittle-endian $(INCLUDES) $(DEFINES)
GCC_PARAM = -fno-exceptions -fno-non-call-exceptions -D__MRE_COMPILER_GCC__ $(SHARED_PARAM)
GPP_PARAM = -fno-threadsafe-statics -fdata-sections -ffunction-sections $(SHARED_PARAM)

LINK_LIB = -l:percommon.a -l:pertcp.a -l:persensor.a -l:persmsper.a -l:perbitstream.a -l:percontact.a -l:permms.a -l:persmsmng.a -l:perfile.a -l:perudp.a
LINK_PARAM = -fno-threadsafe-statics -Wl,--gc-sections -Wl,--print-gc-sections -fpic -fpcc-struct-return -pie -lm -T "$(MRE_SDK)\lib\MRE30\armgcc\scat.ld"


PROJECT_NAME = RTXBlocksVxp

build: 
#	Precheck the Resource 
	$(MRE_SDK)\tools\ResEditor\CmdShell.exe SAVE $(CURDIR)\$(PROJECT_NAME).vcproj
	$(GCC) $(GCC_PARAM)  -c $(MRE_SDK)\lib\MRE30\src\gccmain.c -o $(CURDIR)\arm\gccmain.o 

	$(GCC) $(GCC_PARAM) -c $(CURDIR)\c_fix.c -o $(CURDIR)\arm\c_fix.o

	$(GPP) $(GPP_PARAM) -c $(CURDIR)\main.cpp -o $(CURDIR)\arm\main.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\Keyboard.cpp -o $(CURDIR)\arm\Keyboard.o

#	Link app
	$(GCC) -o "$(CURDIR)\$(PROJECT_NAME).axf" $(CURDIR)\arm\c_fix.o $(CURDIR)\arm\main.o $(CURDIR)\arm\Keyboard.o $(LINK_LIB) $(LINK_PARAM)

#	Pack resource to app 
	"$(MRE_SDK)\tools\ResEditor\CmdShell.exe" pack -silent -resolution 240x320 -o "$(CURDIR)\$(PROJECT_NAME).pkd" -e AXF "$(CURDIR)\$(PROJECT_NAME).vcproj" "$(CURDIR)\$(PROJECT_NAME).axf"

#	Add tag infromation 
	"$(MRE_SDK)\tools\PackDigist.exe" "$(CURDIR)\$(PROJECT_NAME).vcproj" "RTXBlocks" "0.1.0" "Ximik_Boda" "2500" "NotSupportBg" "Not Support rotate" "RTXBlocks" "RTXBlocks" "RTXBlocks" "1234567890" "content" " TCP Sensor SMS(person) BitStream Contact MMS SMSMng" "vxp" "GCC" "PKD" "UnCompress" "NoVPI" "Adaptable" "15728960" "UnSysMemAble" "    " "-1" "UNPUSH" "-1" ""  "UnAutoStart"  "UnTransferImg" "NoIdleShortcut"  $(CURDIR)\arm $(CURDIR)\build.log


#copy /y RTXBlocksVxp.vxp "G:\mre\271670312.vxp"