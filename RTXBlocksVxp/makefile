GCC = $(GCC_ARM_NONE_EABI)\arm-none-eabi-gcc.exe
GPP = $(GCC_ARM_NONE_EABI)\arm-none-eabi-g++

INCLUDES = -I "$(MRE_SDK)\include" -I "$(CURDIR)\include" -I "$(CURDIR)\ResID" -I "$(CURDIR)"
DEFINES = -D _MINIGUI_LIB_ -D _USE_MINIGUIENTRY -D _NOUNIX_ -D _FOR_WNC -D __MRE_SDK__ -D   __MRE_VENUS_NORMAL__  -D  __MMI_MAINLCD_240X320__ -D MRE -D GCC -D __MRE_COMPILER_GCC__

SHARED_PARAM = -c -fpic -march=armv5te -fvisibility=hidden -O2 -mlittle-endian $(INCLUDES) $(DEFINES) -fdata-sections -ffunction-sections
GCC_PARAM = -D__MRE_COMPILER_GCC__ $(SHARED_PARAM)
GPP_PARAM = -fno-threadsafe-statics $(SHARED_PARAM) -fno-exceptions -fno-non-call-exceptions

LINK_LIB = -lstdc++ -l:percommon.a -l:pertcp.a -l:persensor.a -l:persmsper.a -l:perbitstream.a -l:percontact.a -l:permms.a -l:persmsmng.a -l:peraudio.a -l:percallout.a -l:percam.a -l:perfile.a -l:perhttp.a -l:persim.a -l:perrecord.a -l:persmssp.a -l:perlbs.a -l:perpro.a -l:pervideo.a -l:perxml.a -l:persec.a -l:persysstorage.a -l:perpayment.a -l:perbt.a -l:perpn.a -l:perudp.a -l:persysfile.a -l:persso.a 
LINK_PARAM = -fno-threadsafe-statics -Wl,--gc-sections  -fpic -fpcc-struct-return --disable-libstdcxx-verbose -pie -lm -T "$(CURDIR)\scat.ld" 

#-Wl,--print-gc-sections -fexceptions

PROJECT_NAME = RTXBlocksVxp

APP_NAME = "RTXBlocks"
AUTHOR = "Ximik_Boda"

build: 
#	Precheck the Resource 
	$(MRE_SDK)\tools\ResEditor\CmdShell.exe SAVE $(CURDIR)\$(PROJECT_NAME).vcproj
	$(GCC) $(GCC_PARAM) -c $(CURDIR)\gccmain.c -o $(CURDIR)\arm\gccmain.o 
	
	$(GCC) $(GCC_PARAM) -c $(CURDIR)\c_fix.c -o $(CURDIR)\arm\c_fix.o
	$(GCC) $(GCC_PARAM) -c $(CURDIR)\..\RTXBlocks\miniz.c -o $(CURDIR)\arm\miniz.o

	
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\cpp_fix.cpp -o $(CURDIR)\arm\cpp_fix.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\mainVxp.cpp -o $(CURDIR)\arm\mainVxp.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\Sock.cpp -o $(CURDIR)\arm\Sock.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\Main.cpp -o $(CURDIR)\arm\Main.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\Protocol.cpp -o $(CURDIR)\arm\Protocol.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\PacketMaker.cpp -o $(CURDIR)\arm\PacketMaker.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\PacketOpener.cpp -o $(CURDIR)\arm\PacketOpener.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\World.cpp -o $(CURDIR)\arm\World.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\Render.cpp -o $(CURDIR)\arm\Render.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\Log.cpp -o $(CURDIR)\arm\Log.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\T2Input.cpp -o $(CURDIR)\arm\T2Input.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\BlockPalette.cpp -o $(CURDIR)\arm\BlockPalette.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\PlayerInfo.cpp -o $(CURDIR)\arm\PlayerInfo.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\Player.cpp -o $(CURDIR)\arm\Player.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\Time.cpp -o $(CURDIR)\arm\Time.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\Chat.cpp -o $(CURDIR)\arm\Chat.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\UUID.cpp -o $(CURDIR)\arm\UUID.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\Keyboard.cpp -o $(CURDIR)\arm\Keyboard.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\Entity.cpp -o $(CURDIR)\arm\Entity.o
	
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\UIBase.cpp -o $(CURDIR)\arm\UIBase.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\UIButton.cpp -o $(CURDIR)\arm\UIButton.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\UIConfigMenu.cpp -o $(CURDIR)\arm\UIConfigMenu.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\UIEngine.cpp -o $(CURDIR)\arm\UIEngine.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\UITextBox.cpp -o $(CURDIR)\arm\UITextBox.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\UIComboBox.cpp -o $(CURDIR)\arm\UIComboBox.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\UIServerEdit.cpp -o $(CURDIR)\arm\UIServerEdit.o
	$(GPP) $(GPP_PARAM) -c $(CURDIR)\..\RTXBlocks\UIRServerEdit.cpp -o $(CURDIR)\arm\UIRServerEdit.o

