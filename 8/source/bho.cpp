// -------------------
/*
    CSAW 2009 
    Reversing Challenge #8

    S.A. Ridley
    stephen@sa7ori.org
*/
// ------------------

#include <windows.h>
#include <tchar.h>
#include <exdisp.h>
#include <exdispid.h>
#include <mshtml.h>
#include <mshtmdid.h>
#include <shlwapi.h>
#include <string.h>

HINSTANCE hInstance;
LONG gref=0;
const CLSID BhoCLSID = {0xC9C42510,0x9B41,0x42c1,0x9D,0xCD,0x72,0x82,0xA2,0xD0,0x7C,0x61};
#define BhoCLSIDs  _T("{C9C42510-9B41-42c1-9DCD-7282A2D07C61}")



class BHO : public IObjectWithSite, public IDispatch 
{ long ref;
  IWebBrowser2* webBrowser;
  IHTMLDocument* doc; IHTMLDocument2 *doc2;
  IHTMLWindow2 *win2;
public:
  // IUnknown...
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv) {if (riid==IID_IUnknown) *ppv=static_cast<BHO*>(this); else if (riid==IID_IObjectWithSite) *ppv=static_cast<IObjectWithSite*>(this); else if (riid==IID_IDispatch) *ppv=static_cast<IDispatch*>(this); else return E_NOINTERFACE; AddRef(); return S_OK;} 
  ULONG STDMETHODCALLTYPE AddRef() {InterlockedIncrement(&gref); return InterlockedIncrement(&ref);}
  ULONG STDMETHODCALLTYPE Release() {int tmp=InterlockedDecrement(&ref); if (tmp==0) delete this; InterlockedDecrement(&gref); return tmp;}

  // IDispatch...
  HRESULT STDMETHODCALLTYPE GetTypeInfoCount(unsigned int FAR* pctinfo) {*pctinfo=1; return NOERROR;}
  HRESULT STDMETHODCALLTYPE GetTypeInfo(unsigned int iTInfo, LCID lcid, ITypeInfo FAR* FAR*  ppTInfo) {return NOERROR;}
  HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, OLECHAR FAR* FAR* rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId) {return NOERROR;}
 
__inline void tumbler1(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x4e; //N 
    };  
};

__inline void tumbler2(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x49; //I 
    };  
};

__inline void tumbler3(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x41; //A 
    };  
};

__inline void tumbler4(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x56; //V 
    };  
};

__inline void tumbler5(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x55; //U 
    };  
};

  HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR* pDispParams, VARIANT FAR* pVarResult, EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr)
  { 
	char ciphertext[]="\x16\x2d\x20\x65\x36\x20\x29\x29\x36\x65\x06\x16\x04\x12\x36\x65\x27\x3c\x65\x31\x2d\x20\x65\x36\x20\x24\x36\x2d\x2a\x37\x20\x6b";
    // DISPID_DOCUMENTCOMPLETE: This is the earliest point we can obtain the "document" interface
    if (dispIdMember==DISPID_DOCUMENTCOMPLETE){ 
		if (!webBrowser) 
			return E_FAIL; 
      IDispatch *idisp; webBrowser->get_Document(&idisp);
      if (idisp && !doc) 
		  idisp->QueryInterface(IID_IHTMLDocument, (void**)&doc);
      if (idisp && !doc2) 
		  idisp->QueryInterface(IID_IHTMLDocument2, (void**)&doc2);
      if (doc2 && !win2) 
		  doc2->get_parentWindow(&win2);
      IConnectionPointContainer *cpc=0; 
	  if (doc) 
		  doc->QueryInterface(IID_IConnectionPointContainer, (void**) &cpc);
      IConnectionPoint* cp=0; 
	  if (cpc) 
		  cpc->FindConnectionPoint(DIID_HTMLDocumentEvents2, &cp);
      DWORD cookie; 
	  HRESULT hr; 
	  if (cp) 
		  hr=cp->Advise(static_cast<IDispatch*>(this), &cookie);
      if (cp) 
		  cp->Release(); if (cpc) cpc->Release(); if (idisp) idisp->Release();
      if (!doc || !doc2 || !win2 || hr!=S_OK) {
		  release(); return E_FAIL;
	  }
      return NOERROR;
    }

 if (dispIdMember==DISPID_HTMLDOCUMENTEVENTS_ONKEYDOWN)
    { // This shows how to respond to simple events.
		MessageBox(0,_T("Challenge #8 is watching you!"),_T("Challenge #8"),MB_OK); 
      return NOERROR;
    }

/*    if (dispIdMember==DISPID_HTMLDOCUMENTEVENTS_ONCLICK)
    { // This shows how to examine the "event object" of an event
	
      IDispatch *param1=0; if (pDispParams->cArgs==1 && (pDispParams->rgvarg)[0].vt==VT_DISPATCH) param1=(pDispParams->rgvarg)[0].pdispVal;
      IHTMLEventObj *pEvtObj=0; if (param1) param1->QueryInterface(IID_IHTMLEventObj, (void**)&pEvtObj);
      long keycode; HRESULT hr; if (pEvtObj) hr=pEvtObj->get_keyCode(&keycode);
      if (pEvtObj) pEvtObj->Release();
      if (!pEvtObj || hr!=S_OK) return E_FAIL;
      // This shows how to manipulate the CSS style of an element
      int i=keycode-32; if (i<0) i=0; if (i>63) i=63; i*=4;
      wchar_t buf[100]; wsprintfW(buf,L"rgb(%i,%i,%i)",i,255-i,i/2);
      IHTMLElement *body=0; doc2->get_body(&body);
      IHTMLStyle *style=0; if (body) body->get_style(&style);
      VARIANT v; v.vt=VT_BSTR; v.bstrVal=buf;
      if (style) style->put_backgroundColor(v);
      if (style) style->Release(); if (body) body->Release();
      if (!body || !style) return E_FAIL;
	  
      return NOERROR;
    }*/

	if (dispIdMember==DISPID_HTMLDOCUMENTEVENTS_ONCLICK){
		IDispatch *param1=0; if (pDispParams->cArgs==1 && (pDispParams->rgvarg)[0].vt==VT_DISPATCH) param1=(pDispParams->rgvarg)[0].pdispVal;
		IHTMLEventObj *pEvtObj=0; if (param1) param1->QueryInterface(IID_IHTMLEventObj, (void**)&pEvtObj);
		HRESULT hr;
		// This shows how to manipulate the CSS style of an element
		IHTMLElement *body=0; doc2->get_body(&body);
		BSTR URL, site;
		site = SysAllocString(L"dontstuffbeansupyournose");
		doc2->get_URL(&URL);

	   tumbler5((char *)&ciphertext);
	   tumbler3((char *)&ciphertext);
	   tumbler4((char *)&ciphertext);
	   tumbler2((char *)&ciphertext);
	   tumbler1((char *)&ciphertext);

		if (wcsstr(URL,site) != NULL){//it's in there.
			//MessageBox(0,URL,_T("Challenge #8"),MB_OK);
			MessageBoxA(0,(LPCSTR)&ciphertext,"Challenge #8",MB_OK);
		}
		
	}
    return NOERROR;
  }

  // IObjectWithSite...
  HRESULT STDMETHODCALLTYPE GetSite(REFIID riid, void** ppvSite) {return E_NOINTERFACE;}
  HRESULT STDMETHODCALLTYPE SetSite(IUnknown* iunk)
  { // This is called by IE to plug us into the current web window
    release();
    iunk->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser);
    IConnectionPointContainer *cpc=0; iunk->QueryInterface(IID_IConnectionPointContainer, (void**)&cpc);
    IConnectionPoint* cp=0; if (cpc) cpc->FindConnectionPoint(DIID_DWebBrowserEvents2, &cp);
    DWORD cookie; HRESULT hr; if (cp) hr=cp->Advise(static_cast<IDispatch*>(this), &cookie);
    if (!webBrowser || !cpc || !cp || hr!=S_OK) {if (cp) cp->Release(); if (cpc) cpc->Release(); release(); return E_FAIL;}
    return S_OK;
  }

  // BHO...
  BHO() : ref(0), webBrowser(0), doc(0), doc2(0), win2(0) {};
  ~BHO() {release();}
  void release() {if (webBrowser) webBrowser->Release(); webBrowser=0; if (doc) doc->Release(); doc=0; if (doc2) doc2->Release(); doc2=0; if (win2) win2->Release(); win2=0;}

};


