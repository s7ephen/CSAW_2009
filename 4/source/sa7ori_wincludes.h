#include <sys/stat.h>
#include <sys/types.h>
//#include <Winbase.h>
//#include <winbase.h>

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include <winnt.h>
//#include <ntstatus.h>
//#include <ntdef.h>
//#include <psapi.h>


#define HUN 100
#define SPACE " "

//export prototypes
void wtf_error();
char pr1nt(const int *indent);

//------
typedef DWORD NTSTATUS;
typedef unsigned int uint32; //Im loozin it.
typedef unsigned short uint16;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;

typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;

typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;


typedef LONG KPRIORITY;

typedef unsigned long ULONG_PTR, *PULONG_PTR;

struct _PEB_50 //sizes taken mostly from windbg's "dt"
{
    BYTE     InheritedAddressSpace; //+000
    BYTE     ReadImageFileExecOptions; //+001
    BYTE     BeingDebugged; //+002
    BYTE     SpareBool; //+003
    void     *Mutant; //+004
    void     *ImageBaseAddress; //+008
    struct   _PEB_LDR_DATA *Ldr; //+00c
    struct   _RTL_USER_PROCESS_PARAMETERS *ProcessParameters; //+010
    void     *SubSystemData; //+014
    void     *ProcessHeap; //+018
    void     *FastPebLock; //+01c
    void     *FastPebLockRoutine; //+020
    void     *FastPebUnlockRoutine; //+024
    uint32   EnvironmentUpdateCount; //+028
    void     *KernelCallbackTable; //+02c
    uint32   SystemReserved[2]; //+030
    struct   _PEB_FREE_BLOCK *FreeList; //+038
    uint32   TlsExpansionCounter; //+03c
    void     *TlsBitmap; //+040
    uint32   TlsBitmapBits[2]; //+044
    void     *ReadOnlySharedMemoryBase; //+04c
    void     *ReadOnlySharedMemoryHeap; //+050
    void     **ReadOnlyStaticServerData; //+054
    void     *AnsiCodePageData; //+058
    void     *OemCodePageData; //+05c
    void     *UnicodeCaseTableData; //+060
    uint32   NumberOfProcessors; //+064
    uint32   NtGlobalFlag; //+068
    uint32   Filler_6C; //+6c
    LARGE_INTEGER CriticalSectionTimeout; //+070
    uint32   HeapSegmentReserve; //+078
    uint32   HeapSegmentCommit; //+07c
    uint32   HeapDeCommitTotalFreeThreshold; //+080
    uint32   HeapDeCommitFreeBlockThreshold; //+084
    uint32   NumberOfHeaps; //+088
    uint32   MaximumNumberOfHeaps; //+08c
    void     **ProcessHeaps; //+090
    void     *GdiSharedHandleTable; //+094
    void     *ProcessStarterHelper; //+098
    uint32   GdiDCAttributeList; //+09c
    void     *LoaderLock; //+0a0
    uint32   OSMajorVersion; //+0a4
    uint32   OSMinorVersion; //+0a8
    uint16   OSBuildNumber; //+0ac
    uint16   OSCSDVersion; //+0ae
    uint32   OSPlatformId; //+0b0
    uint32   ImageSubsystem; //+0b4
    uint32   ImageSubsystemMajorVersion; //+0b8
    uint32   ImageSubsystemMinorVersion; //+0bc
    uint32   ImageProcessAffinityMask; //+0c0
    uint32   GdiHandleBuffer[34]; //+0c4
    void *PostProcessInitRoutine; //+14c //fp
    void     *TlsExpansionBitmap; //+150
    uint32   TlsExpansionBitmapBits[32]; //+154
    uint32   SessionId; //+1d4
    void     *AppCompatInfo; //+1d8
    UNICODE_STRING CSDVersion; //+1dc
};

typedef struct _PEB_50 PEB;//probably shouldn't, but...
typedef PEB* PPEB;

typedef struct _PROCESS_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    KPRIORITY BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

