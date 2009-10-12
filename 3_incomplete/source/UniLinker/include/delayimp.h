#ifndef __delayimp_h__
#define __delayimp_h__

/****************************************************************************
 *
 *   structures and prototypes necessary for delay loading of imports.
 *
 *
 *  This technology supports delayed loading of DLLs for arbitrary images.
 *
 *  There are too modeles of Delay Loading Helpers now. It's strongly
 *  recommended to use V2 (new) mode. It linking by default.
 *  If You need to link module in V1 mode use linker switch -d^.
 *  In compatibility mode (using V1 structures) the module can't be linked
 *  with base >= 0x80000000 when imports by name are present.
 *
 *  If some DLL have DelayLoading DLLs and is dynamically unloading
 *  all DelayLoading DLL's also unload automatically.
 *
 *
 *  Restrictions:
 *      You cannot delay loading of a reference to imported data.
 *      You cannot delay loading of KERNEL32.DLL.
 *
 *      You must be very careful with using __FUnloadDelayLoadedDLL in a
 *      multithreaded environment.  It is possible to cause problems
 *      if one thread gets a delay loaded function address from this
 *      module, at the same time as another thread unloads the delayed
 *      load DLL.  There is no way for us to guard against that, so
 *      the user has to be aware of the possibility.
 *
 *  Notes:
 *      TLS support works for delayed load DLLs with OS restriction to
 *      dinamically linked DLLs.
 *
 ****************************************************************************/

//#include <windows.h>  // MUST be included before

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef _DELAY_IMP_VER
#define _DELAY_IMP_VER  2 // msc-compatibility
#endif

#if defined(_WIN64) && _DELAY_IMP_VER < 2
#error  "For PE+ possible only new format of delay-load tables"
#endif

typedef const IMAGE_THUNK_DATA  *LPCIMAGE_THUNK_DATA;
typedef DWORD                   RVA;

typedef enum {                   // Delay Load Attributes
    dlattrRva = 0x1,             // RVAs are used instead of pointers
}DLAttr;


typedef struct ImgDelayDescr {
    union { // DLAttr bits
      DWORD               dwAttributes; // bc-style
      DWORD               grAttrs;      // ms-style
    };
    union {
#ifndef _WIN64
      LPCSTR              szDllName;    // V1
#endif
      RVA                 rvaDLLName;   // V2
    };
    union {
#ifndef _WIN64
      HMODULE             *phmod;       // V1
#endif
      RVA                 rvaHmod;      // V2
    };
    union {
#ifndef _WIN64
      PIMAGE_THUNK_DATA   pIAT;         // V1
#endif
      RVA                 rvaIAT;       // V2
    };
    union {
#ifndef _WIN64
      LPCIMAGE_THUNK_DATA pINT;         // V1
#endif
      RVA                 rvaINT;       // V2
    };
    union {
#ifndef _WIN64
      LPCIMAGE_THUNK_DATA pBoundIAT;    // V1
#endif
      RVA                 rvaBoundIAT;  // V2
    };
    union {
#ifndef _WIN64
      LPCIMAGE_THUNK_DATA pUnloadIAT;   // V1
#endif
      RVA                 rvaUnloadIAT; // V2
    };
    DWORD                 dwTimeStamp;
}ImgDelayDescr, *PImgDelayDescr;
typedef const ImgDelayDescr *LPCImgDelayDescr;


// Delay load import hook notifications
typedef enum {
    dliNoteStartProcessing,   // used to bypass or note helper only
    dliStartProcessing = dliNoteStartProcessing,  // ms-alias
    dliNotePreLoadLibrary,    // called just before LoadLibrary, can
                              //  override w/ new HMODULE return val
    dliNotePreGetProcAddress, // called just before GetProcAddress, can
                              //  override w/ new FARPROC return value
    dliFailLoadLibrary,       // failed to load library, fix it by
                              //  returning a valid HMODULE
    dliFailGetProcAddress,    // failed to get proc address, fix it by
                              //  returning a valid FARPROC
    dliNoteEndProcessing,     // called after all processing is done,
                              //  no bypass possible at this point
                              //  except by longjmp(), throw(), or RaiseException.
}dliNotification;