#	Link app
	$(GCC) -o $(CURDIR)\$(PROJECT_NAME).axf $(CURDIR)\arm\gccmain.o $(CURDIR)\arm\mainVxp.o $(CURDIR)\arm\Main.o $(CURDIR)\arm\c_fix.o $(CURDIR)\arm\miniz.o \
	$(CURDIR)\arm\Sock.o $(CURDIR)\arm\Protocol.o $(CURDIR)\arm\PacketMaker.o $(CURDIR)\arm\PacketOpener.o $(CURDIR)\arm\World.o $(CURDIR)\arm\BlockPalette.o \
	$(CURDIR)\arm\PlayerInfo.o $(CURDIR)\arm\Player.o $(CURDIR)\arm\Time.o $(CURDIR)\arm\Chat.o $(CURDIR)\arm\UUID.o $(CURDIR)\arm\Keyboard.o $(CURDIR)\arm\Render.o \
	$(CURDIR)\arm\Log.o $(CURDIR)\arm\Entity.o $(CURDIR)\arm\T2Input.o $(CURDIR)\arm\UIBase.o $(CURDIR)\arm\UIButton.o $(CURDIR)\arm\UIConfigMenu.o $(CURDIR)\arm\UIEngine.o \
	$(CURDIR)\arm\UITextBox.o $(CURDIR)\arm\UIComboBox.o $(CURDIR)\arm\cpp_fix.o $(CURDIR)\arm\UIServerEdit.o $(CURDIR)\arm\UIRServerEdit.o $(LINK_LIB) $(LINK_PARAM)

#	Pack resource to app 
	"$(MRE_SDK)\tools\ResEditor\CmdShell.exe" pack -silent -resolution 240x320 -o "$(CURDIR)\$(PROJECT_NAME).pkd" -e AXF "$(CURDIR)\$(PROJECT_NAME).vcproj" "$(CURDIR)\$(PROJECT_NAME).axf"

#	Add tag infromation 
	"$(MRE_SDK)\tools\PackDigist.exe" "$(CURDIR)\$(PROJECT_NAME).vcproj" $(APP_NAME) "0.1.0" $(AUTHOR) "3000" "NotSupportBg" "Not Support rotate" $(APP_NAME) $(APP_NAME) $(APP_NAME) "1234567890" "content" " TCP Sensor SMS(person) BitStream Contact MMS SMSMng Audio Call Camera File HTTP SIM card Record SMS(SP) LBS ProMng Video XML Sec SysStorage Payment BT PUSH UDP SysFile sso" "vxp" "GCC" "PKD" "UnCompress" "NoVPI" "Adaptable" "15728960" "UnSysMemAble" "    " "-1" "UNPUSH" "-1" ""  "UnAutoStart"  "UnTransferImg" "NoIdleShortcut"  $(CURDIR)\arm $(CURDIR)\build.log

install:#temp
	copy /y RTXBlocksVxp.vxp "G:\mre\271670312.vxp"

