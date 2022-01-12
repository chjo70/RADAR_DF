#include "stdafx.h"
#include "ADSBReceivedProcessMngr.h"

// ADSB 서버 To 수신국_ADSB(AS=ADSB데몬)
#define OPCODE_ADSBS_AS_SEND_RAWADSBDATA	0xFF000100

// 수신국_ADSB(AS=ADSB데몬) To 감시국(OV) - Send
#define OPCODE_AS_OV_RES_SELECTADSB			0x81010100
#define OPCODE_AS_OV_RES_SETINTERVALADSB	0x81010200
#define OPCODE_AS_OV_SEND_ADSBDATA			0x81011100

// 감시국(OV) To 수신국_ADSB(AS=ADSB데몬) - Receive
#define OPCODE_OV_AS_REQ_SELECTADSB			0x01810100
#define OPCODE_OV_AS_REQ_SETINTERVALADSB	0x01810200
#define OPCODE_OV_AS_RES_RECVADSBDATA		0x01811100


CADSBReceivedProcessMngr* CADSBReceivedProcessMngr::uniqueInstance=nullptr;
void CALLBACK GRTimerForCheckAndSendADSBProc(PVOID lpParam, BOOLEAN TimerOrWaitFired);


void CALLBACK GRTimerForCheckAndSendADSBProc(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CADSBReceivedProcessMngr* pCThis = ((CADSBReceivedProcessMngr*)lpParam);

	TRACE("[Map Size: %d]\n",pCThis->m_mapADSBDecodedData.size());

	if(pCThis->m_mapADSBDecodedData.size() > 0)
	{
		map<string, STADSBDecodedData>::iterator iterMapADSBData = pCThis->m_mapADSBDecodedData.begin();

		for(;iterMapADSBData != pCThis->m_mapADSBDecodedData.end();)
		{
			(*iterMapADSBData).second.uiTout++;

			TRACE("[Map ICAO: %s, Tout: %d, AircraftID: %s, Latitude: %f, Longitude: %f, Altitude: %d, Heading: %f, TrackTime: %s]\n", (*iterMapADSBData).first.c_str(), (*iterMapADSBData).second.uiTout, (*iterMapADSBData).second.acAircarftID, (*iterMapADSBData).second.fLat, (*iterMapADSBData).second.fLon, (*iterMapADSBData).second.uiAltitude, (*iterMapADSBData).second.fHeading, (*iterMapADSBData).second.acTrackTime);

			if(pCThis->m_ucTout <= (*iterMapADSBData).second.uiTout)
			{
				pCThis->m_mapADSBDecodedData.erase(iterMapADSBData++);
			}
			else
			{
				iterMapADSBData++;
			}
		}
	}

	if(pCThis->m_bIsAvalibleForSend && (pCThis->m_ucCheckTick == pCThis->m_ucSendingTIme))
	{
		pCThis->SendADSBData();
		pCThis->m_ucCheckTick = 1;
	}
	else
	{
		pCThis->m_ucCheckTick++;
	}
}


CADSBReceivedProcessMngr::CADSBReceivedProcessMngr()
:m_hCommIF_ADSBRPMngr(m_hCommIF)
{
	char readBuf[100] = {0};
	char *envini_path = ("..\\ICAA\\config.ini");
	CString	cstrServerClient = CString("");
	GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("MODE"), NULL, readBuf, _countof(readBuf), envini_path);
	cstrServerClient.Format(("%s"), readBuf);	// SERVER or CLIENT
	CString	strPort = CString("");

	if(strcmp(cstrServerClient, "ADSBD") == 0)
	{
		InitializeCriticalSection(&m_csReceiveAndAssign);

		m_hCommIF.RegisterOpcode(OPCODE_ADSBS_AS_SEND_RAWADSBDATA, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_AS_REQ_SELECTADSB, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_AS_REQ_SETINTERVALADSB, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_AS_RES_RECVADSBDATA, this);

		m_hTimer = nullptr;
		m_hTimerQueue = nullptr;

		m_ucTout = 20;

		m_bIsAvalibleForSend = true;
		m_ucSendADSBDataInterval = 1;
		m_ucCheckTick = 1;
		m_ucSendingTIme = 1;

		m_ucMyLastIP = GetLastIP();
	
		if(m_hTimerQueue == NULL)
		{
			m_hTimerQueue = CreateTimerQueue();
			CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, (WAITORTIMERCALLBACK)GRTimerForCheckAndSendADSBProc, this, 0, 1000, 0);
		}
	}
}

CADSBReceivedProcessMngr::~CADSBReceivedProcessMngr()
{
	char readBuf[100] = {0};
	char *envini_path = ("..\\ICAA\\config.ini");
	CString	cstrServerClient = CString("");
	GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("MODE"), NULL, readBuf, _countof(readBuf), envini_path);
	cstrServerClient.Format(("%s"), readBuf);	// SERVER or CLIENT
	CString	strPort = CString("");

	if(strcmp(cstrServerClient, "ADSBD") == 0)
	{
		m_hCommIF.UnregisterOpcode(OPCODE_ADSBS_AS_SEND_RAWADSBDATA, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_AS_REQ_SELECTADSB, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_AS_REQ_SETINTERVALADSB, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_AS_RES_RECVADSBDATA, this);

		if(m_hTimerQueue != NULL)
		{
			BOOL bRtn = DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, NULL);

			if(bRtn == TRUE)
			{
				m_hTimerQueue = NULL;
				m_hTimer = NULL;
			}
			else
			{
				if(ERROR_IO_PENDING == GetLastError() || m_hTimer == NULL)
				{
					m_hTimerQueue = NULL;
					m_hTimer = NULL;
				}
				else
				{
					while(DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, NULL) == 0)
					{
						if(ERROR_IO_PENDING == GetLastError())
						{
							m_hTimerQueue = NULL;
							m_hTimer = NULL;

							break;
						}
					}
				}		
			}
		}

		DeleteCriticalSection(&m_csReceiveAndAssign);
	}
}