typedef struct DelayLoadProc {
    BOOL      fImportByName;
    union {
      LPCSTR  szProcName;
      DWORD   dwOrdinal;
    };
}DelayLoadProc;

typedef struct DelayLoadInfo {
    DWORD             cb;         // size of structure
    LPCImgDelayDescr  pidd;       // raw form of data (everything is there)
    FARPROC           *ppfn;      // points to address of function to load
    LPCSTR            szDll;      // name of dll
    DelayLoadProc     dlp;        // name or ordinal of procedure
    HMODULE           hmodCur;    // the hInstance of the library we have loaded
    FARPROC           pfnCur;     // the actual function that will be called
    DWORD             dwLastError;// error received (if an error notification)
}DelayLoadInfo, *PDelayLoadInfo;

typedef FARPROC (WINAPI *DelayedLoadHook)(dliNotification dliNotify,
                                          PDelayLoadInfo pdli);
typedef DelayedLoadHook PfnDliHook; // ms-alias

//---------------------------------------------------------------------------
// Unload support
// takes a pointer to a name to unload, or NULL to unload all the delay loaded
// DLLs in the list.

#if defined(_MSC_VER) && _MSC_VER >= 1300 && _DELAY_IMP_VER >= 2
BOOL WINAPI __FUnloadDelayLoadedDLL2(LPCSTR szDll);
#else
BOOL WINAPI __FUnloadDelayLoadedDLL(LPCSTR szDll);
#endif

#if _DELAY_IMP_VER >= 2

// Snap load support (NULL not accepted :)
HRESULT WINAPI __HrLoadAllImportsForDll(LPCSTR szDll);

// when used new form (RVA) this address can be used as load address
extern const IMAGE_DOS_HEADER __ImageBase;

#endif // _DELAY_IMP_VER

//--------------------------------------------------------------------------
// hooks

/* The "notify hook" gets called for every call to the
   delay load helper.  This allows a user to hook every call and
   skip the delay load helper entirely.

   dliNotify ==
   {
       dliNoteStartProcessing   |
       dliNotePreLoadLibrary    |
       dliNotePreGetProcAddress |
       dliNoteEndProcessing
   }

   on this call.
 */
#if defined(_MSC_VER) && _MSC_VER >= 1300 && _DELAY_IMP_VER >= 2
extern DelayedLoadHook __pfnDliNotifyHook2;
#else
extern DelayedLoadHook __pfnDliNotifyHook;
#endif

/* This is the failure hook,

   dliNotify ==
   {
       dliFailLoadLibrary       |
       dliFailGetProcAddress
   }
*/
#if defined(_MSC_VER) && _MSC_VER >= 1300 && _DELAY_IMP_VER >= 2
extern DelayedLoadHook __pfnDliFailureHook2;
#else
extern DelayedLoadHook __pfnDliFailureHook;
#endif

//--------------------------------------------------------------------------
#ifdef  __cplusplus
} // extern "C" {
#endif

#if _DELAY_IMP_VER >= 2

#if defined(__cplusplus) && !defined(DO_NOT_USE_TEMPLATES)

// you can use next macros and templates for convert's RVA to addr's
template<typename X> struct __PFromRVA {
static inline X *PFromRVA(RVA rva)
{
    return((X*)((LPBYTE)&__ImageBase + rva));
}
};
#define PFromRVA(rva,typ)  __PFromRVA<typ>::PFromRVA(rva)

#else   // defined(__cplusplus) && !defined(DO_NOT_USE_TEMPLATES)

#define PFromRVA(rva,typ) ((typ*)((LPBYTE)&__ImageBase + rva))

#endif // defined(__cplusplus) && !defined(DO_NOT_USE_TEMPLATES)

#endif // _DELYAY_IMP_VER

//---------------------------------------------------------------------------
// Exception information
//
#define FACILITY_UNILINK    ((LONG)0xec)

// when linked with another linker's
#define FACILITY_VISUALCPP  ((LONG)0x6d)
#define FACILITY_BORLANDCPP ((LONG)0xfb)

// make exception code
#define MakeExcCode(sev,fac,err)  ((sev) | (fac<<16) | err)

#endif // __delayimp_h__

