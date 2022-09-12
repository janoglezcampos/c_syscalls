#include "c_syscalls.h"

#define U_PTR(x)		((DWORD_PTR)x)
#define C_PTR(x)		((PVOID)x)

#ifdef _M_IX86

__declspec(naked) BOOL LocalIsWow64()
{
    __asm {
        mov eax, fs:[0xc0]
        test eax, eax
        jne wow64
        mov eax, 0
        ret
        wow64:
        mov eax, 1
        ret
    }
}

#else

BOOL LocalIsWow64()
{
    return FALSE;
}

#endif

ULONG HashString( PVOID Inp, ULONG Len )
{
	ULONG  hsh;
	PUCHAR ptr;
	UCHAR  cur;

	hsh = 5381;
	ptr = Inp;

	while ( TRUE )
	{
		cur = * ptr;

		if ( ! Len ) {
			if ( ! * ptr ) {
				break;
			};
		} else {
			if ( ( ULONG )( ptr - ( PUCHAR )Inp ) >= Len ) {
				break;
			};
			if ( ! * ptr ) {
				++ptr; continue;
			};
		};

		if ( cur >= 'a' )
			cur -= 0x20;

		hsh = ((hsh << 5) + hsh) + cur; ++ptr;
	};
	return hsh;
};

PVOID PebGetModule( IN ULONG Hsh )
{
	PPEB                  peb;
	PPEB_LDR_DATA         ldr;
	PLDR_DATA_TABLE_ENTRY dte;
	PLIST_ENTRY           ent;
	PLIST_ENTRY           hdr;
	ULONG                 mod;

	peb = NtCurrentTeb()->ProcessEnvironmentBlock;
	ldr = peb->Ldr;
	hdr = & ldr->InLoadOrderModuleList;
	ent = hdr->Flink;

	for ( ; hdr != ent ; ent = ent->Flink ) {
		dte = C_PTR( ent );
		mod = HashString( dte->BaseDllName.Buffer, dte->BaseDllName.Length );

		if ( mod == Hsh ) {
			return C_PTR( dte->DllBase );
		};
	};
	return NULL;
}; 

PVOID PeGetFuncEat( PVOID Ptr, ULONG Hsh )
{
	PIMAGE_DOS_HEADER       dos;
	PIMAGE_NT_HEADERS       nth;
	PIMAGE_DATA_DIRECTORY   dir;
	PIMAGE_EXPORT_DIRECTORY exp;
	PDWORD                  aof;
	PDWORD                  aon;
	PUSHORT                 ano;
	PCHAR                   str;
	DWORD                   cnt;
	ULONG                   hxp;

	dos = C_PTR( Ptr );
	nth = C_PTR( U_PTR(dos + dos->e_lfanew ));
	dir = C_PTR( &nth->OptionalHeader.DataDirectory[0] );

	if ( dir->VirtualAddress ) {
		exp = C_PTR( U_PTR(dos + dir->VirtualAddress ));
		aof = C_PTR( U_PTR(dos + exp->AddressOfFunctions ));
		aon = C_PTR( U_PTR(dos + exp->AddressOfNames ));
		ano = C_PTR( U_PTR(dos + exp->AddressOfNameOrdinals ));

		for( cnt=0;cnt<exp->NumberOfNames;++cnt ) {	
			str = C_PTR( U_PTR(dos + aon[cnt] ));
			hxp = HashString(str, 0);

			if ( hxp == Hsh ) {
				return C_PTR( U_PTR(dos + aof[ano[cnt]] ));
			};
		};
	};
	return NULL;
};

USHORT GetSsn(int hash, PVOID* addr){
	PVOID ntdll_addr;
	PVOID funct_addr;
	PVOID ssn_addr;
	USHORT ssn;

	ntdll_addr = PebGetModule(NTDLL);
	funct_addr = PeGetFuncEat(ntdll_addr, hash);
	ssn = *(PUSHORT)((DWORD_PTR) funct_addr + SYSCALL_SSN_DIST);

	if(addr != NULL) {
		if(LocalIsWow64()){
			*addr = (PVOID)((DWORD_PTR)funct_addr + SYSCALL_INST_DIST_WOW64);
		} else {
			*addr = (PVOID)((DWORD_PTR)funct_addr + SYSCALL_INST_DIST);
		}
	}

	return ssn;
}