class MyClassFactory : public IClassFactory
{ long ref;
  public:
  // IUnknown... (nb. this class is instantiated statically, which is why Release() doesn't delete it.)
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv) {if (riid==IID_IUnknown || riid==IID_IClassFactory) {*ppv=this; AddRef(); return S_OK;} else return E_NOINTERFACE;}
  ULONG STDMETHODCALLTYPE AddRef() {InterlockedIncrement(&gref); return InterlockedIncrement(&ref);}
  ULONG STDMETHODCALLTYPE Release() {int tmp = InterlockedDecrement(&ref); InterlockedDecrement(&gref); return tmp;}
  // IClassFactory...
  HRESULT STDMETHODCALLTYPE LockServer(BOOL b) {if (b) InterlockedIncrement(&gref); else InterlockedDecrement(&gref); return S_OK;}
  HRESULT STDMETHODCALLTYPE CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, LPVOID *ppvObj) {*ppvObj = NULL; if (pUnkOuter) return CLASS_E_NOAGGREGATION; BHO *bho=new BHO(); bho->AddRef(); HRESULT hr=bho->QueryInterface(riid, ppvObj); bho->Release(); return hr;}
  // MyClassFactory...
  MyClassFactory() : ref(0) {}
};


STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppvOut)
{ static MyClassFactory factory; *ppvOut = NULL;
  if (rclsid==BhoCLSID) {return factory.QueryInterface(riid,ppvOut);}
  else return CLASS_E_CLASSNOTAVAILABLE;
}

STDAPI DllCanUnloadNow(void)
{ return (gref>0)?S_FALSE:S_OK;
}

STDAPI DllRegisterServer(void)
{ TCHAR fn[MAX_PATH]; GetModuleFileName(hInstance,fn,MAX_PATH);
  SHSetValue(HKEY_CLASSES_ROOT,_T("CLSID\\")BhoCLSIDs,_T(""),REG_SZ,_T("BHO"),4*sizeof(TCHAR));
  SHSetValue(HKEY_CLASSES_ROOT,_T("CLSID\\")BhoCLSIDs _T("\\InProcServer32"),_T(""),REG_SZ,fn,((int)_tcslen(fn)+1)*sizeof(TCHAR));
  SHSetValue(HKEY_CLASSES_ROOT,_T("CLSID\\")BhoCLSIDs _T("\\InProcServer32"),_T("ThreadingModel"),REG_SZ,_T("Apartment"),10*sizeof(TCHAR));
  SHSetValue(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\")BhoCLSIDs,_T(""),REG_SZ,_T(""),sizeof(TCHAR));
  return S_OK;
}

STDAPI DllUnregisterServer()
{ SHDeleteKey(HKEY_CLASSES_ROOT,_T("CLSID\\") BhoCLSIDs);
  SHDeleteKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\")BhoCLSIDs);
  return S_OK;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{ if (fdwReason==DLL_PROCESS_ATTACH) hInstance=hinstDLL;
  return TRUE;
}

