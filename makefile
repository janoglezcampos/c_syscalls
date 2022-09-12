CC = cl

CFLAGS =
LFLAGS = 

SOURCE = src/*.c

ASMIN_INDIRECT_x64 = src/asm/indirect.x64.asm
ASMIN_DIRECT_x64 = src/asm/direct.x64.asm
ASMIN_INDIRECT_x86 = src/asm/indirect.x86.asm
ASMIN_DIRECT_x86 = src/asm/direct.x86.asm

BIN_FOLDER = bin

ASMOUT = $(BIN_FOLDER)\stub.obj
OUT = $(BIN_FOLDER)\SyscallTest.exe

direct_x64: 	clean create_out_folder asm_direct_x64   compile clean_obj
indirect_x64:	clean create_out_folder asm_indirect_x64 compile clean_obj
direct_x86: 	clean create_out_folder asm_direct_x86   compile clean_obj
indirect_x86:	clean create_out_folder asm_indirect_x86 compile clean_obj

asm_indirect_x64:
	nasm -f win64 $(ASMIN_INDIRECT_x64) -o $(ASMOUT)

asm_direct_x64:
	nasm -f win64 $(ASMIN_DIRECT_x64) -o $(ASMOUT)

asm_indirect_x86:
	nasm -f win32 $(ASMIN_INDIRECT_x86) -o $(ASMOUT)

asm_direct_x86:
	nasm -f win32 $(ASMIN_DIRECT_x86) -o $(ASMOUT)

compile:
	$(CC) $(SOURCE) $(ASMOUT) /Fo:$(BIN_FOLDER)\ /Fe: $(OUT) $(CFLAGS) $(LFLAGS) 

clean:
!IF EXISTS($(BIN_FOLDER))
	del /q $(BIN_FOLDER)\*
!ENDIF

clean_obj:
	del /q $(BIN_FOLDER)\*.obj

create_out_folder:
!IF !EXISTS($(BIN_FOLDER))
	mkdir $(BIN_FOLDER)
!ENDIF