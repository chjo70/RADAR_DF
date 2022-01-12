#ifndef __COMPLIB_FACADE_H__
#define __COMPLIB_FACADE_H__

#include <math.h>
#include "ExternalIcd.h"
#include "zlib.h"

#define ZLIB_BUFFER_MARGIN		1.001	// 압축시 데이터 크기가 늘어나는 경우도 있음.

class CComplibFacade
{
public :
	CComplibFacade();
	virtual ~CComplibFacade();

	UCHAR* inflate(ULONG *ulResultLen, const UCHAR *ucSrcData, ULONG ulSrcLen);
	UCHAR* deflate(ULONG *ulResultLen, const UCHAR *ucSrcData, ULONG ulSrcLen);

public :
	UCHAR* m_pucInflateBuffer;
	UCHAR* m_pucDeflateBuffer;
	UINT   m_uiBufferSize;

	UCHAR* m_pucRet_inflate;
	UCHAR* m_pucRet_deflate;
};

#endif	// __COMPLIB_FACADE_H__
