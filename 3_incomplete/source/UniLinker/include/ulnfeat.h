#ifndef __ulnfeat_h__
#define __ulnfeat_h__

//#undef RESOURCE_STRING_LANG
//#undef RESOURCE_STRING_SUBLANG

//#undef RESOURSE_STRING_NOSMART

//#define NO_ULNFEAT_WARNING

//---------------------------------------------------------
#ifndef _WIN32
#error TLS and Resource realized only on win32 platform
#endif

#ifdef __BORLANDC__
#if __BORLANDC__ < 0x550
#error This features required Borland's compiler version 5.5 or higher
#endif
#ifndef __cplusplus
#error Borland support this features realized ONLY in C++ mode
#endif
#ifndef NO_ULNFEAT_WARNING
#pragma warn +8071
#endif
#elif defined(_MSC_VER)
#if _MSC_VER < 1000
#error This features required Microsoft's compiler version 4.0 or higher
#endif
#ifndef NO_ULNFEAT_WARNING
#pragma warning(1:4309)
#endif
#else
#error This features required Borland, Microsoft or Intel compiler
#endif

//---------------------------------------------------------
#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifdef __cplusplus
#define __EC  extern "C"
#else
#define __EC extern
#endif


#pragma pack(1)

typedef struct {
  unsigned short _flags;
  unsigned short _slength;
  char           _txt[];
}__RSTDCL_A;

typedef struct {
  unsigned short _flags;
  unsigned short _slength;
  wchar_t        _txt[];
}__RSTDCL_W;

#pragma pack()


#define __ECA __EC __declspec(selectany) const


#ifdef _M_X64
#define __NAME(p,n) _ ## p ## _ ## n
#else
#define __NAME(p,n) p ## _ ## n
#endif

#define __RSNAME(n)    __NAME(RESOURCE_STRING, n)
#define __RNAME(n,l,s) __RSNAME(n) ## _ ## l ## _ ## s

#define __ECTD(n)     __ECA PIMAGE_TLS_CALLBACK __NAME(TLS_CALLBACK, n)

#define __ECRA(n,l,s) __ECA __RSTDCL_A __RNAME(n, l, s)
#define __ECRU(n,l,s) __ECA __RSTDCL_W __RNAME(n, l, s)

#define __ECREF(n) __EC void * __RSNAME(n)

//---------------------------------------------------------

#define TLS_CALLBACK(n, f) __ECTD(n) = f

//---------------------------------------------------------
#ifndef RESOURCE_STRING_NOSMART
#define ___rsn 0
#else
#define ___rsn 2
#endif


#define RESOURCE_STRING(n, l, s, t) __ECREF(n); __ECRA(n, l, s) = { \
                    ((MAKELANGID(l, s) & 0) | ___rsn), sizeof(t), t }

#define RESOURCE_STRING_W(n, l, s, t) __ECREF(n); __ECRU(n, l, s) = { \
           ((MAKELANGID(l, s) & 0) | ___rsn | 1), sizeof(t), L ## t }


#define RESOURCE_STRING_DL(n, txt) \
               RESOURCE_STRING(n, RESOURCE_STRING_LANG, \
                                 RESOURCE_STRING_SUBLANG, txt)

#define RESOURCE_STRING_DL_W(n, txt) \
               RESOURCE_STRING_W(n, RESOURCE_STRING_LANG, \
                                 RESOURCE_STRING_SUBLANG, txt)


//---------------------------------------------------------
#define EXTERN_RESOURCE_STRING(n) __ECREF(n)

#ifndef __INTEL_COMPILER
#define RESOURCE_STRING_ID(n) ((UINT)(SIZE_T)&__RSNAME(n))
#else
#define RESOURCE_STRING_ID(n) ((UINT)(0|(SIZE_T)&__RSNAME(n)))
#endif

#endif // __ulnfeat_h__
