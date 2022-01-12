//-----------------------------/
#ifndef __DEF_SOCKET__
#define __DEF_SOCKET__
//------------------------------/

#define	PACKET_HEADER_SIZE		((int)sizeof(int) * 2)

typedef enum {
	emPdfsPacketHeader_1		= 0,
	emPdfsPacketHeader_2,

	emCountPdfsPacketHeader,
} enumPdfsPacketHeader;

typedef enum {
	// 메인데몬 (TCP)
	emWM_SOCKET_ACCEPT_MN			= WM_USER+1,
	emWM_SOCKET_RECEIVE_MN,
	emWM_SOCKET_WARNING_MN,
	emWM_SOCKET_CLOSE_MN,
	// 메인데몬(UDP) - 사용하지는 않음
	emWM_SOCKET_RECEIVE_MN_UDP,					
	emWM_SOCKET_WARNING_MN_UDP,					
	emWM_SOCKET_CLOSE_MN_UDP,					

	// 전파품질측정(TCP)
	emWM_SOCKET_ACCEPT_MR,
	emWM_SOCKET_RECEIVE_MR,
	emWM_SOCKET_WARNING_MR,
	emWM_SOCKET_CLOSE_MR,
	// 전파품질측정(UDP)
	emWM_SOCKET_RECEIVE_MR_UDP,					
	emWM_SOCKET_WARNING_MR_UDP,					
	emWM_SOCKET_CLOSE_MR_UDP,					

	// 점유율측정(TCP)
	emWM_SOCKET_ACCEPT_MO,
	emWM_SOCKET_RECEIVE_MO,
	emWM_SOCKET_WARNING_MO,
	emWM_SOCKET_CLOSE_MO,
	// 점유율측정(UDP)
	emWM_SOCKET_RECEIVE_MO_UDP,					
	emWM_SOCKET_WARNING_MO_UDP,					
	emWM_SOCKET_CLOSE_MO_UDP,

	// 방향탐지(TCP)
	emWM_SOCKET_ACCEPT_DF,
	emWM_SOCKET_RECEIVE_DF,
	emWM_SOCKET_WARNING_DF,
	emWM_SOCKET_CLOSE_DF,
	// 방향탐지(UDP)
	emWM_SOCKET_RECEIVE_DF_UDP,					
	emWM_SOCKET_WARNING_DF_UDP,					
	emWM_SOCKET_CLOSE_DF_UDP,

	//////////////////////////////////////////////////////////////////////////

	emWM_SOCKET_RECEIVE_DEM,
	emWM_SOCKET_WARNING_DEM,
	emWM_SOCKET_CLOSE_DEM,

	emWM_Socket_Receive_CDMA,
	emWM_Socket_Warning_CDMA,
	emWM_Socket_Close_CDMA,

	emWM_Diagram_Receive_CDMA,
	emWM_Diagram_Warning_CDMA,
	emWM_Diagram_Close_CDMA,

	emWM_Broadcast_Receive_PDFS,
	emWM_Broadcast_Warning_PDFS,
	emWM_Broadcast_Close_PDFS,

	emWM_CDMA_Connect_PDFS,
	emWM_CDMA_Socket_Accept,

	emWM_Socket_Receive_Simulator,
	emWM_Socket_Warning_Simulator,
	emWM_Socket_Close_Simulator,

	//////////////////////////////////////////////////////////////////////////
	// IQ데이터 전송용(TCP)
	emWM_SOCKET_ACCEPT_IQ,
	emWM_SOCKET_RECEIVE_IQ,
	emWM_SOCKET_WARNING_IQ,
	emWM_SOCKET_CLOSE_IQ,
	//////////////////////////////////////////////////////////////////////////

	// FFT 전송용(TCP)
	emWM_SOCKET_ACCEPT_FFT,
	emWM_SOCKET_RECEIVE_FFT,
	emWM_SOCKET_WARNING_FFT,
	emWM_SOCKET_CLOSE_FFT,

	// 준고정형(TCP)
	emWM_SOCKET_ACCEPT_SFMS,
	emWM_SOCKET_RECEIVE_SFMS,
	emWM_SOCKET_WARNING_SFMS,
	emWM_SOCKET_CLOSE_SFMS,

	emWM_SOCKET_ACCEPT_SFMS_FFT,
	emWM_SOCKET_RECEIVE_SFMS_FFT,
	emWM_SOCKET_WARNING_SFMS_FFT,
	emWM_SOCKET_CLOSE_SFMS_FFT,

	emCountSocketEventTag,
} enumSocketEventTag;

typedef enum {
	emPdfsGetSystemInfo_TAG			= 0,			
	emPdfsCheckUserInfo_TAG,						
	emPdfsManagementUser_TAG,						
	emPdfsUpdateEnvInfo_TAG,						
	emPdfsSaveRemoteResult_TAG,						
	emPdfsSaveLocalResult_TAG,						
	emPdfsLoadRemoteResult_TAG,						
	emPdfsLoadLocalResult_TAG,						
	emPdfsLoadResultEnd_TAG,						
	emDummyDatabaseMessageEnd_TAG,					
} enumPdfsDatabaseMessageTag;

typedef enum {
	emPdfsRadioCommand_TAG			= 0,			
	emDummyRadioMessageEnd_TAG,
} enumPdfsRadioMessageTag;

typedef enum {
	emPdfsDeviceMeasureEnv_TAG		= 0,			
	emPdfsDeviceMeasureStop_TAG,					
	emDummyDeviceMessageEnd_TAG,
} enumPdfsDeviceMessageTag;

typedef enum {
	emPdfsMesaureTime_GPS_TAG		= 0,
	emPdfsLatitude_GPS_TAG,
	emPdfsLongitude_GPS_TAG,
	emPdfsState_GPS_TAG,
	emPdfsSatelliteNum_GPS_TAG,
	emPdfsHDOP_GPS_TAG,
	emPdfsAntennaHeight_GPS_TAG,
	emPdfsMesaureDay_GPS_TAG,
	emPdfsMesaureMonth_GPS_TAG,
	emPdfsMesaureYear_GPS_TAG,
	emPdfsTrueBearing_GPS_TAG,
	emPdfsMagneticBearing_GPS_TAG,
	emPdfsGroundSpeedKNT_GPS_TAG,
	emPdfsGroundSpeedKMH_GPS_TAG,
	emPdfsMode_GPS_TAG,
	emPdfsCompass_Tag,
} enumPdfsDeviceLogTag;

typedef enum {
	emPdfsAntenna_GPS_TAG		= 0,				
	emPdfsAntenna_DGPS_TAG,							
	emPdfsAntenna_DF_TAG,							
	emDummyAntennaMessageEnd_TAG,
} enumPdfsAntennaMessageTag;



#if !defined(DEF_ENUM_PACKET_TAG) // ###################################

