/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Apr 02 14:20:17 2002
 */
/* Compiler settings for C:\Work\Acrobat_SDK\depot\Acrobat\SDKs\acroSDKs\acrobat\users\xchang\newsamples\RemoteControlAcrobat\RemoteControlAcrobat.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_RemoteControlAcrobatLib = {0xFC82CBE9,0xF8F6,0x4F87,{0xAE,0xF1,0x15,0x05,0x90,0x01,0x04,0x6C}};


#ifdef __cplusplus
}
#endif

