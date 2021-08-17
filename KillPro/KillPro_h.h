

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for KillPro.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __KillPro_h_h__
#define __KillPro_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IKillPro_FWD_DEFINED__
#define __IKillPro_FWD_DEFINED__
typedef interface IKillPro IKillPro;

#endif 	/* __IKillPro_FWD_DEFINED__ */


#ifndef __KillPro_FWD_DEFINED__
#define __KillPro_FWD_DEFINED__

#ifdef __cplusplus
typedef class KillPro KillPro;
#else
typedef struct KillPro KillPro;
#endif /* __cplusplus */

#endif 	/* __KillPro_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __KillPro_LIBRARY_DEFINED__
#define __KillPro_LIBRARY_DEFINED__

/* library KillPro */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_KillPro;

#ifndef __IKillPro_DISPINTERFACE_DEFINED__
#define __IKillPro_DISPINTERFACE_DEFINED__

/* dispinterface IKillPro */
/* [uuid] */ 


EXTERN_C const IID DIID_IKillPro;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9f2c5c0d-e5fb-45d2-a011-f1ba402854ef")
    IKillPro : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IKillProVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKillPro * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKillPro * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKillPro * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IKillPro * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IKillPro * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IKillPro * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IKillPro * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IKillProVtbl;

    interface IKillPro
    {
        CONST_VTBL struct IKillProVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKillPro_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKillPro_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKillPro_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKillPro_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IKillPro_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IKillPro_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IKillPro_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IKillPro_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_KillPro;

#ifdef __cplusplus

class DECLSPEC_UUID("a33eff31-ddbc-490e-848d-aaa811bdaf27")
KillPro;
#endif
#endif /* __KillPro_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