CADSBReceivedProcessMngr* CADSBReceivedProcessMngr::GetInstance()
{
	if (uniqueInstance == nullptr)
	{
		uniqueInstance = new CADSBReceivedProcessMngr();		
	}

	return uniqueInstance;
}

void CADSBReceivedProcessMngr::Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData)
{
	//TRACE("[Log] Message is received.\n");

	// 수신단에서 Msg 데이터 형성
	STMsg stInstanceMsg;

	stInstanceMsg.uiOpcode = i_uiOpcode;
	stInstanceMsg.usMSize = i_usSize;
	stInstanceMsg.ucLinkID = i_ucLink;
	stInstanceMsg.ucRevOperID = i_ucRevOperID;

	memcpy(stInstanceMsg.buf, i_pvData, i_usSize);

	// 쓰레드 처리를 위해 Msg Queue에 집어넣기
	PushMsg(stInstanceMsg); 

	m_cond.LIGSetEvent();
}

void CADSBReceivedProcessMngr::ProcessMsg(STMsg& i_stMsg)
{
	unsigned int uiNormalOpcode = i_stMsg.uiOpcode & 0xFFFFFF00;

	switch(uiNormalOpcode)
	{
	case OPCODE_ADSBS_AS_SEND_RAWADSBDATA:	// ADSBS-AS
		{
			// Build STADSBRawPacket From i_stMsg
			STADSBRawPacket stPushbackADSBRawPacket = STADSBRawPacket();

			stPushbackADSBRawPacket.buf_size = i_stMsg.usMSize;
			memcpy((void*)stPushbackADSBRawPacket.buf, (void*)i_stMsg.buf, stPushbackADSBRawPacket.buf_size);

			PushADSBRawPacket(stPushbackADSBRawPacket);

			ProcessADSBParse();
		}
		break;

	case OPCODE_OV_AS_REQ_SELECTADSB:	// ADSBS-AS
		{
			// 마지막 IP를 체크하여 1: 14, 2: 24, 3: 34를 확인 후
			// m_bIsAvalibleForSend 값을 True 또는 False로 바꿈
			unsigned char ucLastIP = GetLastIP();
			unsigned char ucRecvSelectADSB = 0;

			memcpy(&ucRecvSelectADSB, i_stMsg.buf, sizeof(unsigned char));

			if(ucLastIP/10 == ucRecvSelectADSB)
			{
				m_bIsAvalibleForSend = true;

				bool bRtn = m_hCommIF.Send(OPCODE_AS_OV_RES_SELECTADSB, sizeof(unsigned char), 0, 0, m_ucMyLastIP, (void*)&ucRecvSelectADSB);
			}
			else
			{
				m_bIsAvalibleForSend = false;
			}
		}
		break;

	case OPCODE_OV_AS_REQ_SETINTERVALADSB:	// ADSBS-AS
		{
			unsigned char ucRecvIntervalSend = 0;

			memcpy(&ucRecvIntervalSend, i_stMsg.buf, sizeof(unsigned char));

			m_ucSendingTIme = ucRecvIntervalSend;

			bool bRtn = m_hCommIF.Send(OPCODE_AS_OV_RES_SETINTERVALADSB, sizeof(unsigned char), 0, 0, m_ucMyLastIP, (void*)&ucRecvIntervalSend);
		}
		break;

	case OPCODE_OV_AS_RES_RECVADSBDATA:	// ADSBS-AS
		{
			TRACE("[수신 확인] 운용부 ADS-B 데이터 수신 확인\n");
		}
		break;

	default:
		break;
	}
}

// Hex To Binary 변환 함수
bool CADSBReceivedProcessMngr::ConvertCharToBinary(IN char* i_pcInput, IN int i_iSize, OUT char* i_pcOuput)
{
	bool bRtn = true;

	for(int iBitCnt = 0; iBitCnt < i_iSize; iBitCnt++)
	{
		char inputChar = *(i_pcInput+iBitCnt);

		for(int i = (8*(iBitCnt+1))-1; i>=(8*iBitCnt); i--)
		{
			i_pcOuput[i] = (char)(inputChar & 1);
			inputChar = inputChar >> 1;
		}
	}

	return bRtn;
}

bool CADSBReceivedProcessMngr::ConvertBinaryToInt(IN char* i_pcInput, IN int i_iCntOfBits, OUT int* i_piOutput)
{
	bool bRtn = true;

	int iDecimalNumber = 0;
	
	for(int i = i_iCntOfBits-1; i >= 0; i--)
	{
		iDecimalNumber += (int)(i_pcInput[i_iCntOfBits-i-1]*pow(2.0,(double)i));
	}

	(*i_piOutput) = iDecimalNumber;

	return bRtn;
}

