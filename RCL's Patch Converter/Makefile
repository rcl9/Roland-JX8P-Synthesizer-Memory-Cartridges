#
# Makefile for Visual Studio
#

# NOTE: Use 'unsigned char'

# Name of the Windows executable file 
WIN_EXENAME = convert

OUT = /Fo
OBJ = obj

# -------------------->>>>  Directory Locations  <<<<-----------------------

# Object file directory
OBJ_DIRECTORY = 

all: $(WIN_EXENAME)

# Uncomment for no debugging
NODEBUG = 1

CPU= 

# Include the standard Makefile defines for this specific platform
!include <ntwin32.mak>

!IFNDEF NODEBUG
VC4_OPT = 
!ELSE
cdebug = /Ogityb2 /Gs /Gy
!ENDIF

# _DLL = Used when linking against msvcrt.dll (needed for the DLL modules)
# Warning level 3, ANSI only, Compile w/o linking, unsigned char
VC4_COMMON_CFLAGS = /D_NO_MFC_SUPPORT /D_MT /MT /nologo /W3 /c /J /DOS_WIN32 /DWIN32 /DMSDOS /DWINDOWS3 $(DEBUG_MALLOC) 

CFLAGS = 

VC4_LINK_OPTIONS  = $(lflags) $(linkdebug) /FORCE

# Intel VC++ 4.0
$(WIN_EXENAME): $(OBJ_DIRECTORY)convert.obj 
	$(link) /nologo $(conflags) /OUT:$(WIN_EXENAME).exe $(VC4_LINK_OPTIONS) convert.obj $(guilibsmt) 

$(OBJ_DIRECTORY)convert.obj : convert.cpp 
	 $(cc) $(cflags) $(VC4_COMMON_CFLAGS) $(CFLAGS) $(cvars) $(cdebug) $(OUT)$(OBJ_DIRECTORY)convert.$(OBJ) -c convert.cpp 