typedef enum _enumPacketTag
{
    PID_PACKET_TOP	                    = 0,	//                                              
    PID_SYS_REBOOT,                             // 시스템 재부팅                                
                                                                                                
    //--------------------------------------------                                              
    // 제어 데몬 - 소켓 연결 관련                                                               
    //--------------------------------------------                                              
    PID_MON_CONNECT_MN,                         // 메인데몬 연결                                
    PID_MON_DISCONNECT_MN,                      // 메인데몬 연결 종료                           
                                                                                                
    //--------------------------------------------                                              
    // 전파품질 제어 데몬                                                                       
    //--------------------------------------------                                              
    PID_MON_CONFIRM_MR,                         // 전파품질 데몬 연결확인                       
    PID_MON_CONNECT_MR,                         // 전파품질 데몬 연결                           
    PID_MON_DISCONNECT_MR,                      // 전파품질 데몬 연결 종료                      
    PID_MON_INIT_MR,                            // 전파품질 데몬 초기화                         
                                                                                                
    //--------------------------------------------                                              
    // 불법전파 제어 데몬 - NOT USED -                                                          
    //--------------------------------------------                                              
    PID_MON_CONFIRM_MI,                         // 불법전파 데몬 연결확인                       
    PID_MON_CONNECT_MI,                         // 불법전파 데몬 연결                           
    PID_MON_DISCONNECT_MI,                      // 불법전파 데몬 연결 종료                      
    PID_MON_INIT_MI,                            // 불법전파 데몬 초기화                         
                                                                                                
    //--------------------------------------------                                              
    // 점유율 데몬                                                                              
    //--------------------------------------------                                              
    PID_MON_CONFIRM_MO,                         // 수신환경조사 데몬 연결확인                   
    PID_MON_CONNECT_MO,                         // 수신환경조사 데몬 연결                       
    PID_MON_DISCONNECT_MO,                      // 수신환경조사 데몬 연결 종료                  
    PID_MON_INIT_MO,                            // 수신환경조사 데몬 초기화                     
                                                                                                
    //--------------------------------------------                                              
    // 측정 파라미터 구분                                                                       
    //--------------------------------------------                                              
    PID_MEASURE_PARAM_MR_DT,                    // 전파품질 직접측정                            
    PID_MEASURE_PARAM_MR_SE,                    // 전파스펙트럼측정                             
    PID_MEASURE_PARAM_MR_AT,                    // 최대수신방위각                               
    PID_MEASURE_PARAM_MR_SP_S,                  // 스퓨리어스측정                               
    PID_MEASURE_PARAM_MR_SP_H,                  // 고조파측정                                   
    // 전파품질 - 채널                                                                          
    PID_MEASURE_PARAM_MR_CH_S,                  // 채널정보 전송 시작                           
    PID_MEASURE_PARAM_MR_CH_D,                  // 채널정보 데이터                              
    PID_MEASURE_PARAM_MR_CH_E,                  // 채널정보 전송 종료                           
    // 불법전파 - 채널                                                                          
    PID_MEASURE_PARAM_MI_CH_S,                  // 채널정보 전송 시작                           
    PID_MEASURE_PARAM_MI_CH_D,                  // 채널정보 데이터                              
    PID_MEASURE_PARAM_MI_CH_E,                  // 채널정보 전송 종료                           
    // 불법전파 - 대역                                                                          
    PID_MEASURE_PARAM_MI_BW,                    // 대역측정                                     
    // 점유율 - 채널                                                                            
    PID_MEASURE_PARAM_MO_CH_S,                  // 채널정보 전송 시작                           
    PID_MEASURE_PARAM_MO_CH_D,                  // 채널정보 데이터                              
    PID_MEASURE_PARAM_MO_CH_E,                  // 채널정보 전송 종료                           
    // 점유율 - 대역                                                                            
    PID_MEASURE_PARAM_MO_BW,                    // 대역측정                                     
                                                                                                
    // MR/MI/MO 자동임계레벨 가측정	                                                            
    PID_MEASURE_PARAM_MR_TH,                    // 임계레벨                                     
    PID_MEASURE_PARAM_MI_TH,                    // 임계레벨                                     
    PID_MEASURE_PARAM_MI_TH_RESEND, 	        // 임계레벨을 1000파씩 이어받기                 
    PID_MEASURE_PARAM_MO_TH,                    // 임계레벨                                     
                                                                                                
    PID_MEASURE_PARAM_MR_ENV,                   // 전파품질 환경정보                            
    PID_MEASURE_PARAM_MI_ENV,                   // 불법전파 환경정보                            
    PID_MEASURE_PARAM_MO_ENV,                   // 수신환경조사 환경정보                        
                                                                                                
    //--------------------------------------------                                              
    // 측정 모드 구분/응답 테그                                                                 
    //--------------------------------------------                                              
    PID_MEASURE_PARAM_MR_DT_R,                  // 전파품질 직접측정 응답                       
    PID_MEASURE_PARAM_MR_SE_R,                  // 전파스펙트럼측정 응답                        
    PID_MEASURE_PARAM_MR_AT_R,                  // 최대수신방위각 응답 [지능형]                 
    PID_MEASURE_PARAM_MR_SP_S_R,                // 스퓨리어스측정 응답                          
    PID_MEASURE_PARAM_MR_SP_H_R,                // 고조파측정 응답                              
    // 전파품질 - 채널 -                                                                        
    PID_MEASURE_PARAM_MR_CH_S_R,                // 전파품질 채널정보 전송 시작 응답             
    PID_MEASURE_PARAM_MR_CH_D_R,                // 전파품질 채널정보 데이터 응답                
    PID_MEASURE_PARAM_MR_CH_E_R,                // 전파품질 채널정보 전송 종료 응답             
    // 불법전파 - 채널 - 지능형			                                                        
    PID_MEASURE_PARAM_MI_CH_S_R,                // 불법전파 채널정보 전송 시작 응답             
    PID_MEASURE_PARAM_MI_CH_D_R,                // 불법전파 채널정보 데이터 응답                
    PID_MEASURE_PARAM_MI_CH_E_R,                // 불법전파 채널정보 전송 종료 응답             
    PID_MEASURE_PARAM_MI_BW_R,                  // 불법전파 대역측정 응답                       
	// 수신환경조사 - 채널 -	                                                                
    PID_MEASURE_PARAM_MO_CH_S_R,                // 수신환경조사 채널정보 전송 시작 응답         
    PID_MEASURE_PARAM_MO_CH_D_R,                // 수신환경조사 채널정보 데이터 응답            
    PID_MEASURE_PARAM_MO_CH_E_R,                // 수신환경조사 채널정보 전송 종료 응답         
    PID_MEASURE_PARAM_MO_BW_R,                  // 수신환경조사 대역측정 응답                   
                                                                                                
    //측정 시작                                                                                 
    PID_MEASURE_START_MR,                       	                                            
    PID_MEASURE_START_MI,                                                                       
    PID_MEASURE_START_MO,                                                                       
    PID_MEASURE_START_ME,                                                                       
    PID_MEASURE_START_MP,                                                                       
    //측정 중지                                                                                 
    PID_MEASURE_STOP_MR,                                                                        
    PID_MEASURE_STOP_MI,                                                                        
    PID_MEASURE_STOP_MO,                                                                        
    PID_MEASURE_STOP_ME,                                                                        
    PID_MEASURE_STOP_MP,                                                                        
    //측정 완료                                                                                 
    PID_MEASURE_COMPLETE_MR,                                                                    
    PID_MEASURE_COMPLETE_MI,                                                                    
    PID_MEASURE_COMPLETE_MO,                                                                    
    PID_MEASURE_COMPLETE_ME,                                                                    
    PID_MEASURE_COMPLETE_MP,                                                                    
                                                                                                
    // 가측정 시작                                                                              
    PID_MEASURE_START_MR_DUMMY,                                                                 
    PID_MEASURE_START_MI_DUMMY,                                                                 
    PID_MEASURE_START_MO_DUMMY,                                                                 
    PID_MEASURE_START_ME_DUMMY,                                                                 
    PID_MEASURE_START_MP_DUMMY,                                                                 
    // 가측정 중지                                                                              
    PID_MEASURE_STOP_MR_DUMMY,                                                                  
    PID_MEASURE_STOP_MI_DUMMY,                                                                  
    PID_MEASURE_STOP_MO_DUMMY,                                                                  
    PID_MEASURE_STOP_ME_DUMMY,                                                                  
    PID_MEASURE_STOP_MP_DUMMY,                                                                  
    // 가측정 완료                                                                              
    PID_MEASURE_COMPLETE_MR_DUMMY,                                                              
    PID_MEASURE_COMPLETE_MI_DUMMY,                                                              
    PID_MEASURE_COMPLETE_MO_DUMMY,                                                              
    PID_MEASURE_COMPLETE_ME_DUMMY,                                                              
    PID_MEASURE_COMPLETE_MP_DUMMY,                                                              
    // 스펙트럼 데이터                                                                          
    PID_MEASURE_FFT_MR_INIT_ENV,                                                                
    PID_MEASURE_FFT_MI_INIT_ENV,                                                                
    PID_MEASURE_FFT_MO_INIT_ENV,                                                                
    PID_MEASURE_FFT_MR_CURRENT,                                                                 
    PID_MEASURE_FFT_MI_CURRENT,                                                                 
    PID_MEASURE_FFT_MO_CURRENT,                                                                 
    // 측정 결과                                                                                
    PID_MEASURE_RESULT_MR,                                                                      
    PID_MEASURE_RESULT_MI,                                                                      
    PID_MEASURE_RESULT_MO,                                                                      
    PID_MEASURE_RESULT_ME,                                                                      
    PID_MEASURE_RESULT_MP,                                                                      
    PID_MEASURE_RESULT_AT,                                                                      
    PID_MEASURE_RESULT_TH,                                                                      
    // 1회 스캔 완료		                                                                    
    PID_MEASURE_SCAN_1_COMPLETE_ME,                                                             
    PID_MEASURE_SCAN_1_COMPLETE_MI,                                                             
    PID_MEASURE_SCAN_1_COMPLETE_MO,                                                             
                                                                                                
    // 불법전파탐사 - 대역측정시 허가주파수대역 제외하기 위해                                   
    PID_LICENSE_BAND_INFO,                      // 허가대역 정보 송신                           
    PID_LICENSE_BAND_INFO_R,                    // 허가대역 정보 수신 응답                      
                                                                                                
    //--------------------------------------------                                              
    // 측정장비 직접제어 관련                                                                   
    //--------------------------------------------                                              
    PID_DIRECT_RX_INIT_S,                       // 수신기 초기화                                
    PID_DIRECT_RX_MEASURE_TIME_MR_S,            // 전파품질 측정시간 설정                       
    PID_DIRECT_RX_MEASURE_TIME_MR_R,            // 전파품질 측정시간 설정 응답                  
    PID_DIRECT_RX_MEASURE_TIME_FSCAN_S,         // FSCAN 측정시간 설정                          
    PID_DIRECT_RX_MEASURE_TIME_FSCAN_R,         // FSCAN 측정시간 설정 응답                     
    PID_DIRECT_RX_MEASURE_TIME_MSCAN_S,         // MSCAN 측정시간 설정                          
    PID_DIRECT_RX_MEASURE_TIME_MSCAN_R,         // MSCAN 측정시간 설정 응답                     
    PID_DIRECT_RX_MEASURE_COUNT_MR_S,           // 전파품질 측정횟수 설정                       
    PID_DIRECT_RX_MEASURE_COUNT_MR_R,           // 전파품질 측정횟수 설정 응답                  
    PID_DIRECT_RX_MEASURE_COUNT_FSCAN_S,        // FSCAN 측정횟수 설정                          
    PID_DIRECT_RX_MEASURE_COUNT_FSCAN_R,        // FSCAN 측정횟수 설정 응답                     
    PID_DIRECT_RX_MEASURE_COUNT_MSCAN_S,        // MSCAN 측정횟수 설정                          
    PID_DIRECT_RX_MEASURE_COUNT_MSCAN_R,        // MSCAN 측정횟수 설정 응답                     
    PID_DIRECT_RX_MODE_S,                       // 수신기모드[FFM,FSCAN,MSCAN] 설정             
    PID_DIRECT_RX_MODE_Q,                       // 수신기모드[FFM,FSCAN,MSCAN] 설정값 요청      
    PID_DIRECT_RX_MODE_R,                       // 수신기모드[FFM,FSCAN,MSCAN] 설정값 응답      
    PID_DIRECT_RX_FREQ_S,                       // 수신기 주파수 설정                           
    PID_DIRECT_RX_FREQ_Q,                       // 수신기 주파수 설정값 요청                    
    PID_DIRECT_RX_FREQ_R,                       // 수신기 주파수 설정값 응답                    
    PID_DIRECT_RX_DEMOD_S,                      // 수신기 변조형식 설정                         
    PID_DIRECT_RX_DEMOD_Q,                      // 수신기 변조형식 설정값 요청                  
    PID_DIRECT_RX_DEMOD_R,                      // 수신기 변조형식 설정값 응답                  
    PID_DIRECT_RX_BW_S,                         // 수신기 대역폭 설정                           
    PID_DIRECT_RX_BW_Q,                         // 수신기 대역폭 설정값 요청                    
    PID_DIRECT_RX_BW_R,                         // 수신기 대역폭 설정값 응답                    
    PID_DIRECT_RX_SQUELCH_STATE_S,              // 수신기 스켈치 설정                           
    PID_DIRECT_RX_SQUELCH_STATE_Q,              // 수신기 스켈치 설정값 요청                    
    PID_DIRECT_RX_SQUELCH_STATE_R,              // 수신기 스켈치 설정값 응답                    
    PID_DIRECT_RX_THRESHOLD_S,                  // 수신기 임계레벨 설정                         
    PID_DIRECT_RX_THRESHOLD_Q,                  // 수신기 임계레벨 설정값 요청                  
    PID_DIRECT_RX_THRESHOLD_R,                  // 수신기 임계레벨 설정값 응답                  
    PID_DIRECT_RX_AFC_STATE_S,                  // 수신기 AFC ON/OFF 설정                       
    PID_DIRECT_RX_AFC_STATE_Q,                  // 수신기 AFC ON/OFF 설정값 요청                
    PID_DIRECT_RX_AFC_STATE_R,                  // 수신기 AFC ON/OFF 설정값 응답                
    PID_DIRECT_RX_ATT_MODE_T,                   // 수신기 ATT 자동/수동 설정 [무응답]           
    PID_DIRECT_RX_ATT_MODE_S,                   // 수신기 ATT 자동/수동 설정                    
    PID_DIRECT_RX_ATT_MODE_Q,                   // 수신기 ATT 자동/수동 설정값 요청             
    PID_DIRECT_RX_ATT_MODE_R,                   // 수신기 ATT 자동/수동 설정값 응답             
    PID_DIRECT_RX_ATT_VALUE_T,                  // 수신기 ATT 값 설정 [무응답]                  
    PID_DIRECT_RX_ATT_VALUE_S,	                // 수신기 ATT 값 설정                           
    PID_DIRECT_RX_ATT_VALUE_Q,	                // 수신기 ATT 값 설정값 요청                    
    PID_DIRECT_RX_ATT_VALUE_R,	                // 수신기 ATT 값 설정값 응답                    
    PID_DIRECT_RX_RF_MODE_S,	                // 수신기 NOR/LNA/DISTORTION 설정               
    PID_DIRECT_RX_RF_MODE_Q,	                // 수신기 NOR/LNA/DISTORTION 설정값 요청        
    PID_DIRECT_RX_RF_MODE_R,	                // 수신기 NOR/LNA/DISTORTION 설정값 응답        
    PID_DIRECT_RX_GAIN_SEL_T,	                // 수신기 GAIN 자동/수동 설정 [무응답]          
    PID_DIRECT_RX_GAIN_SEL_S,	                // 수신기 GAIN 자동/수동 설정                   
    PID_DIRECT_RX_GAIN_SEL_Q,	                // 수신기 GAIN 자동/수동 설정값 요청            
    PID_DIRECT_RX_GAIN_SEL_R,	                // 수신기 GAIN 자동/수동 설정값 응답            
    PID_DIRECT_RX_GAIN_VALUE_T,                 // 수신기 GAIN 값 설정 [무응답]                 
    PID_DIRECT_RX_GAIN_VALUE_S,                 // 수신기 GAIN 값 설정                          
    PID_DIRECT_RX_GAIN_VALUE_Q,                 // 수신기 GAIN 값 설정값 요청                   
    PID_DIRECT_RX_GAIN_VALUE_R,                 // 수신기 GAIN 값 설정값 응답                   
    PID_DIRECT_RX_LEVEL_INDICATE_SEL_S,         // 수신레벨[AVG/PEAK/FAST/RMS] 설정             
    PID_DIRECT_RX_LEVEL_INDICATE_SEL_Q,         // 수신레벨[AVG/PEAK/FAST/RMS] 설정값 요청      
    PID_DIRECT_RX_LEVEL_INDICATE_SEL_R,         // 수신레벨[AVG/PEAK/FAST/RMS] 설정값 응답      
    PID_DIRECT_RX_VOLUME_S,                     // 수신기 볼륨 설정                             
    PID_DIRECT_RX_VOLUME_Q,                     // 수신기 볼륨 설정값 요청                      
    PID_DIRECT_RX_VOLUME_R,                     // 수신기 볼륨 설정값 응답                      
    PID_DIRECT_RX_CALFILTER_STATE_S,            // 수신기 CAL FILTER[ON/OFF] 설정               
    PID_DIRECT_RX_CALFILTER_STATE_Q,            // 수신기 CAL FILTER[ON/OFF] 설정값 요청        
    PID_DIRECT_RX_CALFILTER_STATE_R,            // 수신기 CAL FILTER[ON/OFF] 설정값 응답        
    PID_DIRECT_RX_FPDP_MODE_S,                  // 수신기 FPDP [BIT32_FLAG] 설정                
    PID_DIRECT_RX_FPDP_MODE_Q,                  // 수신기 FPDP [BIT32_FLAG] 설정값 요청         
    PID_DIRECT_RX_FPDP_MODE_R,                  // 수신기 FPDP [BIT32_FLAG] 설정값 응답         
    PID_DIRECT_RX_LEVEL_Q,                      // 수신기 수신레벨 요청                         
    PID_DIRECT_RX_LEVEL_R,                      // 수신기 수신레벨 응답                         
    PID_DIRECT_RX_SELECT_DEVICE_S,              // 수신기 [1/2/3/4/5] 선택                      
    PID_DIRECT_RX_SELECT_DEVICE_R,              // 수신기 [1/2/3/4/5] 선택 응답                 
                                                                                                
    //--------------------------------------------                                              
    // 안테나/전단기/스위치 제어 관련 - 지능형                                                  
    //--------------------------------------------                                              
    PID_DIRECT_ANT_CH_SEL_S,                                                                    
    PID_DIRECT_ANT_CH_SEL_R,                                                                    
    PID_DIRECT_ANT_DIR_ANGLE_S,                                                                 
    PID_DIRECT_ANT_DIR_ANGLE_R,                                                                 
    PID_DIRECT_ANT_DIR_ANGLE_E,                                                                 
    PID_DIRECT_ANT_DIR_STOP_S,                                                                  
    PID_DIRECT_ANT_DIR_STOP_R,                                                                  
    PID_DIRECT_ANT_ROTATE_CW_S,                                                                 
    PID_DIRECT_ANT_ROTATE_CW_R,                                                                 
    PID_DIRECT_ANT_ROTATE_CCW_S,                                                                
    PID_DIRECT_ANT_ROTATE_CCW_R,                                                                
    PID_DIRECT_ANT_STATE_QUERY_S,                                                               
    PID_DIRECT_ANT_STATE_QUREY_R,                                                               
    PID_DIRECT_ANT_PRESELECT_S,                                                                 
    PID_DIRECT_ANT_PRESELECT_R,                                                                 
    PID_DIRECT_ANT_CYCLE_ATTRIB_S,                                                              
    PID_DIRECT_ANT_CYCLE_ATTRIB_R,                                                              
    PID_DIRECT_ANT_CYCLE_ATTRIB_Q,                                                              
                                                                                                
    //--------------------------------------------                                              
    // 시스템 구성 관련 - 측정                                                                  
    //--------------------------------------------                                              
    PID_SYS_MON_CONFIG_SET_Q,                   // 측정용 안테나타입/모드 설정                  
    PID_SYS_MON_CONFIG_SET_R,                   // 측정용 안테나타입/모드 설정값 응답           
                                                                                                
    //--------------------------------------------                                              
    // 시스템 보정 관련 - 측정                                                                  
    //--------------------------------------------                                              
    PID_SYS_MON_CAL_INIT_S,                     // 측정용 시스템보정 초기테이블값 요청          
    PID_SYS_MON_CAL_INIT_R,                     // 측정용 시스템보정 초기테이블값 응답          
    PID_SYS_MON_CAL_INIT_E,                     // 측정용 시스템보정 초기테이블값 응답 종료     
    PID_SYS_MON_CAL_MEASURE_S,                  // 측정용 시스템보정 시작                       
    PID_SYS_MON_CAL_MEASURE_R,                  // 측정용 시스템보정 결과값 응답                
    PID_SYS_MON_CAL_MEASURE_E,                  // 측정용 시스템보정 종료                       
    PID_SYS_MON_CAL_MEASURE_P,            	    // 측정용 시스템보정값 레지스트리 반영	        
    PID_SYS_MON_CAL_MEASURE_C,                  // 측정용 시스템보정 취소 요청                  
    		                                                                                    
    PID_SYS_MON_ANT_FACTOR_INIT_S,              // 측정용 안테나 타입별 팩터값 전송 시작        
    PID_SYS_MON_ANT_FACTOR_INIT_R,           	// 측정용 안테나 타입별 팩터값 전송 응답        
    PID_SYS_MON_ANT_FACTOR_INIT_E,          	// 측정용 안테나 타입별 팩터값 전송 종료        
                                                                                                
    //--------------------------------------------                                              
    // 자기진단 관련                                                                            
    // 응답 : (INT-R == 0 정상 / INT-R < 0 오류)                                                
    //--------------------------------------------                                              
    PID_DEVICE_STATUS_RX1_Q,                    // 수신기#1 자기진단 요청                       
    PID_DEVICE_STATUS_RX1_R,                    // 수신기#1 자기진단 응답                       
    PID_DEVICE_STATUS_RX2_Q,                    // 수신기#2 자기진단 요청                       
    PID_DEVICE_STATUS_RX2_R,                    // 수신기#2 자기진단 응답                       
    PID_DEVICE_STATUS_RX3_Q,                    // 수신기#3 자기진단 요청                       
    PID_DEVICE_STATUS_RX3_R,                    // 수신기#3 자기진단 응답                       
    PID_DEVICE_STATUS_RX4_Q,                    // 수신기#4 자기진단 요청                       
    PID_DEVICE_STATUS_RX4_R,                    // 수신기#4 자기진단 응답                       
    PID_DEVICE_STATUS_RX5_Q,                    // 수신기#5 자기진단 요청                       
    PID_DEVICE_STATUS_RX5_R,                    // 수신기#5 자기진단 응답            	        
    PID_DEVICE_STATUS_ANT_Q,                    // 안테나 제어기 자기진단 요청                  
    PID_DEVICE_STATUS_ANT_R,                    // 안테나 제어기 자기진단 응답                  
    PID_DEVICE_STATUS_RFC_Q,                    // 안테나 신호분배기 자기진단 요청              
    PID_DEVICE_STATUS_RFC_R,                    // 안테나 신호분배기 자기진단 응답              
    PID_DEVICE_STATUS_DSP_Q,                    // DSP 자기진단 요청                            
    PID_DEVICE_STATUS_DSP_R,                    // DSP 자기진단 응답                            
    PID_DEVICE_STATUS_ATR_Q,                    // 안테나회전기 자기진단 요청                   
    PID_DEVICE_STATUS_ATR_R,                    // 안테나회전기 자기진단 응답                   
                                                                                                
    //--------------------------------------------                                              
    // 시스템 상세 진단                                                                         
    // 응답 : (각 장비별 정의된 상태 비트 5개)                                                  
    //--------------------------------------------                                              
    PID_DEVICE_DETAIL_STATUS_RX1_Q,             // 수신기#1 상세진단 요청                       
    PID_DEVICE_DETAIL_STATUS_RX1_R,             // 수신기#1 상세진단 응답                       
    PID_DEVICE_DETAIL_STATUS_RX2_Q,             // 수신기#2 상세진단 요청                       
    PID_DEVICE_DETAIL_STATUS_RX2_R,             // 수신기#2 상세진단 응답                       
    PID_DEVICE_DETAIL_STATUS_RX3_Q,             // 수신기#3 상세진단 요청                       
    PID_DEVICE_DETAIL_STATUS_RX3_R,             // 수신기#3 상세진단 응답                       
    PID_DEVICE_DETAIL_STATUS_RX4_Q,             // 수신기#4 상세진단 요청                       
    PID_DEVICE_DETAIL_STATUS_RX4_R,             // 수신기#4 상세진단 응답                       
    PID_DEVICE_DETAIL_STATUS_RX5_Q,             // 수신기#5 상세진단 요청                       
    PID_DEVICE_DETAIL_STATUS_RX5_R,             // 수신기#5 상세진단 응답            	        
    PID_DEVICE_DETAIL_STATUS_ANT_Q,             // 안테나 제어기 상세진단 요청                  
    PID_DEVICE_DETAIL_STATUS_ANT_R,             // 안테나 제어기 상세진단 응답                  
    PID_DEVICE_DETAIL_STATUS_RFC_Q,             // 안테나 신호분배기 상세진단 요청              
    PID_DEVICE_DETAIL_STATUS_RFC_R,             // 안테나 신호분배기 상세진단 응답              
    PID_DEVICE_DETAIL_STATUS_DSP_Q,             // DSP 상세진단 요청                            
    PID_DEVICE_DETAIL_STATUS_DSP_R,             // DSP 상세진단 응답                            
    PID_DEVICE_DETAIL_STATUS_ATR_Q,             // 안테나회전기 상세진단 요청                   
    PID_DEVICE_DETAIL_STATUS_ATR_R,             // 안테나회전기 상세진단 응답                   
                                                                                                
    //--------------------------------------------                                              
    // 음성복조 수동 제어 관련                                                                  
    //--------------------------------------------                                              
    PID_DEMOD_MANUAL_OPERATE_SET_S,                                                             
    PID_DEMOD_MANUAL_OPERATE_SET_R,                                                             
    PID_DEMOD_MANUAL_OPERATE_REC_S,                                                             
    PID_DEMOD_MANUAL_OPERATE_REC_R,                                                             
    PID_DEMOD_MANUAL_OPERATE_SET_END_S,                                                         
    PID_DEMOD_MANUAL_OPERATE_SET_END_R,                                                         
    PID_DEMOD_MANUAL_OPERATE_REC_END_S,                                                         
    PID_DEMOD_MANUAL_OPERATE_REC_END_R,                                                         
    		                                                                                    
    // 수신기별 [AFC]                                                                           
    PID_CURRENT_FREQ_RX1_Q,                                                                     
    PID_CURRENT_FREQ_RX1_R,                     // 현재 측정중인 주파수 [UDP 소켓] 전송용       
    PID_CURRENT_FREQ_RX2_Q,                                                                     
    PID_CURRENT_FREQ_RX2_R,                                                                     
    PID_CURRENT_FREQ_RX3_Q,                                                                     
    PID_CURRENT_FREQ_RX3_R,                                                                     
    PID_CURRENT_FREQ_RX4_Q,                                                                     
    PID_CURRENT_FREQ_RX4_R,                                                                     
    PID_CURRENT_FREQ_RX5_Q,                                                                     
    PID_CURRENT_FREQ_RX5_R,                                                                     
    // 수신기별 신호레벨 처리                                                                   
    PID_CURRENT_LEVEL_RX1_Q,                                                                    
    PID_CURRENT_LEVEL_RX1_R,					// 전파품질측정: 잡음레벨의 경우 채널파워(double형으로) 전송 겸용
    PID_CURRENT_LEVEL_RX2_Q,                                                                    
    PID_CURRENT_LEVEL_RX2_R,                                                                    
    PID_CURRENT_LEVEL_RX3_Q,                                                                    
    PID_CURRENT_LEVEL_RX3_R,                                                                    
    PID_CURRENT_LEVEL_RX4_Q,                                                                    
    PID_CURRENT_LEVEL_RX4_R,                                                                    
    PID_CURRENT_LEVEL_RX5_Q,                                                                    
    PID_CURRENT_LEVEL_RX5_R,                                                                    
                                                                                                
    PID_MON_ERROR,								// 측정데몬 에러인 경우 [ MON --> OP ] 전송     
                                                                                                
    //--------------------------------------------                                              
    // 운용감시 - NOT USED -                                                                    
    //--------------------------------------------                                              
    PID_MON_CONFIRM_MA,                                                                         
    PID_MON_CONNECT_MA,                                                                         
    PID_MON_DISCONNECT_MA,                                                                      
    PID_MON_INIT_MA,                                                                            
    PID_MEASURE_COMPLETE_MA,                                                                    
    PID_MEASURE_PARAM_MA_DT,                                                                          
    PID_MEASURE_PARAM_MA_BW_S,                                                                        
    PID_MEASURE_PARAM_MA_BW_D,                                                                        
    PID_MEASURE_PARAM_MA_BW_E,                                                                        
    PID_MEASURE_PARAM_MA_CH_S,                                                                        
    PID_MEASURE_PARAM_MA_CH_D,                                                                        
    PID_MEASURE_PARAM_MA_CH_E,                                                                        
    PID_MEASURE_PARAM_MA_DT_R,                                                                        
    PID_MEASURE_PARAM_MA_BW_S_R,                                                                      
    PID_MEASURE_PARAM_MA_BW_D_R,                                                                      
    PID_MEASURE_PARAM_MA_BW_E_R,                                                                      
    PID_MEASURE_PARAM_MA_CH_S_R,                                                                      
    PID_MEASURE_PARAM_MA_CH_D_R,                                                                      
    PID_MEASURE_PARAM_MA_CH_E_R,                                                                      
    //--------------------------------------------                                              
    // 대역 SKIP                                                      
    PID_MEASURE_PARAM_MA_BW_SKIP_S,                                                                 
    PID_MEASURE_PARAM_MA_BW_SKIP_D,                                                                 
    PID_MEASURE_PARAM_MA_BW_SKIP_E,                                                                 
                                                                                                
    PID_MEASURE_START_MA,                                                                       
    PID_MEASURE_STOP_MA,                                                                        
    PID_MEASURE_PARAM_MA,                                                                       
    PID_MEASURE_PARAM_MA_R,                                                                     
    PID_MEASURE_RESULT_MA_RX1,                                                                  
    PID_MEASURE_RESULT_MA_RX2,                                                                  
    PID_MEASURE_RESULT_MA_RX3,                                                                  
    PID_MEASURE_RESULT_MA_RX4,                                                                  
    //--------------------------------------------                                              
    PID_MEASURE_HOLD_MA,                                                                        
    PID_MEASURE_PLAY_MA,                                                                        
    PID_MEASURE_REC_STRAT_MA,                                                                   
    PID_MEASURE_REC_STOP_MA,                                                                    
    PID_MEASURE_SCAN_COMPLETE_MA,                                                               
    PID_MEASURE_SCAN_1_COMPLETE_MA,                                                             
    //--------------------------------------------		                                        

	PID_RX1_ERROR,                                                                         
	PID_RX1_INIT,                                                                          
	PID_RX2_ERROR,                                                                         
	PID_RX2_INIT,                                                                          
	PID_RX3_ERROR,                                                                         
	PID_RX3_INIT,                                                                          
	PID_RX4_ERROR,                                                                         
	PID_RX4_INIT,                                                                          
    PID_RX5_ERROR,                                                                         
    PID_RX5_INIT,                                                                          
                                                                                           
	PID_DEMOD_INIT,                                                                        
                                                                                           
	PID_RESERVED_01,                                                                       
	PID_RESERVED_02,                                                                       
	PID_RESERVED_03,                                                                       
	PID_RESERVED_04,                                                                       
	PID_RESERVED_05,                                                                       
	PID_RESERVED_06,                                                                       
	PID_RESERVED_07,                                                                       
	PID_RESERVED_08,                                                                       
	PID_RESERVED_09,                                                                       
	PID_RESERVED_10,                                                                       
                                                                                           
    //--------------------------------------------		                                   
    // Message Event Tag                                                                   
    //--------------------------------------------	                                       
	MID_RX1_CONNECT,                                                                       
	MID_RX2_CONNECT,                                                                       
	MID_RX3_CONNECT,                                                                       
	MID_RX4_CONNECT,                                                                       
    MID_RX5_CONNECT,                                                                       
	                                                                                       
	MID_RX1_DISCONNECT,                                                                    
	MID_RX2_DISCONNECT,                                                                    
	MID_RX3_DISCONNECT,                                                                    
	MID_RX4_DISCONNECT,                                                                    
    MID_RX5_DISCONNECT,                                                                    
                                                                                           
                                                                                           
	//	- DEM (음성복조장치 제어 프로세스)                                                 
	MID_DEM_CONNECT,                                                                       
    	                                                                                   
	MID_RX_PARAMETERS_SET,              // 주파수,대역폭,변조형식,스켈치 동시 설정         
	MID_RX1_PARAMETERS_SET,             // 수신기#1 주파수,대역폭,변조형식,스켈치 동시 설정
	MID_RX2_PARAMETERS_SET,             // 수신기#2 주파수,대역폭,변조형식,스켈치 동시 설정
	MID_RX3_PARAMETERS_SET,             // 수신기#3 주파수,대역폭,변조형식,스켈치 동시 설정
	MID_RX4_PARAMETERS_SET,             // 수신기#4 주파수,대역폭,변조형식,스켈치 동시 설정
	MID_RX5_PARAMETERS_SET,             // 수신기#5 주파수,대역폭,변조형식,스켈치 동시 설정
                                                                                           
	MID_RX1_CURRENT_FREQ_REQ,           // 수신기#1 현재 주파수[AFC] 요청                  
	MID_RX2_CURRENT_FREQ_REQ,           // 수신기#2 현재 주파수[AFC] 요청                  
	MID_RX3_CURRENT_FREQ_REQ,           // 수신기#3 현재 주파수[AFC] 요청                  
	MID_RX4_CURRENT_FREQ_REQ,           // 수신기#4 현재 주파수[AFC] 요청                  
	MID_RX5_CURRENT_FREQ_REQ,           // 수신기#5 현재 주파수[AFC] 요청                  
                                                                                           
	MID_RX_REQUEST_LEVEL,               // 수신기 신호레벨 요청                            
	MID_RX_REQUEST_LEVEL_MA,            // 운용감시 직접측정: 신호레벨                     
	MID_RX_REQUEST_LEVEL_SP,            // SPURIOUS : OnReceiveMessage 이벤트用
	MID_RX_REQUEST_LEVEL_SH,            // HARMONIC : OnReceiveMessage 이벤트用
	MID_RX_REQUEST_LEVEL_ME,            // ME(10MHz이하) : OnReceiveMessage 이벤트用
	MID_RX_REQUEST_LEVEL_CAL,           // CALIBRATION : REQUEST LEVEL                     
	                                                                                       
	MID_RX_FSCAN_PARAM_SET,             // 대역스캔 파라미터 설정                          
	MID_RX_FSCAN_START,                                                                    
	MID_RX_FSCAN_STOP,                                                                     
    MID_RX_FSCAN_CONTINUE,              // 운용감시 FSCAN 재시작                           
                                                                                           
	MID_RX_MSCAN_PARAM_SET_STRAT,       // 채널스캔 파라메터 설정/스캔시작                 
	MID_RX_MSCAN_STOP,                                                                     
	MID_RX_CHANNEL_CLEAR,                                                                  
                                                                                           
	MID_RX_SWEEP_FREQ_SET,              // SWEEP시 주파수만 변경하고 스캔하는 경우         
	MID_RX_ATT_GAIN_SET,                // ATTENUATION / GAIN SETTING                      
                                                                                           
    MID_SYS_MON_CAL_SG_OFF,             // 측정용 시스템보정 신호발생기 OFF 
    MID_SYS_DF_REQ,                     // [전파품질 데몬 --> 방탐데몬] 전파품질측정과 동시에 방탐수행 요청 
    MID_SYS_DF_RES,                     // [방탐데몬 --> 전파품질 데몬] 동시 수행된 방탐 결과 수신           
	//--------------------------------------------	
	
    //--------------------------------------------                              
    // 방향탐지 관련                                                            
    //--------------------------------------------
    PID_MON_CONFIRM_DF,                 // 방향탐지 데몬 연결확인               
    PID_MON_CONNECT_DF,                 // 방향탐지 데몬 연결                   
    PID_MON_DISCONNECT_DF,              // 방향탐지 데몬 연결 종료              
    PID_MON_INIT_DF,                    // 방향탐지 데몬 초기화                 

	PID_MEASURE_PARAM_DF_ENV,           // 방탐 환경정보     [불필요]           
                                                                                
    PID_MEASURE_PARAM_DF_DT,                                                    
    PID_MEASURE_PARAM_DF_BW_S,                                                  
    PID_MEASURE_PARAM_DF_BW_D,                                                  
    PID_MEASURE_PARAM_DF_BW_E,                                                  
    PID_MEASURE_PARAM_DF_CH_S,                                                  
    PID_MEASURE_PARAM_DF_CH_D,                                                  
    PID_MEASURE_PARAM_DF_CH_E,                                                  
    PID_MEASURE_PARAM_ME_DF,	                                                
                                                                                
    PID_MEASURE_PARAM_DF_DT_R,                                                  
    PID_MEASURE_PARAM_DF_BW_S_R,
    PID_MEASURE_PARAM_DF_BW_D_R,                                                
    PID_MEASURE_PARAM_DF_BW_E_R,
    PID_MEASURE_PARAM_DF_CH_S_R,
    PID_MEASURE_PARAM_DF_CH_D_R,                                                
    PID_MEASURE_PARAM_DF_CH_E_R, 
                                                                    	        
    PID_MEASURE_PARAM_ME_DT_R,                                                  
                                                                                
    PID_MEASURE_START_DF,                                                       
    PID_MEASURE_STOP_DF,                                                        
    PID_MEASURE_COMPLETE_DF,                                                    
    PID_MEASURE_FFT_DF_INIT_ENV,                                                
    PID_MEASURE_FFT_DF_CURRENT,                                                 
    PID_MEASURE_FFT_CL_CURRENT,                                                 
    PID_MEASURE_RESULT_DF,                                                      
    PID_MEASURE_CAL_FULL,                                                       
    PID_MEASURE_CAL_FULL_R,                                                     
    PID_MEASURE_CAL_COMPLETE_FULL,                                              
    PID_MEASURE_CAL_TABLE,                                                      
    PID_MEASURE_CAL_TABLE_R,                                                    
    PID_MEASURE_CAL_COMPLETE_TABLE,                                             
    PID_MEASURE_START_RC,                                                       
    PID_MEASURE_STOP_RC,                                                        
    PID_MEASURE_COMPLETE_RC,                                                    
    PID_MEASURE_RESULT_RC,                                                      
    PID_MEASURE_SAVE_CAL_DATA,     
	
	PID_MEASURE_MR_DF_Q,				// 전파품질측정 단일모드 방탐 요청
	PID_MEASURE_MR_DF_R,				// 전파품질측정 단일모드 방탐 결과
	PID_MEASURE_MR_DF_DENY,				// 전파품질측정 단일모드 방탐 불가
                                                                                
    PID_MEASURE_RESULT_DF_GPS,                                                  
    PID_FFT_RECEIVE,                    // [OP]	                                
    PID_SPEED_TEST,                     // 네트웍 속도 체크 [OP]                
    PID_MESSAGE_DATA,                   // [OP]                                 
                                                                                
    PID_SIGNAL_SIMULATE_START,                                                  
    PID_SIGNAL_SIMULATE_STOP,                                                   
    PID_SIGNAL_SIMULATE_RESULT,                                                 
    PID_SIGNAL_SIMULATE_SET,                                                    
    PID_SIGNAL_SIMULATE_RETURN,                                                 
                                                                                
    PID_EVALUATE_RX_SIGNAL_START,                                               
    PID_EVALUATE_RX_SIGNAL_STOP,                                                
    PID_EVALUATE_RX_SIGNAL_RESULT,                                              
	PID_EVALUATE_RX_SIGNAL_COMPLETE,                                            
                                                                                
    PID_CHANNEL_MISMATCH_START,                                                 
    PID_CHANNEL_MISMATCH_STOP,                                                  
    PID_CHANNEL_MISMATCH_RESULT_Q,                                              
    PID_CHANNEL_MISMATCH_RESULT_R,                                              
    PID_CHANNEL_MISMATCH_COMPLETE,                                              
    PID_CHANNEL_MISMATCH_SAVE,   
	
	//------------------------------------------
	// RF 경로 점검 관련
	//------------------------------------------
	PID_DEVICE_STATUS_RFPATH_Q,			// RF 경로점검 요청
	PID_DEVICE_STATUS_RFPATH_R,			// RF 경로점검 결과
  
	//------------------------------------------
	// RF 경로 보정 관련
	//------------------------------------------
	PID_MEASURE_RFPATH_CAL_Q,			// RF 경로보정 요청
	PID_MEASURE_RFPATH_CAL_R,			// RF 경로보정 결과

	//------------------------------------------
	// SG 자체점검
	//------------------------------------------
	PID_DEVICE_STATUS_SG_Q,				// 신호발생기 자기진단 요청             
	PID_DEVICE_STATUS_SG_R,				// 신호발생기 자기진단 응답             
	PID_DEVICE_DETAIL_STATUS_SG_Q,		// 신호발생기 상세진단 요청             
	PID_DEVICE_DETAIL_STATUS_SG_R,		// 신호발생기 상세진단 응답             

	//------------------------------------------
	// 방사보정		/// HAPPYIM [2010/6/11] 방탐(넥스원방식) 적용
	//------------------------------------------
	PID_MEASURE_PARAM_RADIATION_CAL_Q,                                                                    
	PID_MEASURE_PARAM_RADIATION_CAL_R,
	PID_MEASURE_PARAM_CHANNEL_LOSS_Q,
	PID_MEASURE_PARAM_CHANNEL_LOSS_R,

	//--------------------------------------------                              
    // 음성복조 및 저장장치 제어 관련                                           
    //--------------------------------------------                              
    PID_VOICE_FILE_SRV_CONNECT       = 1000,                                    
    PID_VOICE_FILE_SRV_CONNECT_R,                                               
    PID_VOIPGW_CONNECT,                                                         
    PID_VOIPGW_CONNECT_R,                                                       
    PID_VOICE_FILE_SRV_DISCONNECT,                                              
    PID_VOIPGW_DISCONNECT,                                                      
    PID_VOICE_FILE_SRV_INIT_PORT_IP,                                            
    PID_VOIPGW_INIT_PORT_IP,                                                    
    PID_DEMOD_REC_STRAT,                                                        
    PID_DEMOD_REC_STOP,                                                         
    PID_DEMOD_STREAM_STRAT,                                                     
    PID_DEMOD_STREAM_STOP,                                                      
    PID_DEMOD_STREAM_TRANCELATE,                                                
    PID_DEMOD_REC_COMPLETE_MR,                                                  
    PID_DEMOD_REC_COMPLETE_MI,                                                  

    //--------------------------------------------                              
    // 연계관련	(이동종감 <--> 지휘통제)                                        
    //--------------------------------------------     

	PID_CONN_MRMS_CCS_CONNECT         = 2049,   // CCS, RMS 소켓 연결/종료

    //--------------------------------------------
	// 일괄제어(Batch Measurement Request) CCS -> MRMS
	//--------------------------------------------
	PID_CONN_CCS_MRMS_BATCH_MRDC_REQ  = 2100,   // 전파품질측정 요청                                
	PID_CONN_CCS_MRMS_BATCH_MRAT_REQ,			// 전파품질-신호각 측정 요청                        

    PID_CONN_CCS_MRMS_BATCH_MIBW_REQ,           // 불법전파탐사 요청                                
    PID_CONN_CCS_MRMS_BATCH_MOBW_REQ,           // 점유율 대역 요청                                 
	PID_CONN_CCS_MRMS_BATCH_MEDT_REQ,           // 전파스펙트럼측정 요청                            
    PID_CONN_CCS_MRMS_BATCH_MPHC_REQ,           // 스퓨리어스--고조파 요청                          
    PID_CONN_CCS_MRMS_BATCH_MPSP_REQ,           // 스퓨리어스--불요파 요청                          
	PID_CONN_CCS_MRMS_BATCH_STOP_REQ,			// 일괄제어 중지 요청                               
    //--------------------------------------------
	// 일괄제어(Batch Measurement Response) MRMS -> CCS
	//--------------------------------------------
	PID_CONN_MRMS_CCS_BATCH_END,				// 일괄제어 완료 전송                               
	PID_CONN_MRMS_CCS_BATCH_ERR,                // 일괄제어 에러 전송                               
    //--------------------------------------------
	// 상세운용 현황 & 스펙트럼 MRMS -> CCS 전송
	//--------------------------------------------
	PID_CONN_MRMS_CCS_OPRTDSPCT_MRDC  = 2150,	// 전파품질측정 상세운용 현황 정보 스펙트럼         
	PID_CONN_MRMS_CCS_OPRTDSPCT_MRCH,			// 전파품질측정 상세운용 현황 정보 스펙트럼         
 	PID_CONN_MRMS_CCS_OPRTDSPCT_MIBW,           // 불법전파탐사 상세운용 현황 정보 스펙트럼         
	PID_CONN_MRMS_CCS_OPRTDSPCT_MICH,           // 불법전파탐사 상세운용 현황 정보 스펙트럼         
	PID_CONN_MRMS_CCS_OPRTDSPCT_MOBW,           // 점유율측정 상세운용 현황 정보 스펙트럼           
	PID_CONN_MRMS_CCS_OPRTDSPCT_MOCH,           // 점유율측정 상세운용 현황 정보 스펙트럼           
	PID_CONN_MRMS_CCS_OPRTDSPCT_MEDT,			// 고속스펙트럼 상세운용 현황 정보 스펙트럼         
	PID_CONN_MRMS_CCS_OPRTDSPCT_MPHC,           // 스퓨리어스측정-고조파 상세운용 현황 정보 스펙트럼
	PID_CONN_MRMS_CCS_OPRTDSPCT_MPSP,			// 스퓨리어스측정-불요파 상세운용 현황 정보 스펙트럼
	//--------------------------------------------
	// 운영현황 상세제어시 측정환경을 MRMS -> CCS 전송
	//--------------------------------------------
	PID_CONN_MRMS_CCS_MEASENV_MRDC,				// 전파품질측정 직접 환경       
	PID_CONN_MRMS_CCS_MEASENV_MRCH,				// 전파품질측정 채널 환경       
	PID_CONN_MRMS_CCS_MEASENV_MIBW,             // 불법전파탐사 대역 환경       
	PID_CONN_MRMS_CCS_MEASENV_MICH,             // 불법전파탐사 채널 환경       
	PID_CONN_MRMS_CCS_MEASENV_MOBW,             // 점유율측정 대역 환경         
	PID_CONN_MRMS_CCS_MEASENV_MOCH,             // 점유율측정 채널 환경         
	PID_CONN_MRMS_CCS_MEASENV_MEDT,				// 전파스펙트럼 환경            
	PID_CONN_MRMS_CCS_MEASENV_MPHC,             // 스퓨리어스측정-고조파 환경   
	PID_CONN_MRMS_CCS_MEASENV_MPSP,				// 스퓨리어스측정-불요파 환경   
	//--------------------------------------------
	// 운영현황상세와 측정결과를 MRMS ->CCS 로 전송
	//--------------------------------------------
	PID_CONN_MRMS_CCS_MEASRET_MRDC,				// 전파품질-직접 측정 결과      
	PID_CONN_MRMS_CCS_MEASRET_MRCH,				// 전파품질-채널 측정 결과      
	PID_CONN_MRMS_CCS_MEASRET_MRAT,				// 전파품질-신호각 측정 결과    
	PID_CONN_MRMS_CCS_MEASRET_MIBW,             // 불법전파탐사 결과            
	PID_CONN_MRMS_CCS_MEASRET_MICH,             // 불법전파탐사 결과            
	PID_CONN_MRMS_CCS_MEASRET_MOBW,             // 점유율대역 결과              
	PID_CONN_MRMS_CCS_MEASRET_MOCH,             // 점유율대역 결과              
	PID_CONN_MRMS_CCS_MEASRET_MEDT,             // 전파스펙트럼 결과            
	PID_CONN_MRMS_CCS_MEASRET_MPHC,             // 스퓨리어스-고조파 결과       
	PID_CONN_MRMS_CCS_MEASRET_MPSP,             // 스퓨리어스-불요파 결과       
	//------------------------------
	PID_CONN_CCS_MRMS_OPRTD_REQ       = 2200,   // 상세운용 현황정보 요청        

    //--------------------------------------------                                              
    // 지능형, 고정방탐 연계 관련                                                               
    //--------------------------------------------                                              
    PID_CONN_FDF_TO_RMS_AZI_REQ       = 3020,   // * RMS 최대수신방위각측정 요청                
    PID_CONN_RMS_TO_FDF_AZI_RESULT,             // * RMS 최대수신방위각측정 결과                
    PID_CONN_FDF_TO_RMS_AZI_STOP,               // * RMS 최대수신방위각측정 중지                
    PID_CONN_RMS_TO_FDF_AZI_ERR,                // * RMS 최대수신방위각측정 에러                
                                               	//   지능형에서 측정중일 경우 요청처리          
                                             	//   불가함을 고정방탐으로 통보                 
    PID_CONN_RMS_TO_FDF_STATUS_REQ    = 3040,   // * FDF 상태정보 요청				            
    PID_CONN_FDF_TO_RMS_STATUS_RES,             // * FDF 상태정보 확인				            
    PID_CONN_RMS_TO_FDF_DF_REQ        = 3050,	// * FDF 방탐처리 요청				            
    PID_CONN_RMS_TO_FDF_DF_STOP,                // * FDF 방탐처리 중지				            
    PID_CONN_FDF_TO_RMS_DF_RESULT     = 3060,	// * FDF 방탐처리 결과				            
    PID_CONN_RMS_TO_FDF_REQ_REPORT    = 3070,   // * FDF 처리결과 수신                          
                                                //   [Success/Error/Why?...]	                
    //--------------------------------------------                                              
    // 지능형, 이동방탐 연계관련	                                                            
    //--------------------------------------------                                              
    PID_CONN_MDF_LOGIN_GET	          = 4000,	// 이동방탐 사용자 로그인                       
                                                // (CDMA 연결후 통보, DB Update / Insert)       
    PID_CONN_MDF_LOGOUT_GET,  		    		// 이동방탐 사용자 로그아웃: MDF -> MBS         
    PID_CONN_CCS_TO_MDF_MSG_GET	      = 4010,	// 지휘통제에서 메시지 전송: CCS -> MBS         
    PID_CONN_CCS_TO_MDF_MSG_PUT,	    		// 지휘통제에서 메시지 전송: MBS -> MDF         
    PID_CONN_MDF_TO_CCS_MSG_GET,	    		// 이동방탐에서 메시지 전송: MDF -> MBS         
    PID_CONN_MDF_TO_CCS_MSG_PUT,    			// 이동방탐에서 메시지 전송: MBS -> CCS         
    PID_CONN_RMS_TO_MDF_MSG_GET	      = 4110,	// 감시응용에서 메시지 전송: RMS -> MBS         
    PID_CONN_RMS_TO_MDF_MSG_PUT,	   			// 감시응용에서 메시지 전송: MBS -> MDF         
    PID_CONN_MDF_TO_RMS_MSG_GET,	   			// 이동방탐에서 메시지 전송: MDF -> MBS         
    PID_CONN_MDF_TO_RMS_MSG_PUT,    			// 이동방탐에서 메시지 전송: MBS -> RMS         
    PID_CONN_MDF_TO_MDF_MSG_GET	      = 4410,	// 이동방탐에서 메시지 전송: MDF -> MBS         
    PID_CONN_MDF_TO_MDF_MSG_PUT,				// 이동방탐에서 메시지 전송: MBS -> MDF         
    PID_CONN_CCS_TO_MDF_DF_REQ_GET	  = 4050,	// 지휘통제에서 방탐요청: CCS -> MBS            
    PID_CONN_CCS_TO_MDF_DF_REQ_PUT,		    	// 지휘통제에서 방탐요청: MBS -> MDF            
    PID_CONN_MDF_TO_CCS_DF_RESULT_GET = 4060,	// 지휘통제로 방탐결과 전송: MDF -> MBS         
    PID_CONN_MDF_TO_CCS_DF_RESULT_PUT,			// 지휘통제로 방탐결과 전송: MBS -> CCS         
    PID_CONN_RMS_TO_MDF_DF_REQ_GET	  = 4150,	// 감시응용에서 방탐요청: RMS -> MBS            
    PID_CONN_RMS_TO_MDF_DF_REQ_PUT,				// 감시응용에서 방탐요청: MBS -> MDF            
    PID_CONN_RMS_TO_MDF_DF_RESULT_GET = 4160,	// 감시응용으로 방탐결과 전송: MDF -> MBS       
    PID_CONN_RMS_TO_MDF_DF_RESULT_PUT,			// 감시응용으로 방탐결과 전송: MBS -> RMS       
    PID_CONN_MDF_TO_MDF_DF_RESULT_GET = 4460,	// 타 이동방탐으로 방탐결과 전송: MDF -> MBS    
    PID_CONN_MDF_TO_MDF_DF_RESULT_PUT,			// 타 이동방탐으로 방탐결과 전송: MBS -> MDF    
    PID_CONN_CCS_TO_MDF_MOVE_REQ_GET  = 4030,	// 지휘통제에서 이동 요청 [수신]: CCS -> MBS    
    PID_CONN_CCS_TO_MDF_MOVE_REQ_PUT,			// 지휘통제에서 이동 요청 [통보]: MBS -> MDF    
    PID_CONN_RMS_TO_MDF_MOVE_REQ_GET  = 4130,	// 감시응용에서 이동 요청 [수신]: RMS -> MBS    
    PID_CONN_RMS_TO_MDF_MOVE_REQ_PUT,			// 지휘통제에서 이동 요청 [통보]: MBS -> MDF    
    PID_CONN_MDF_LICENSE_REQ_GET      = 4560,	// 이동방탐 허가DB조회 요청 : MDF -> MBS        
    PID_CONN_MDF_LICENSE_RESULT_PUT,			// 이동방탐 허가DB조회 결과 : MBS -> MDF        
    PID_CONN_MDF_MBS_CONTINUE,					// MDF와 MBS 의 연결유지                        
    PID_CONN_CCS_ALL_ID_SEND,					// 지휘통제에서 자신의 ID 정보 전송             
                                                                                                
    PID_FILE_DELETE_VOIPGW_S          = 9001,   // 파일명                                       
    PID_FILE_DELETE_VOIPGW_R,                   // 구조체                                       
    PID_FILE_DELETE_STATUS_VOIPGW_Q,            // 파일명                                       
    PID_FILE_DELETE_STATUS_VOIPGW_R,            // 구조체                                       
    PID_EXIT_VOIPGW_S,
	
	PID_SET_SG_ON					  = 10001,	// 신호발생기: Output On
	PID_SET_SG_OFF,								// 신호발생기: Output Off
	PID_SET_SG_FREQ,							// 신호발생기: Freq Set (Hz)
	PID_SET_SG_LEVEL,							// 신호발생기: Level Set ( dBm)
	
	PID_BIG_TRANSFER					= 20001,
	
	PID_MEASURE_SIGNAL_LEVEL			= 20002,	// 신호레벨 조회
	PID_MEASURE_SIGNAL_LEVEL_R,
	
	PID_MEASURE_IQ_DF_RESULT_R			= 20004,	// 교정중 방탐 결과 요청시 응답 PID
	
	PID_MEASURE_CAL_DATA				= 20005,
	PID_MEASURE_CAL_DATA_R,

	emRmsUDP_ServicePortInit_S_TAG		= 29821,
	emRmsUDP_ServicePortInit_R_TAG,

	PID_MAKING_MI_CHANNEL				= 30000,

	emRmsPacketFFT_InitEnv_R_TAG		= 30200,	// OP에서 Env 수신 여부

	PID_PACKET_END           
	
} enumPacketTag;

#endif // DEF_ENUM_PACKET_TAG


#endif	// END #ifndef __DEF_SOCKET__
