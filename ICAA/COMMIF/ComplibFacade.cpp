#include "stdafx.h"
#include "ComplibFacade.h" 

CComplibFacade::CComplibFacade()
{
	m_uiBufferSize = compressBound(SZ_MAX_MSG_LEN * ZLIB_BUFFER_MARGIN);
	TRACE1("buffer size:%d\n", m_uiBufferSize);

	m_pucInflateBuffer = new UCHAR[m_uiBufferSize];
	if(m_pucInflateBuffer == NULL)
	{
		TRACE0("Memory Alloc Fail\n");
	}

	m_pucDeflateBuffer = new UCHAR[m_uiBufferSize];
	if(m_pucDeflateBuffer == NULL)
	{
		TRACE0("Memory Alloc Fail\n");
	}

	m_pucRet_deflate = nullptr;
	m_pucRet_inflate = nullptr;
}

CComplibFacade::~CComplibFacade()
{
	if(m_pucInflateBuffer!=NULL)
	{
		delete m_pucInflateBuffer;
		m_pucInflateBuffer = NULL;
	}

	if(m_pucDeflateBuffer!=NULL)
	{
		delete m_pucDeflateBuffer;
		m_pucDeflateBuffer = NULL;
	}
}

UCHAR* CComplibFacade::inflate(ULONG *ulResultLen, const UCHAR *ucSrcData, ULONG ulSrcLen)
{
	if ( ucSrcData == nullptr )
	{//DTEC_NullPointCheck		
		m_pucRet_inflate = nullptr;		
	}
	else
	{
		// [주의] ulDestLen은 compress2 수행 후 받는 결과값(압축후 데이터 크기)이나 적절하게 큰 값을 넣어주지 않으면 에러 발생함.
		*(ulResultLen) = m_uiBufferSize;

		int result;
		result = uncompress(m_pucInflateBuffer, ulResultLen, ucSrcData, ulSrcLen);

		if(result == Z_OK)		// if success
		{
			m_pucRet_inflate = m_pucInflateBuffer;
		}
		else if(result == Z_MEM_ERROR)	// if there was not enough memory
		{ //DTEC_Else
			m_pucRet_inflate = nullptr;	
			TRACE1("not enough memory:%d\n", result);			
		}
		else if(result == Z_BUF_ERROR)	// if there was not enough room in the output buffer
		{//DTEC_Else
			m_pucRet_inflate = nullptr;	
			TRACE1("not enough room in the output buffer:%d\n", result);			
		}
		else if(result == Z_DATA_ERROR)	// input data was corrupted or incomplete
		{//DTEC_Else
			m_pucRet_inflate = nullptr;	
			TRACE1("input data was corrupted or incomplete:%d\n", result);			
		}
		else	// if unknown error
		{//DTEC_Else
			m_pucRet_inflate = nullptr;	
			TRACE1("unknown error:%d\n", result);
		}
	}
	
	return m_pucRet_inflate;
	////TRACE0("CComplibFacade::inflate\n");
	//if ( !ucSrcData )
	//{
	//	TRACE1("[Src:0x%x]\n", ucSrcData);
	//	return NULL;
	//}

	//if ( ulSrcLen<=0 )
	//{
	//	*(ulResultLen) = 0;
	//	return m_ucInflateBuffer;
	//}

	//// [주의] ulDestLen은 compress2 수행 후 받는 결과값(압축후 데이터 크기)이나 적절하게 큰 값을 넣어주지 않으면 에러 발생함.
	//*(ulResultLen) = m_uiBufferSize;

	//int result;
	//result = compress2(m_ucInflateBuffer, ulResultLen, ucSrcData, ulSrcLen, Z_BEST_SPEED);

	//if(result == Z_OK)		// if success
	//{
	//	return m_ucInflateBuffer;
	//}
	//else if(result == Z_MEM_ERROR)	// if there was not enough memory
	//{
	//	TRACE1("not enough memory:%d\n", result);
	//	return NULL;
	//}
	//else if(result == Z_BUF_ERROR)	// if there was not enough room in the output buffer
	//{
	//	TRACE1("not enough room in the output buffer:%d\n", result);
	//	return NULL;
	//}
	//else if(result == Z_STREAM_ERROR)	// if the level parameter is invalid
	//{
	//	TRACE1("level parameter is invalid:%d\n", result);
	//	return NULL;
	//}
	//else	// if unknown error
	//{
	//	TRACE1("unknown error:%d\n", result);
	//	return NULL;
	//}
	//return NULL;
}

UCHAR* CComplibFacade::deflate(ULONG *ulResultLen, const UCHAR *ucSrcData, ULONG ulSrcLen)
{
	if ( ucSrcData == nullptr )
	{//DTEC_NullPointCheck		
		m_pucRet_deflate = nullptr;		
	}
	else
	{
		// [주의] ulDestLen은 compress2 수행 후 받는 결과값(압축후 데이터 크기)이나 적절하게 큰 값을 넣어주지 않으면 에러 발생함.
		*(ulResultLen) = m_uiBufferSize;

		int result;
		result = uncompress(m_pucDeflateBuffer, ulResultLen, ucSrcData, ulSrcLen);

		if(result == Z_OK)		// if success
		{
			m_pucRet_deflate = m_pucDeflateBuffer;
		}
		else if(result == Z_MEM_ERROR)	// if there was not enough memory
		{ //DTEC_Else
			m_pucRet_deflate = nullptr;	
			TRACE1("not enough memory:%d\n", result);			
		}
		else if(result == Z_BUF_ERROR)	// if there was not enough room in the output buffer
		{//DTEC_Else
			m_pucRet_deflate = nullptr;	
			TRACE1("not enough room in the output buffer:%d\n", result);			
		}
		else if(result == Z_DATA_ERROR)	// input data was corrupted or incomplete
		{//DTEC_Else
			m_pucRet_deflate = nullptr;	
			TRACE1("input data was corrupted or incomplete:%d\n", result);			
		}
		else	// if unknown error
		{//DTEC_Else
			m_pucRet_deflate = nullptr;	
			TRACE1("unknown error:%d\n", result);
		}
	}
	
	return m_pucRet_deflate;

	//if (!ucSrcData )
	//{
	//	TRACE1("Not valid source:%x", ucSrcData);
	//	return NULL;
	//}

	//if ( ulSrcLen<=0 )
	//{
	//	*(ulResultLen) = 0;
	//	return m_ucDeflateBuffer;
	//}

	//// [주의] ulDestLen은 compress2 수행 후 받는 결과값(압축후 데이터 크기)이나 적절하게 큰 값을 넣어주지 않으면 에러 발생함.
	//*(ulResultLen) = m_uiBufferSize;

	//int result;
	//result = uncompress(m_ucDeflateBuffer, ulResultLen, ucSrcData, ulSrcLen);

	//if(result == Z_OK)		// if success
	//{
	//	return m_ucDeflateBuffer;
	//}
	//else if(result == Z_MEM_ERROR)	// if there was not enough memory
	//{
	//	TRACE1("not enough memory:%d\n", result);
	//	return NULL;
	//}
	//else if(result == Z_BUF_ERROR)	// if there was not enough room in the output buffer
	//{
	//	TRACE1("not enough room in the output buffer:%d\n", result);
	//	return NULL;
	//}
	//else if(result == Z_DATA_ERROR)	// input data was corrupted or incomplete
	//{
	//	TRACE1("input data was corrupted or incomplete:%d\n", result);
	//	return NULL;
	//}
	//else	// if unknown error
	//{
	//	TRACE1("unknown error:%d\n", result);
	//	return NULL;
	//}
	//return NULL;
}