typedef struct _CLIENT_ID {
    ULONG       UniqueProcess;
    ULONG       UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation,              // 0        Y        N
    SystemProcessorInformation,          // 1        Y        N
    SystemPerformanceInformation,        // 2        Y        N
    SystemTimeOfDayInformation,          // 3        Y        N
    SystemNotImplemented1,               // 4        Y        N
    SystemProcessesAndThreadsInformation, // 5       Y        N
    SystemCallCounts,                    // 6        Y        N
    SystemConfigurationInformation,      // 7        Y        N
    SystemProcessorTimes,                // 8        Y        N
    SystemGlobalFlag,                    // 9        Y        Y
    SystemNotImplemented2,               // 10       Y        N
    SystemModuleInformation,             // 11       Y        N
    SystemLockInformation,               // 12       Y        N
    SystemNotImplemented3,               // 13       Y        N
    SystemNotImplemented4,               // 14       Y        N
    SystemNotImplemented5,               // 15       Y        N
    SystemHandleInformation,             // 16       Y        N
    SystemObjectInformation,             // 17       Y        N
    SystemPagefileInformation,           // 18       Y        N
    SystemInstructionEmulationCounts,    // 19       Y        N
    SystemInvalidInfoClass1,             // 20
    SystemCacheInformation,              // 21       Y        Y
    SystemPoolTagInformation,            // 22       Y        N
    SystemProcessorStatistics,           // 23       Y        N
    SystemDpcInformation,                // 24       Y        Y
    SystemNotImplemented6,               // 25       Y        N
    SystemLoadImage,                     // 26       N        Y
    SystemUnloadImage,                   // 27       N        Y
    SystemTimeAdjustment,                // 28       Y        Y
    SystemNotImplemented7,               // 29       Y        N
    SystemNotImplemented8,               // 30       Y        N
    SystemNotImplemented9,               // 31       Y        N
    SystemCrashDumpInformation,          // 32       Y        N
    SystemExceptionInformation,          // 33       Y        N
    SystemCrashDumpStateInformation,     // 34       Y        Y/N
    SystemKernelDebuggerInformation,     // 35       Y        N
    SystemContextSwitchInformation,      // 36       Y        N
    SystemRegistryQuotaInformation,      // 37       Y        Y
    SystemLoadAndCallImage,              // 38       N        Y
    SystemPrioritySeparation,            // 39       N        Y
    SystemNotImplemented10,              // 40       Y        N
    SystemNotImplemented11,              // 41       Y        N
    SystemInvalidInfoClass2,             // 42
    SystemInvalidInfoClass3,             // 43
    SystemTimeZoneInformation,           // 44       Y        N
    SystemLookasideInformation,          // 45       Y        N
    SystemSetTimeSlipEvent,              // 46       N        Y
    SystemCreateSession,                 // 47       N        Y
    SystemDeleteSession,                 // 48       N        Y
    SystemInvalidInfoClass4,             // 49
    SystemRangeStartInformation,         // 50       Y        N
    SystemVerifierInformation,           // 51       Y        Y
    SystemAddVerifier,                   // 52       N        Y
    SystemSessionProcessesInformation    // 53       Y        N
} SYSTEM_INFORMATION_CLASS;

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,          // Note: this is kernel mode only
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    ProcessDeviceMap,
    ProcessSessionInformation,
    ProcessForegroundInformation,
    ProcessWow64Information,
    MaxProcessInfoClass
    } PROCESSINFOCLASS;

typedef LONG KPRIORITY;

typedef unsigned long ULONG_PTR, *PULONG_PTR;

typedef struct _VM_COUNTERS {
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
} VM_COUNTERS;

typedef VM_COUNTERS *PVM_COUNTERS;

typedef struct _IO_COUNTERSEX {
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
} IO_COUNTERSEX, *PIO_COUNTERSEX;

typedef enum {
    StateInitialized,
    StateReady,
    StateRunning,
    StateStandby,
    StateTerminated,
    StateWait,
    StateTransition,
    StateUnknown
} THREAD_STATE;

typedef enum _KWAIT_REASON {
    Executive,
    FreePage,
    PageIn,
    PoolAllocation,
    DelayExecution,
    Suspended,
    UserRequest,
    WrExecutive,
    WrFreePage,
    WrPageIn,
    WrPoolAllocation,
    WrDelayExecution,
    WrSuspended,
    WrUserRequest,
    WrEventPair,
    WrQueue,
    WrLpcReceive,
    WrLpcReply,
    WrVirtualMemory,
    WrPageOut,
    WrRendezvous,
    Spare2,
    Spare3,
    Spare4,
    Spare5,
    Spare6,
    WrKernel,
    MaximumWaitReason
    } KWAIT_REASON;


typedef struct _SYSTEM_THREADS {
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    ULONG WaitTime;
    PVOID StartAddress;
    CLIENT_ID ClientId;
    KPRIORITY Priority;
    KPRIORITY BasePriority;
    ULONG ContextSwitchCount;
    THREAD_STATE State;
    KWAIT_REASON WaitReason;
} SYSTEM_THREADS, *PSYSTEM_THREADS;

typedef struct _SYSTEM_PROCESSES { // Information Class 5
    ULONG NextEntryDelta;
    ULONG ThreadCount;
    ULONG Reserved1[6];
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ProcessName;
    KPRIORITY BasePriority;
    ULONG ProcessId;
    ULONG InheritedFromProcessId;
    ULONG HandleCount;
    ULONG Reserved2[2];
    VM_COUNTERS VmCounters;
    IO_COUNTERSEX IoCounters;  // Windows 2000 only
    SYSTEM_THREADS Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;


#define MAX_VALUE_NAME 16383
#define MAX_KEY_LENGTH 255


#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)

typedef struct _SYSTEM_HANDLE {
        ULONG ProcessId;
        UCHAR ObjectTypeNumber;
        UCHAR Flags;
        USHORT Handle;
        PVOID Object;
        ACCESS_MASK GrantedAccess;
}SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
        ULONG count;
        SYSTEM_HANDLE handle[1]; //array of count size
}SYSTEM_HANDLE_INFORMATION,     *PSYSTEM_HANDLE_INFORMATION;


typedef NTSTATUS (NTAPI *tZwQuerySystemInformation)(
        IN SYSTEM_INFORMATION_CLASS     SystemInformationClass,
        OUT PVOID                       pSystemInformation,
        IN ULONG                        uSystemInformationLength,
        OUT PULONG                  puReturnLength OPTIONAL
        );

void wtf_error() {
//This function  a failed attempt at an indentation wrapper for basic output.
	LPVOID lpMsgBuf;
	int err = GetLastError();
	FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
	);
	printf("\t[!] Error (%d): %s\n", err, (LPCTSTR)lpMsgBuf);
	LocalFree( lpMsgBuf );
};

/*
INCOMPLETE
//input: number of indentations
//output: indentations and line header for easy readable output.
char pr1nt(const int *indent) {
	int *n; char *output[HUN];
	if (indent >= 10) {indent = 10;}; //knock it down some.
	
	for (n=0;n<=indent;n++) {
		sprintf(output, "\t"); //indent one
	};
	if ((*indent % 2) == 0) { //then it is even and we print [+]
		sprintf(output, "[+] ");
	} else { //then it is odd and we print [-]
		sprintf(output, "[-] ");
	};
	return (output);
};
*/
