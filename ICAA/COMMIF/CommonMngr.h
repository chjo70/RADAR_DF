
/**
* [식별자 : CLS-GMU-GR-L-COMM]
*
* [추적성 : SRS-G-SOFR-009]
*
* @class CCommonMngr
* @brief 공통 Manager 부모 클래스
*
* (1) 클래스 설명
* -  C/E/F 매니저 클래스가 상속받을 수 있는 부모 클래스. 상속 시 Observer 패턴 사용 가능.
*
* (2) 설계결정사항
* - C/E/F Manager 클래스는 본 클래스를 상속받아서 구현한다.
*
* (3) 제한 및 예외처리
* - 해당사항없음
*/
#pragma once

#include "CGRCommIF.h"

#include <map>
#include <list>
#include <queue>

using namespace std;

typedef unsigned long uint32_tt;


class CCommonMngr
{

public:
	
	/**
	* @brief       생성자함수
	* @param       void   
	* @return         
	*/
	CCommonMngr(void);
	
	/**
	* @brief       소멸자함수
	* @param       void   
	* @return         
	*/
	virtual ~CCommonMngr(void);

	/**
	* @brief 매니저 이름을 등록하기 위한 스트링
	* - 특정 매니저를 찾고자 할 때, 본인이 등록한 매니저 이름으로 비교하여 해당 매니저를 찾아낼 수 있음. 2014.12.30. 
	*/
	string m_strIAM; 

protected:
	/**
	* @brief STL list 컨테이너 객체
	* - Observer로 등록되는 Controller 클래스 포인터를 저장하기 위한 list 컨테이너 객체
	*/
	//std::list<CCommonControllerIF*> m_listData;

	/*!
	 * @brief STL list 컨테이너 객체
	 * - 특정 매니저의 정보 변화에 따라서 다른 매니저가 그 정보를 observer 패턴을 사용해 받아야할 필요성 대두. 2013.10.10. 이정남
	 */
	std::list<CCommonMngr*> m_listMngr;

	/**
	* @brief CGRCommIF 객체
	* - 통신을 수행하기 위한 통신 객체
	*/
	CGRCommIF& m_hCommIF;
	/*CGRDMapIF& m_hDMapIF;
	CGRInterCommIF& m_hInterCommIF;*/

	bool m_bIsServerMode; // 2013.12.10. 이정남 추가
	bool m_bIsReplayMode; // 2015.02.06. 이정남 추가
	
	/*LIGLock m_commonLock;
	LIGLock m_commonMngrLock;*/

protected:
	void InitTimeStruct(tm& i_Time);

public:
	
	/**
	* @brief       COMM Interface로부터 메시지를 수신하는 함수 (재정의)
	* @param    i_uiOpcode		op 코드 
	* @param    i_usSize		메시지 크기 
	* @param    i_usOperatorID		운용자 ID 번호
	* @param    i_pvData		수신된 데이터의 시작 주소값
	* @param    i_bServer		지상임무 연동기 또는 POSN 여부
	* @param   i_bReplay		임무재연 모드 여부
	* @return     void
	*/
	virtual void Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOper, unsigned char ucOpertorID, void *i_pvData) = 0;
	
	// 종료(프로그램 종료/로그오프/시스템 종료) 때 처리 함수, 2014.06.19. - 진기병
	// 자식들이 재정의해서 써야 함
	
	/**
	* @brief       MakeFSigID 생성함수
	* @return      UINT   MakeFSigID
	*/
	UINT MakeFSigID();
	unsigned int m_unLastSigID; ///< 마지막 신호 ID 변수

	
	// float와 double형 변수를 network으로 보내기 위한 endian 처리용 함수 - 2013.12.24. 진기병
	
	/**
	* @brief       ntohf 변환함수
	* @param       float fSrc   플롯형 변수
	* @return      float   플롯형 변수
	*/
	float ntohf( float fSrc );
	
	/**
	* @brief       htonf 변환함수
	* @param       float fSrc   플롯형 변수
	* @return      float   플롯형 변수
	*/
	float htonf( float fSrc );
	
	/**
	* @brief       ntohd 변환함수
	* @param       double dblSrc   더블형 변수
	* @return      float   플롯형 변수
	*/
	float ntohd( double dblSrc );
	
	/**
	* @brief       htond 변환함수
	* @param       double dblSrc   더블형 변수
	* @return      float   플롯형 변수
	*/
	float htond( double dblSrc );

	
	/**
	* @brief       현재 시간 요청 함수
	* @return      string  시간 문자열 객체  
	*/
	string GetCurrentTimeInfo();	// CGRTaskMngr에서 CCommonMngr로 이동 - 2014.04.10. 진기병
	
	/**
	* @brief       현재 DF 시간 요청 함수
	* @param       unsigned int unSec  초  
	* @param       unsigned int unNanoSec  나노초  
	* @return      string   시간 문자열 객체
	*/
	string GetCurrDFTimeInfo( unsigned int unSec, unsigned int unNanoSec );	// F에서 C로 정밀방탐 요청 후 수신했을 때 시각정보 구하는 함수, 2016.07.12 - 진기병
	
	/**
	* @brief       운용자ID 획득함수
	* @return      unsigned short  운용자ID 
	*/
	unsigned short GetLogInOperatorId();	// CGRTaskMngr에서 CCommonMngr로 이동 - 2014.04.10. 진기병
	//2대운용_이정남(시작)=================================================
	
	/**
	* @brief       운용자ID 획득 함수
	* @param       int i_nLinkNum   링크번호
	* @return      unsigned short   운용자ID
	*/
	unsigned short GetLogInOperatorId(int i_nLinkNum);
	//2대운용_이정남(끝)=================================================

	
	/**
	* @brief       링크번호 획득함수
	* @param       unsigned short & i_usOperId   운용자ID
	* @return      int   링크번호
	*/
	int GetLinkNum(unsigned short& i_usOperId); // 2015.1.5. 이정남 추가. 운용자 id 를 넣으면 데이터링크 번호(1 혹은 2)를 리턴해주는 함수

	unsigned int MakeOPCode(unsigned char i_uchCMDCode, unsigned char i_uchSrc=DEVICECODE_BRA, 
		unsigned char i_uchDest=DEVICECODE_TRD, unsigned char i_uchEncrypt=0, unsigned char i_uchClass=0);	
	