// NL(lat) Function
double CADSBReceivedProcessMngr::NL_lat(double i_dbLat)
{
	double dbRst = 0;
	double lastRst = 0;
	double dbArccosRst = 0;
	double dbCombinedRst = 0;
	double upperRst = 0;
	double lowerRst = 0;
	double dbNZ = 15.0;

	upperRst = 1 - cos(PI/(2*dbNZ));
	lowerRst = cos(PI*i_dbLat/180.0) * cos(PI*i_dbLat/180.0);

	dbCombinedRst = upperRst / lowerRst;

	dbArccosRst = acos(1 - dbCombinedRst);

	lastRst = (2*PI)/dbArccosRst;

	dbRst = floor(lastRst);

	return dbRst;
}

bool CADSBReceivedProcessMngr::PushADSBRawPacket(STADSBRawPacket i_stADSBRawPacket)
{
	bool bRtn = true;

	// Start CriticalSection
	EnterCriticalSection(&m_csReceiveAndAssign);

	// Push back the ADSB-Raw Packet
	m_listADSBRawPacket.push_back(i_stADSBRawPacket);

	// End CriticalSection
	LeaveCriticalSection(&m_csReceiveAndAssign);

	return bRtn;
}

bool CADSBReceivedProcessMngr::ProcessADSBParse()
{
	bool bRtn = true;

	// Copy ADSB Raw Packet to Local Variable
	std::list<STADSBRawPacket> m_listLocalADSBRawPacket = list<STADSBRawPacket>();

	// Start CriticalSection
	EnterCriticalSection(&m_csReceiveAndAssign);

	m_listLocalADSBRawPacket.assign(m_listADSBRawPacket.begin(), m_listADSBRawPacket.end());

	m_listADSBRawPacket.clear();

	// End CriticalSection
	LeaveCriticalSection(&m_csReceiveAndAssign);

	// Main Processing
	// Iterating Process
	while(!m_listLocalADSBRawPacket.empty())
	{
		bool bRtnAI = false;
		bool bRtnAP = false;
		bool bRtnAV = false;

		STADSBRawPacket stLocalADSBRawPacket = STADSBRawPacket();
		STADSBDecodedData stLocalADSBDecodeDAta = STADSBDecodedData();

		stLocalADSBRawPacket = (STADSBRawPacket)m_listLocalADSBRawPacket.front();

		// 1_DF(Download Format) 파싱
		bool bRtnDF = ParseDF(&stLocalADSBRawPacket);

		if(bRtnDF == true)
		{
			// 2_ICAO24 파싱
			bool bRtnICAO = ParseICAO(&stLocalADSBRawPacket, &stLocalADSBDecodeDAta);

			if(bRtnICAO == true)
			{
				// 3_TC(Type Code) 파싱 case값 별 의미
				// 1(1-4): Aircraft Identification
				// 2(5-8): Surface position
				// 3(9-18): Airborne position(w/ Baro Altitude)
				// 4(19): Airborne velocities
				// 5(20-22): Airborne position(w/ GNSS Height)
				// 6(23-27): Reserved
				// 7(28): AIrcraft status
				// 8(29): Target state and status information
				// 9(31): Aircraft operation status
				unsigned char ucTC = ParseTC(&stLocalADSBRawPacket);

				switch(ucTC)
				{
					case 1:
						{
							bRtnAI = ParseAI(&stLocalADSBRawPacket, &stLocalADSBDecodeDAta);
						}
						break;

					case 3:
						{
							bRtnAP = ParseAP(&stLocalADSBRawPacket, &stLocalADSBDecodeDAta);
						}
						break;

					case 4:
						{
							bRtnAV = ParseAV(&stLocalADSBRawPacket, &stLocalADSBDecodeDAta);
						}
						break;

					case 2:
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
					default:
						{
							//TRACE("[경보]구현되지 않은 TC[%d]가 수신되었습니다.\n", ucTC);
						}
						break;
				}

				// Map check if exist, update if not exist, add
				if(bRtnAI == true)			// Add if not exist
				{
					bool bIsAIDone = false;

					PutTrackTimeInfo(&stLocalADSBDecodeDAta);

					if(m_mapADSBDecodedData.size() != 0)
					{
						for(auto iterADSBDecodedData = m_mapADSBDecodedData.begin(); iterADSBDecodedData != m_mapADSBDecodedData.end(); iterADSBDecodedData++)
						{
							string strFirstICAO = iterADSBDecodedData->first;

							if(strcmp(strFirstICAO.c_str(), stLocalADSBDecodeDAta.strICAOAddr.c_str()) == 0)
							{
								bIsAIDone = true;

								STADSBDecodedData* stAIDecodedData = &iterADSBDecodedData->second;

								strcpy_s(stAIDecodedData->acAircarftID, stLocalADSBDecodeDAta.acAircarftID);
								strcpy_s(stAIDecodedData->acTrackTime, stLocalADSBDecodeDAta.acTrackTime);
								stAIDecodedData->uiTout = 0;

								break;
							}
						}
					}
					
					// map에 정보가 하나도 없거나 정보와 동일한 ICAO24 정보가 없을 경우 신규 등록
					if(bIsAIDone == false)
					{
						m_mapADSBDecodedData.insert(make_pair(stLocalADSBDecodeDAta.strICAOAddr, stLocalADSBDecodeDAta));
					}
				}
				else if(bRtnAP == true)		// Update if not exist
				{
					bool bIsAPDone = false;

					PutTrackTimeInfo(&stLocalADSBDecodeDAta);

					if(m_mapADSBDecodedData.size() != 0)
					{
						for(auto iterADSBDecodedData = m_mapADSBDecodedData.begin(); iterADSBDecodedData != m_mapADSBDecodedData.end(); iterADSBDecodedData++)
						{
							string strFirstICAO = iterADSBDecodedData->first;

							if(strcmp(strFirstICAO.c_str(), stLocalADSBDecodeDAta.strICAOAddr.c_str()) == 0)
							{
								bIsAPDone = true;

								STADSBDecodedData* stAIDecodedData = &iterADSBDecodedData->second;

								bool bIsEvenRecently = false;

								if(stLocalADSBDecodeDAta.isEven == true)
								{
									stAIDecodedData->isEven = true;
									stAIDecodedData->fLatEven = stLocalADSBDecodeDAta.fLatEven;
									stAIDecodedData->fLonEven = stLocalADSBDecodeDAta.fLonEven;
									bIsEvenRecently = true;
								}
								else if(stLocalADSBDecodeDAta.isOdd == true)
								{
									stAIDecodedData->isOdd = true;
									stAIDecodedData->fLatOdd = stLocalADSBDecodeDAta.fLatOdd;
									stAIDecodedData->fLonOdd = stLocalADSBDecodeDAta.fLonOdd;
								}

								stAIDecodedData->uiAltitude = stLocalADSBDecodeDAta.uiAltitude;

								// 기존 stAIDecodedData에 Even과 Odd 값이 전부 존재하면 위경도 계산 수행
								if(stAIDecodedData->isEven == true && stAIDecodedData->isOdd == true)
								{
									CalcLat_Lon(bIsEvenRecently, stAIDecodedData);
								}

								strcpy_s(stAIDecodedData->acTrackTime, stLocalADSBDecodeDAta.acTrackTime);
								stAIDecodedData->uiTout = 0;

								break;
							}
						}
					}

					// map에 정보가 하나도 없거나 정보와 동일한 ICAO24 정보가 없을 경우 신규 등록
					if(bIsAPDone == false)
					{
						m_mapADSBDecodedData.insert(make_pair(stLocalADSBDecodeDAta.strICAOAddr, stLocalADSBDecodeDAta));
					}
				}
				else if(bRtnAV == true)
				{
					bool bIsAVDone = false;

					PutTrackTimeInfo(&stLocalADSBDecodeDAta);

					if(m_mapADSBDecodedData.size() != 0)
					{
						for(auto iterADSBDecodedData = m_mapADSBDecodedData.begin(); iterADSBDecodedData != m_mapADSBDecodedData.end(); iterADSBDecodedData++)
						{
							string strFirstICAO = iterADSBDecodedData->first;

							if(strcmp(strFirstICAO.c_str(), stLocalADSBDecodeDAta.strICAOAddr.c_str()) == 0)
							{
								bIsAVDone = true;

								STADSBDecodedData* stAIDecodedData = &iterADSBDecodedData->second;

								stAIDecodedData->bIsGroundSpeed = stLocalADSBDecodeDAta.bIsGroundSpeed;
								stAIDecodedData->fVel = stLocalADSBDecodeDAta.fVel;
								stAIDecodedData->fHeading = stLocalADSBDecodeDAta.fHeading;

								strcpy_s(stAIDecodedData->acTrackTime, stLocalADSBDecodeDAta.acTrackTime);
								stAIDecodedData->uiTout = 0;

								break;
							}
						}
					}

					// map에 정보가 하나도 없거나 정보와 동일한 ICAO24 정보가 없을 경우 신규 등록
					if(bIsAVDone == false)
					{
						m_mapADSBDecodedData.insert(make_pair(stLocalADSBDecodeDAta.strICAOAddr, stLocalADSBDecodeDAta));
					}
				}
				else
				{

				}
			}
			else
			{
				// 해당 부분은 호출되지 않을 것으로 예상
				//TRACE("올바르지 않은 ICAO24 값을 가지고 있습니다.\n");

				bRtn = false;
			}
		}
		else
		{
			//TRACE("ADS-B와 관련없는 DF값을 가지고 있습니다.\n");

			bRtn = false;
		}
		
		// Pop front of list which is useless
		m_listLocalADSBRawPacket.pop_front();
	}

	return bRtn;
}

