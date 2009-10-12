/*
  Sa7ori Debug...some stuff to help with debugging

*/
#ifndef _SA7DEBUG_H_
#define _SA7DEBUG_H_

#ifdef  __cplusplus
extern "C" {
#endif

// Debug Register 7 struct
typedef struct _DR7 {
    union {
        DWORD Dr7;
        struct {
            BYTE    LocalDr0:1;                 // <-- 0
            BYTE    GlobalDr0:1;                // <-- 1
            BYTE    LocalDr1:1;                 // <-- 2
            BYTE    GlobalDr1:1;                // <-- 3
            BYTE    LocalDr2:1;                 // <-- 4
            BYTE    GlobalDr2:1;                // <-- 5
            BYTE    LocalDr3:1;                 // <-- 6
            BYTE    GlobalDr3:1;                // <-- 7
            BYTE    LocalExact:1;               // <-- 8
            BYTE    GlobalExact:1;              // <-- 9
            BYTE    Reserved0:2;
            BYTE    InterruptRedirection:1;     // <-- 12 UNDOCUMENTED ( IceBp
            BYTE    GlobalDebug:1;
            BYTE    TRace1:1;                   // <-- 14 UNDOCUMENTED (TRace1
            BYTE    TRace2:1;      // <-- 15 UNDOCUMENTED (Trace2
            BYTE    ConditionDr0:2;
            BYTE    LengthDr0:2;
            BYTE    ConditionDr1:2;
            BYTE    LengthDr1:2;
            BYTE    ConditionDr2:2;
            BYTE    LengthDr2:2;
            BYTE    ConditionDr3:2;
            BYTE    LengthDr3:2;
        };
    };
} DR7, *PDR7;

typedef enum _DRX {

    ID_DR0  =   0,  
    ID_DR1,
    ID_DR2,
    ID_DR3

} DRX;

__inline VOID ContinueSingleStepMode(CONTEXT a, HANDLE hThread){
    a.ContextFlags      =   CONTEXT_FULL;//CONTEXT_CONTROL;
    GetThreadContext(hThread, &a);
    a.EFlags            = a.EFlags|0x100;
    SetThreadContext(hThread, &a);
}

__inline VOID ClearSingleStepMode(CONTEXT a, HANDLE hThread){
    a.ContextFlags      =   CONTEXT_FULL;//CONTEXT_CONTROL;
    GetThreadContext(hThread, &a);
    a.EFlags            = a.EFlags&=~0x100;
    SetThreadContext(hThread, &a);
}

__inline VOID ContinueSingleStepModeAndDecEip(CONTEXT a, HANDLE hThread) {
    a.ContextFlags  =   CONTEXT_FULL;
    GetThreadContext(hThread, &a);
    a.EFlags        = a.EFlags|0x100;
    a.Eip--;
    SetThreadContext(hThread, &a);
}

void printDr7(DWORD Dr7){
    DR7 Dr7out;
    Dr7out.Dr7 = Dr7;
    printf("\n***** DR7 *****");
    printf("\nLocalDr0: %.02X", Dr7out.LocalDr0);
    printf("\nGlobalDr0: %.02X", Dr7out.GlobalDr0);
    printf("\nLocalDr1: %.02X", Dr7out.LocalDr1);
    printf("\nGlobalDr1: %.02X", Dr7out.GlobalDr1);
    printf("\nLocalDr2: %.02X", Dr7out.LocalDr2);
    printf("\nGlobalDr2: %.02X", Dr7out.GlobalDr2);
    printf("\nLocalDr3: %.02X", Dr7out.LocalDr3);
    printf("\nGlobalDr3: %.02X", Dr7out.GlobalDr3);
    printf("\nLocalExact: %.02X", Dr7out.LocalExact);
    printf("\nGlobalExact: %.02X", Dr7out.GlobalExact);
    printf("\nReserved0: %.02X", Dr7out.Reserved0);
    printf("\nInterruptRedirection: %.02X", Dr7out.InterruptRedirection);
    printf("\nGlobalDebug: %.02X", Dr7out.GlobalDebug);
    printf("\nTrace1: %.02X", Dr7out.TRace1);
    printf("\nTrace2: %.02X", Dr7out.TRace2);
    printf("\nConditionDr0: %.02X", Dr7out.ConditionDr0);
    printf("\nLengthDr0: %.02X", Dr7out.LengthDr0);
    printf("\nConditionDr1: %.02X", Dr7out.ConditionDr1);
    printf("\nLengthDr1: %.02X", Dr7out.LengthDr1);
    printf("\nConditionDr2: %.02X", Dr7out.ConditionDr2);
    printf("\nLengthDr2: %.02X", Dr7out.LengthDr2);
    printf("\nConditionDr3: %.02X", Dr7out.ConditionDr3);
    printf("\nLengthDr3: %.02X", Dr7out.LengthDr3);
    printf("\n***************");
};

#define GlobalnLocalExcept(a)       a.Dr7 = (a.Dr7 | (3 << 8))
#define ActivateGlobalnLocal(a,Id)  a.Dr7 = (a.Dr7 | (1 << (Id*2)) | (1 << (Id*2+1))) 
#define SetLength(a, Id)       a.Dr7 = (a.Dr7 | (0 << (Id*2+18))) // 0 = 1byte
#define SetCondition(a, Id)   a.Dr7 = (a.Dr7 | (0 << (Id*2+16))) // 0 = execution

#ifdef  __cplusplus
}
#endif

#endif