protected:
	//start_static_0918
	template <typename T> void DeletePointers(std::list<T*> &i_listContainer)	//#FA_Q_2502_T5
	{
		for (auto iter = i_listContainer.begin(); iter != i_listContainer.end(); iter++) //#FA_C_PotentialUnboundedLoop_T3
		{
			delete (*iter);
			// *iter = nullptr;
		}
		i_listContainer.clear();
	}
	//end_static_0918

	//////////////////////////////////////////////////////////////////////////
	// 벡터 메모리 삭제
	//start_static_0918
	template <typename T> void DeletePointers(std::vector<T*> &i_vecContainer)	//#FA_Q_2502_T5
	{
		for (auto iter = i_vecContainer.begin(); iter != i_vecContainer.end(); iter++ ) //#FA_C_PotentialUnboundedLoop_T3
		{
			delete (*iter);
			// *iter = nullptr;
		}
		i_vecContainer.clear();
	}
	//end_static_0918

	template <typename T> void DeletePointers2(std::vector<T*> &i_vecContainer)
	{
		for( std::vector<T*>::iterator it=i_vecContainer.begin() ; it != i_vecContainer.end() ; ) { //#FA_C_PotentialUnboundedLoop_T3
			it = i_vecContainer.erase( it );
		}
		i_vecContainer.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	//start_static_0918
	template <typename T1, typename T2> void DeletePointers(std::map<T1, T2*> &i_mapContainer)	//#FA_Q_2502_T5
	{
		for (auto iter = i_mapContainer.begin(); iter != i_mapContainer.end(); ) //#FA_C_PotentialUnboundedLoop_T3
		{
			i_mapContainer.erase( iter++ );
		}
		i_mapContainer.clear();
	}
	//end_static_0918

	template <typename T> void DeleteRangePointers( typename list<T> *pListContainer, typename list<T>::iterator itStart, typename list<T>::iterator itEnd )
	{
		for (auto iter = itStart; iter != itEnd; iter++) //#FA_C_PotentialUnboundedLoop_T3
		{
			delete (*iter);
		}
		pListContainer->erase( itStart, itEnd );
	}

public:
	
	/**
	* @brief       alignStrCnt 처리함수
	* @param       CString & i_str   스트링 객체
	* @param       int i_nCnt   카운트
	* @return      void   
	*/
	void alignStrCnt(CString& i_str, int i_nCnt);
	
	/**
	* @brief       CStrToChar 함수
	* @param       CString & i_str    스트링 객체 
	* @param       char * i_puch    포인터 객체
	* @param       int i_nCnt     카운트
	* @return      void   
	*/
	void CStrToChar(CString &i_str, char *i_puch, int i_nCnt=0);	
	
	/**
	* @brief       CStrToUChar 함수
	* @param       CString & i_str  스트링 객체 
	* @param       unsigned char * i_puch   포인터 객체
	* @param       int i_nCnt   카운트
	* @return      void   
	*/
	void CStrToUChar(CString &i_str, unsigned char *i_puch, int i_nCnt=0);
};

// 
/*! \todo   클래스 안에 있는 템플럿 정의를 밖으로 빼서 선언해도 다른데 영향이 없을 것 같은데, 현재는 그대로 둠.
    \author 조철희 (churlhee.jo@lignex1.com)
    \date 	2016-08-08 16:01:35
*/
template <typename T> void DeleteRangePointers( typename list<T> *pListContainer, typename list<T>::iterator itStart, typename list<T>::iterator itEnd )
{
	for (auto iter = itStart; iter != itEnd; iter++)
	{
		delete (*iter);
	}
	pListContainer->erase( itStart, itEnd );
}


template <typename T> void DeletePointers(std::list<T*> &i_listContainer)
{
	for (auto iter = i_listContainer.begin(); iter != i_listContainer.end(); iter++)
	{
		delete (*iter);
		// *iter = nullptr;
	}
	i_listContainer.clear();
}
template <typename T> void DeletePointers(std::vector<T*> &i_vecContainer)
{
	for (auto iter = i_vecContainer.begin(); iter != i_vecContainer.end(); iter++ )
	{
		delete (*iter);
		// *iter = nullptr;
	}
	i_vecContainer.clear();
}
template <typename T1, typename T2> void DeletePointers(std::map<T1, T2*> &i_mapContainer)
{
	for (auto iter = i_mapContainer.begin(); iter != i_mapContainer.end(); )
	{
		i_mapContainer.erase( iter++ );
	}
	i_mapContainer.clear();
}