bool CADSBReceivedProcessMngr::ParseDF(STADSBRawPacket* i_stADSBRawPacket)
{
	bool bRtn = true;

	// 1.Decoded Data Parsing
	char acOutputBinary[112] = {0,};

	if(i_stADSBRawPacket->buf_size == 23)
	{
		ConvertCharToBinary(i_stADSBRawPacket->buf+9, i_stADSBRawPacket->buf_size-9, acOutputBinary);

		int iRstOfInt = 0;

		ConvertBinaryToInt(acOutputBinary, 5, &iRstOfInt);

		if(iRstOfInt == 17 || iRstOfInt == 18)
		{
			// Hex코드로 된 Raw 데이터 출력
			stringstream ssTempHexTrace = stringstream();

			for(int i=9; i<i_stADSBRawPacket->buf_size; i++)
			{
				ssTempHexTrace << std::setfill('0') << setw(2) << hex << (int)(unsigned char)i_stADSBRawPacket->buf[i];
			}

			//TRACE("*%s ",ssTempHexTrace.str().c_str());

			ssTempHexTrace.str("");
		}
		else
		{
			//TRACE("ADS-B DF(Download Format)이 17 또는 18이 아닙니다.\n");
			bRtn = false;
		}
	}
	else
	{
		//TRACE("ADS-B Raw Data 크기가 23 Bytes가 아닙니다.\n");
		bRtn = false;
	}

	return bRtn;
}

