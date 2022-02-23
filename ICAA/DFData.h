#pragma once
#ifndef _DFDATA_DEFINE_H_
#define _DFDATA_DEFINE_H_

#pragma pack(push,1)




//#include "define.h"

#define COMINT_MAX_CHANNEL_COUNT	    4 //5			// COMIT ��Ž �ִ� ä�� ��

//LMS_ADD_20220206
#define BAND1_START_FREQ				6000	// ���뿪 ���� ���ļ� ���� //LMS_ADD_20220206
#define BAND1_STOP_FREQ					18000	// ���뿪 ���� ���ļ� ���� //LMS_ADD_20220206
#define BAND2_STOP_FREQ					18000	// ��뿪 ���� ���ļ� ���� //LMS_ADD_20220206
#define APPLY_AZIMUTH_OFFSET_WHEN_CREATING_IDEAL_RADICAL //��纸�������� ���� �� ������ �������� �ݿ��Ҷ� �����Ѵ�.
#define FREQ_RANGE_START_MHz            BAND1_START_FREQ  //LMS_ADD_20171222
#define FREQ_RANGE_END_MHz              18000              //LMS_ADD_20220206   
#define DF_CH_NUM                       4 //5 //9                 //LMS_ADD_20171222    
#define DF_AZIMUTH_NUM                  360               //LMS_ADD_20171222    
struct _stBigArray
{
	USHORT	m_usPhDfRomData[FREQ_RANGE_END_MHz-FREQ_RANGE_START_MHz+1][DF_AZIMUTH_NUM][DF_CH_NUM];     //m_usPhDfRomData[9][360][2501]; // ��纸���� ���� ROM ������[ä��][����][���ļ�] : ���� 0~360, 1�� Step, ���ļ� 500MHz~3000MHz, 1MHz Step
	INT	    m_iRadicalDataFreqMHz[FREQ_RANGE_END_MHz-FREQ_RANGE_START_MHz+1];      //LMS_ADD_20220206
};
#define	PI							(float)(3.14159265)
//LMS_ADD_20190705 ����Ž�� �˰���(CVDF)�Լ�
#define COMINT_PHASE_RESOLUTION	 	0.01f		// COMNT ����(������) �ػ�(16Bit, 0.01��)
#define DIRECT_COS_CALCULATION_APPLY_MODE					// CVDF ����� Direct Cosin ������� ���� ����

#define APPLY_AZIMUTH_RANGE_FOR_LINEAR_CVDF //LMS_ADD_20220206/ CVDF ����� Direct Cosin ������� ���� ����



#pragma pack(pop)

#endif
