#include <windows.h>
#include <stdio.h>
#include "c_syscalls.h"

int main(){
    HANDLE curHandle = GetCurrentProcess();
    HANDLE handle = NULL;
    OBJECT_ATTRIBUTES oa;
    CLIENT_ID cid;
    NTSTATUS status;
    MEMORY_BASIC_INFORMATION memInfo;


    InitializeObjectAttributes(&oa, NULL, 0, NULL,NULL);

    cid.UniqueProcess = (HANDLE) 13876;
    cid.UniqueThread  = (HANDLE) 0;

    status = Syscall(NT_OPEN_PROCESS, &handle, PROCESS_VM_WRITE | PROCESS_VM_READ, &oa, &cid);
    printf("\n\t[-] NtOpenProcess status: 0x%08x\n", status);

    if (status != 0) {
        return;
    }

    status = Syscall(NT_QUERY_VIRTUAL_MEMORY, curHandle, &cid, 0, &memInfo, sizeof(memInfo), NULL);
    printf("\n\t[-] NtQueryVirtualMemory status: 0x%08x\n", status);
    printf("\t[-] NtQueryVirtualMemory status: 0x%02x\n", memInfo.Protect);
    
    status = Syscall(NT_CLOSE, handle);
    printf("\n\t[-] NtClose status: 0x%08x\n", status);
}