bool CADSBReceivedProcessMngr::ParseICAO(STADSBRawPacket* i_stADSBRawPacket, STADSBDecodedData* i_stADSBDecodedData)
{
	bool bRtn = true;
	stringstream ssTempICAOTrace = stringstream();

	ssTempICAOTrace << std::setfill('0') << setw(2) << hex << (int)(unsigned char)i_stADSBRawPacket->buf[10];
	ssTempICAOTrace << std::setfill('0') << setw(2) << hex << (int)(unsigned char)i_stADSBRawPacket->buf[11];
	ssTempICAOTrace << std::setfill('0') << setw(2) << hex << (int)(unsigned char)i_stADSBRawPacket->buf[12];

	//TRACE("ICAO[24bits]: %s\n",ssTempICAOTrace.str().c_str());

	i_stADSBDecodedData->strICAOAddr = ssTempICAOTrace.str();

	return bRtn;
}

unsigned char CADSBReceivedProcessMngr::ParseTC(STADSBRawPacket* i_stADSBRawPacket)
{
	unsigned char ucRtn = 0;
	unsigned char ucRtnSimple = 0;
	int iRstOfInt = 0;
	char acOutputBinary[112] = {0,};

	ConvertCharToBinary(i_stADSBRawPacket->buf+9, i_stADSBRawPacket->buf_size-9, acOutputBinary);
	ConvertBinaryToInt(acOutputBinary+32, 5, &iRstOfInt);

	ucRtn = (unsigned char)(iRstOfInt);

	//TRACE("TC[5bits]: %d\n", ucRtn);

	if(1 <= ucRtn && 4 >= ucRtn)
	{
		ucRtnSimple = 1;
	}
	else if(5 <= ucRtn && 8 >= ucRtn)
	{
		ucRtnSimple = 2;
	}
	else if(9 <= ucRtn && 18 >= ucRtn)
	{
		ucRtnSimple = 3;
	}
	else if(19 == ucRtn)
	{
		ucRtnSimple = 4;
	}
	else if(20 <= ucRtn && 22 >= ucRtn)
	{
		ucRtnSimple = 5;
	}
	else if(23 <= ucRtn && 27 >= ucRtn)
	{
		ucRtnSimple = 6;
	}
	else if(28 == ucRtn)
	{
		ucRtnSimple = 7;
	}
	else if(29 == ucRtn)
	{
		ucRtnSimple = 8;
	}
	else if(31 == ucRtn)
	{
		ucRtnSimple = 9;
	}
	else
	{

	}

	return ucRtnSimple;
}

bool CADSBReceivedProcessMngr::ParseAI(STADSBRawPacket* i_stADSBRawPacket, STADSBDecodedData* i_stADSBDecodedData)
{
	bool bRtn = true;

	int iRstOfInt = 0;
	char acOutputBinary[112] = {0,};

	ConvertCharToBinary(i_stADSBRawPacket->buf+9, i_stADSBRawPacket->buf_size-9, acOutputBinary);
	ConvertBinaryToInt(acOutputBinary+37, 3, &iRstOfInt);

	//TRACE("EC[3bits]: %d\n", iRstOfInt);

	char acCharlookup[65] = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ#####_###############0123456789######";
	stringstream ssAircraftIdentification = stringstream();

	for(int i=0; i<8; i++)
	{
		ConvertBinaryToInt(acOutputBinary+40+(i*6), 6, &iRstOfInt);

		if(strcmp(&acCharlookup[iRstOfInt], "#") != 0)
		{
			char cTempChar = acCharlookup[iRstOfInt];
			ssAircraftIdentification << cTempChar;
		}
	}

	//TRACE("Aircraft Identification[48bits]: %s\n", ssAircraftIdentification.str().c_str());

	strcpy_s(i_stADSBDecodedData->acAircarftID, ssAircraftIdentification.str().size()+1, ssAircraftIdentification.str().c_str());

	return bRtn;
}

bool CADSBReceivedProcessMngr::ParseAP(STADSBRawPacket* i_stADSBRawPacket, STADSBDecodedData* i_stADSBDecodedData)
{
	bool bRtn = true;
	int iRstOfInt = 0;
	char acOutputBinary[112] = {0,};

	ConvertCharToBinary(i_stADSBRawPacket->buf+9, i_stADSBRawPacket->buf_size-9, acOutputBinary);
	ConvertBinaryToInt(acOutputBinary+53, 1, &iRstOfInt);

	int iIsEven = 0;
	float fCPR_LAT = 0;
	float fCPR_LON = 0;

	iIsEven = iRstOfInt;

	// CPR Latitude Odd or Even
	ConvertBinaryToInt(acOutputBinary+54, 17, &iRstOfInt);

	fCPR_LAT = iRstOfInt/131072.0;

	// CPR Longitude Odd or Even
	ConvertBinaryToInt(acOutputBinary+71, 17, &iRstOfInt);

	fCPR_LON = iRstOfInt/131072.0;

	if(iIsEven == 0)
	{
		//TRACE("CPR Even frame!\n");

		i_stADSBDecodedData->isEven = true;
		i_stADSBDecodedData->fLatEven = fCPR_LAT;
		i_stADSBDecodedData->fLonEven = fCPR_LON;
	}
	else
	{
		//TRACE("CPR Odd frame!\n");

		i_stADSBDecodedData->isOdd = true;
		i_stADSBDecodedData->fLatOdd = fCPR_LAT;
		i_stADSBDecodedData->fLonOdd = fCPR_LON;
	}

	// Altitude(feets)
	char acTempAltitude[12] = {0,};

	memcpy(acTempAltitude, acOutputBinary+8, 12);

	int iQBit = acTempAltitude[7];
	int iNValue = 0;

	if(iQBit == 0)
	{
		iNValue = 100;
	}
	else
	{
		iNValue = 25;
	}

	for(int i=7; i<11; i++)
	{
		acTempAltitude[i] = acTempAltitude[i+1];
	}

	ConvertBinaryToInt(acTempAltitude, 11, &iRstOfInt);

	int iNdecimal = iRstOfInt;

	// 최종 경도(Longitude) 값
	unsigned int uiAltitude = iNdecimal * 25/*iNValue*/ - 1000;

	//TRACE("Altitude: %d ft\n", uiAltitude);

	i_stADSBDecodedData->uiAltitude = uiAltitude;

	return bRtn;
}

