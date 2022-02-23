#pragma once
#ifndef _DFDATA_DEFINE_H_
#define _DFDATA_DEFINE_H_

#pragma pack(push,1)




//#include "define.h"

#define COMINT_MAX_CHANNEL_COUNT	    4 //5			// COMIT 방탐 최대 채널 수

//LMS_ADD_20220206
#define BAND1_START_FREQ				6000	// 저대역 시작 주파수 설정 //LMS_ADD_20220206
#define BAND1_STOP_FREQ					18000	// 저대역 종료 주파수 설정 //LMS_ADD_20220206
#define BAND2_STOP_FREQ					18000	// 고대역 종료 주파수 설정 //LMS_ADD_20220206
#define APPLY_AZIMUTH_OFFSET_WHEN_CREATING_IDEAL_RADICAL //방사보정데이터 생성 시 방위각 오프셋을 반영할때 설정한다.
#define FREQ_RANGE_START_MHz            BAND1_START_FREQ  //LMS_ADD_20171222
#define FREQ_RANGE_END_MHz              18000              //LMS_ADD_20220206   
#define DF_CH_NUM                       4 //5 //9                 //LMS_ADD_20171222    
#define DF_AZIMUTH_NUM                  360               //LMS_ADD_20171222    
struct _stBigArray
{
	USHORT	m_usPhDfRomData[FREQ_RANGE_END_MHz-FREQ_RANGE_START_MHz+1][DF_AZIMUTH_NUM][DF_CH_NUM];     //m_usPhDfRomData[9][360][2501]; // 방사보정용 위상 ROM 데이터[채널][방위][주파수] : 방위 0~360, 1도 Step, 주파수 500MHz~3000MHz, 1MHz Step
	INT	    m_iRadicalDataFreqMHz[FREQ_RANGE_END_MHz-FREQ_RANGE_START_MHz+1];      //LMS_ADD_20220206
};
#define	PI							(float)(3.14159265)
//LMS_ADD_20190705 방향탐지 알고리즘(CVDF)함수
#define COMINT_PHASE_RESOLUTION	 	0.01f		// COMNT 위상(위상차) 해상도(16Bit, 0.01도)
#define DIRECT_COS_CALCULATION_APPLY_MODE					// CVDF 연산시 Direct Cosin 연산수행 여부 지정

#define APPLY_AZIMUTH_RANGE_FOR_LINEAR_CVDF //LMS_ADD_20220206/ CVDF 연산시 Direct Cosin 연산수행 여부 지정



#pragma pack(pop)

#endif