bool CADSBReceivedProcessMngr::ParseAV(STADSBRawPacket* i_stADSBRawPacket, STADSBDecodedData* i_stADSBDecodedData)
{
	bool bRtn = true;

	int iRstOfInt = 0;
	char acOutputBinary[112] = {0,};

	ConvertCharToBinary(i_stADSBRawPacket->buf+9, i_stADSBRawPacket->buf_size-9, acOutputBinary);
	ConvertBinaryToInt(acOutputBinary+37, 3, &iRstOfInt);

	// [SubType]
	// 1-2: Ground Speed
	// 3-4: Air Speed
	int iSubType = iRstOfInt;

	switch(iRstOfInt)
	{
	case 1:		//Ground Speed
		{
			// Check Signal of Flight Side
			bool bS_ew = false;		// 0(false): flying West to East, 1(true): flying East to West
			bool bS_ns = false;		// 0(false): flying South to North, 1(true): flying North to South
			
			ConvertBinaryToInt(acOutputBinary+13, 1, &iRstOfInt);

			if(iRstOfInt == 1)
			{
				bS_ew = true;
			}

			ConvertBinaryToInt(acOutputBinary+24, 1, &iRstOfInt);

			if(iRstOfInt == 1)
			{
				bS_ns = true;
			}

			// Calculate Horizontal Velocity
			int iV_ew = 0;
			int iV_ns = 0;
			int iV_we = 0;
			int iV_sn = 0;
			float fVel = 0;
			float fHeading = 0;

			ConvertBinaryToInt(acOutputBinary+14, 10, &iRstOfInt);

			iV_ew = iRstOfInt;

			ConvertBinaryToInt(acOutputBinary+25, 10, &iRstOfInt);

			iV_ns = iRstOfInt;

			if(bS_ew == true)
			{
				iV_we = -1 * (iV_ew - 1);
			}
			else
			{
				iV_we = iV_ew - 1;
			}

			if(bS_ns == true)
			{
				iV_sn = -1 * (iV_ns - 1);
			}
			else
			{
				iV_sn = iV_ns - 1;
			}

			fVel = sqrt((float)(iV_we*iV_we+iV_sn*iV_sn));

			i_stADSBDecodedData->bIsGroundSpeed = true;
			i_stADSBDecodedData->fVel = fVel;

			// Calculate heading
			fHeading = atan2((double)iV_we, (double)iV_sn) * 360.0 / (2 * PI);

			if(fHeading < 0)
			{
				fHeading = fHeading + 360;
			}

			i_stADSBDecodedData->fHeading = fHeading;
		}
		break;

	case 3:		//Air Speed
		{
			// Get heading data
			ConvertBinaryToInt(acOutputBinary+13, 1, &iRstOfInt);

			if(iRstOfInt == 1)	// heading data available
			{
				ConvertBinaryToInt(acOutputBinary+14, 10, &iRstOfInt);

				int iDeciHdg = iRstOfInt;

				float	fHeading = iDeciHdg/1024.0 * 360;

				i_stADSBDecodedData->fHeading = fHeading;
			}

			// Get Air speed data
			ConvertBinaryToInt(acOutputBinary+25, 10, &iRstOfInt);

			int iAirSpeed_knot = iRstOfInt;

			i_stADSBDecodedData->bIsGroundSpeed = false;
			i_stADSBDecodedData->fVel = iAirSpeed_knot;
		}
		break;

	case 2:
	case 4:
		{
			//TRACE("Supersonic Aircraft이며, 분석 대상이 아닙니다.\n");
		}
		break;

	default:
		{
			//TRACE("올바르지 않는 SubType(1-4) 값이 아닙니다.\n");
		}
		break;
	}

	return bRtn;
}

bool CADSBReceivedProcessMngr::CalcLat_Lon(bool i_bIsEvenRecently, STADSBDecodedData* i_pstADSBDecodedData)
{
	bool bRtn = true;

	float i_fCPR_LAT_EVEN = i_pstADSBDecodedData->fLatEven;
	float i_fCPR_LON_EVEN = i_pstADSBDecodedData->fLonEven;
	float i_fCPR_LAT_ODD = i_pstADSBDecodedData->fLatOdd;
	float i_fCPR_LON_ODD = i_pstADSBDecodedData->fLonOdd;

	// Calculate the latitude index j
	unsigned int uidxJ = floor( 59.0*i_fCPR_LAT_EVEN - 60.0*i_fCPR_LAT_ODD + 0.5 );

	// Calculate latitude
	unsigned int uiNZ = 15;
	float fDLat_Even = 360 / (float)(4 * uiNZ);
	float fDLat_Odd = 360 / (float)(4 * uiNZ - 1);
	float fLat_Even = fDLat_Even * (uidxJ%60 + i_fCPR_LAT_EVEN);
	float fLat_Odd = fDLat_Odd * (uidxJ%59 + i_fCPR_LAT_ODD);

	if(fLat_Even >= 270)
	{
		fLat_Even -= 360;
	}

	if(fLat_Odd >= 270)
	{
		fLat_Odd -= 360;
	}

	// Final latitude
	float fLat = 0;

	if(i_bIsEvenRecently == true)
	{
		fLat = fLat_Even;
	}
	else
	{
		fLat = fLat_Odd;
	}

	i_pstADSBDecodedData->fLat = fLat;

	//TRACE("[fLat]: %f\n", i_pstADSBDecodedData->fLat);

	// Check NL(Lat_E) and NL(Lat_O) (Computation possibility of a global longitude)
	int iNL_Lat_E = (int)NL_lat(fLat_Even);
	int iNL_Lat_O = (int)NL_lat(fLat_Odd);

	if(iNL_Lat_E == iNL_Lat_O)
	{
		unsigned int uiNi = 0;
		float fDLon = 0;
		int iM = 0;
		float fLon = 0;

		if(i_bIsEvenRecently == true)
		{
			// Calculate Ni
			unsigned int uiTempRst = (unsigned int)NL_lat(fLat_Even);
			if(uiTempRst > 1)
			{
				uiNi = uiTempRst;
			}
			else
			{
				uiNi = 1;
			}

			// Calculate dLon
			fDLon = 360.0/(float)(uiNi);

			// Calculate m
			iM = floor(i_fCPR_LON_EVEN*(NL_lat(fLat_Even)-1.0) - i_fCPR_LON_ODD*NL_lat(fLat_Even) + 0.5);

			// Calculate Lon
			fLon = fDLon*(iM%uiNi + i_fCPR_LON_EVEN);
		}
		else
		{
			// Calculate Ni
			unsigned int uiTempRst = (unsigned int)NL_lat(fLat_Odd);
			if((uiTempRst-1) > 1)
			{
				uiNi = uiTempRst-1;
			}
			else
			{
				uiNi = 1;
			}

			// Calculate dLon
			fDLon = 360.0/(float)(uiNi);

			// Calculate m
			iM = floor(i_fCPR_LON_EVEN*(NL_lat(fLat_Odd)-1) - i_fCPR_LON_ODD*NL_lat(fLat_Odd) + 0.5);

			// Calculate Lon
			fLon = fDLon*(iM%uiNi + i_fCPR_LON_ODD);
		}

		if(fLon >= 180)
		{
			fLon -= 360;
		}

		i_pstADSBDecodedData->fLon = fLon;

		//TRACE("[fLon]: %f\n", i_pstADSBDecodedData->fLon);
	}
	else
	{
		//TRACE("[에러] NL(Lat_Even)과 NL(Lat_Odd)가 동일한 값이 아닙니다.\n");
	}

	return bRtn;
}

bool CADSBReceivedProcessMngr::PutTrackTimeInfo(STADSBDecodedData* i_stADSBDecodedData)
{
	bool bRtn = true;

	SYSTEMTIME stLocalSysTime = SYSTEMTIME();
	CString cstrLocalSysTime = CString();

	GetLocalTime(&stLocalSysTime);
	cstrLocalSysTime.Format("%04d%02d%02d%02d%02d%02d%03d", stLocalSysTime.wYear, stLocalSysTime.wMonth, stLocalSysTime.wDay, stLocalSysTime.wHour, stLocalSysTime.wMinute, stLocalSysTime.wSecond, stLocalSysTime.wMilliseconds);

	strcpy((char*)(&i_stADSBDecodedData->acTrackTime[0]), cstrLocalSysTime);

	return bRtn;
}

bool CADSBReceivedProcessMngr::SendADSBData()
{
	bool bRtn = true;

	STSendADSBData stTempSendADSBData = STSendADSBData();
	map<string, STADSBDecodedData> mTempCopy = map<string, STADSBDecodedData>();

	// IP끝자리에 따른 ADSB ID 적용
	stTempSendADSBData.ucADSB_ID = 1;

	if(m_mapADSBDecodedData.size() > 0)
	{
		mTempCopy.insert(m_mapADSBDecodedData.begin(), m_mapADSBDecodedData.end());

		stTempSendADSBData.ucNumOfData = mTempCopy.size();

		//stTempSendADSBData.pstFlightADSBData = new STFlightADSBData[stTempSendADSBData.ucNumOfData];
		map<string, STADSBDecodedData>::iterator iterMapADSBSendData = mTempCopy.begin();

		for(int i=0; i<stTempSendADSBData.ucNumOfData; i++)
		{
			strcpy_s(stTempSendADSBData.pstFlightADSBData[i].acFlightID, (*iterMapADSBSendData).second.strICAOAddr.size()+1, (*iterMapADSBSendData).second.strICAOAddr.c_str());
			strcpy_s(stTempSendADSBData.pstFlightADSBData[i].acFlightTitle, (*iterMapADSBSendData).second.acAircarftID);
			strcpy_s(stTempSendADSBData.pstFlightADSBData[i].acTrackTime, (*iterMapADSBSendData).second.acTrackTime);
			
			double dbEndianLat = (double)((*iterMapADSBSendData).second.fLat * 1000000);
			int iEndianLat = (int)dbEndianLat;
			stTempSendADSBData.pstFlightADSBData[i].uiLatitude = htonl((int)(iEndianLat));

			double dbEndianLon = (double)((*iterMapADSBSendData).second.fLon * 1000000);
			int iEndianLon = (int)dbEndianLon;
			stTempSendADSBData.pstFlightADSBData[i].uiLongitude = htonl((int)(iEndianLon));

			int iEndianAlt = (*iterMapADSBSendData).second.uiAltitude;
			stTempSendADSBData.pstFlightADSBData[i].uiAltitude = htonl(iEndianAlt);

			unsigned short	usEndianHeading = (unsigned short)((*iterMapADSBSendData).second.fHeading*100);
			stTempSendADSBData.pstFlightADSBData[i].usHeading = htons(usEndianHeading);

			unsigned short	usEndianVel = (unsigned short)((*iterMapADSBSendData).second.fVel);
			stTempSendADSBData.pstFlightADSBData[i].usVelocity = htons(usEndianVel);

			//TRACE("iEndianLat: %d, iEndianLon: %d", iEndianLat, iEndianLon, );
			iterMapADSBSendData++;
		}
	}
	else
	{
		stTempSendADSBData.ucNumOfData = 0;
	}
	
	// 송신
	unsigned short usMsgSize = sizeof(stTempSendADSBData.ucADSB_ID) + sizeof(stTempSendADSBData.ucNumOfData) + sizeof(STFlightADSBData) * stTempSendADSBData.ucNumOfData;

	// Debug Dummy Code
	//STSendADSBData stDummySendADSBData = STSendADSBData();

	//stDummySendADSBData.ucADSB_ID = 1;
	//stDummySendADSBData.ucNumOfData = 1;
	////stDummySendADSBData.pstFlightADSBData = new STFlightADSBData();

	//strcpy_s(stDummySendADSBData.pstFlightADSBData[0].acFlightID, "7890AA");
	//strcpy_s(stDummySendADSBData.pstFlightADSBData[0].acFlightTitle, "KAL1109_");
	//strcpy_s(stDummySendADSBData.pstFlightADSBData[0].acTrackTime, "20190725132040000");

	//double dbEndianLat = (double)(37.123456 * 1000000);
	//int iEndianLat = (int)dbEndianLat;
	//stDummySendADSBData.pstFlightADSBData[0].uiLatitude = htonl((int)(iEndianLat));

	//double dbEndianLon = (double)(127.654321 * 1000000);
	//int iEndianLon = (int)dbEndianLon;
	//stDummySendADSBData.pstFlightADSBData[0].uiLongitude = htonl((int)(iEndianLon));

	//int iEndianAlt = 30000;
	//stDummySendADSBData.pstFlightADSBData[0].uiAltitude = htonl(iEndianAlt);

	//unsigned short	usEndianHeading = (unsigned short)(180.12*100);
	//stDummySendADSBData.pstFlightADSBData[0].usHeading = htons(usEndianHeading);

	//unsigned short	usEndianVel = (unsigned short)(600);
	//stDummySendADSBData.pstFlightADSBData[0].usVelocity = htons(usEndianVel);

	//usMsgSize = sizeof(stDummySendADSBData.ucADSB_ID) + sizeof(stDummySendADSBData.ucNumOfData) + sizeof(STFlightADSBData) * stDummySendADSBData.ucNumOfData;

	//bRtn = m_hCommIF.Send(OPCODE_AS_OV_SEND_ADSBDATA, usMsgSize, 0, 0, m_ucMyLastIP, (void*)&stDummySendADSBData);

	//delete stDummySendADSBData.pstFlightADSBData;

	//////////////////////////////////////////////////////////////////////////

	bRtn = m_hCommIF.Send(OPCODE_AS_OV_SEND_ADSBDATA, usMsgSize, 0, 0, m_ucMyLastIP, (void*)&stTempSendADSBData);
	

	// 메모리가 존재하면 송신 후 해제
	/*if(stTempSendADSBData.pstFlightADSBData != nullptr)
	{
	delete [] stTempSendADSBData.pstFlightADSBData;
	stTempSendADSBData.pstFlightADSBData = nullptr;
	}*/

	return bRtn;
}

unsigned char CADSBReceivedProcessMngr::GetLastIP()
{
	unsigned char ucLastIP = NULL;

	//////// 자신의 IP 획득 //////////////////////////////////////////////////////////////////
	char name[255] = {0,};
	memset(name, NULL, sizeof(name));

	PHOSTENT hostinfo = PHOSTENT();
	memset(&hostinfo, NULL, sizeof(PHOSTENT));

	CString strIP = _T("");
	if ( gethostname(name, sizeof(name)) == NULL )
	{
		hostinfo = gethostbyname(name);
		if ( hostinfo != NULL )
		{
			strIP = inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[0]);
		}
	}

	int iOffset = NULL;
	int iIp = NULL;
	iOffset = strIP.Find(".", 0);
	iOffset = strIP.Find(".", iOffset+1);
	iOffset = strIP.Find(".", iOffset+1);
	strIP = strIP.Mid(iOffset+1, strIP.GetLength()-iOffset-1);
	iIp	= _ttoi(strIP);

	if ( iIp > 0 && iIp < 65536 )
	{
		ucLastIP = (unsigned char)iIp;
	}	 
	//////////////////////////////////////////////////////////////////////////////////////////

	return ucLastIP;
}