/**
* @file			LIGCondition.h
* @date			2010/10/22
* @author		Jang, MinKi(Greg), Revised by Lee, JeongNam
* @brief			Thread
* @remarks		
*/

#ifndef LIGThread_H
#define LIGThread_H


#include "LIGLock.h"
#include "LIGCondition.h"


//warning: 'struct ILIGThread' has virtual functions but non-virtual destructor
struct ILIGThread
{
	virtual void Run() = 0;
};


/**
 * @class    LIGThread
 * @brief   쓰레드 기능을 상속받아 사용할 수 있게 Wrapping 한클래스
 *
 * (1) 클래스 설명
 * - 쓰레드 기능을 상속받아 사용할 수 있게 Wrapping 한클래스
 *
 * (2) 설계결정사항
 * - 해당사항없음
 *
 * (3) 제한 및 예외처리
 * - 해당사항없음
 */
class LIGThread : public ILIGThread
{
	//!< Thread Status 정의
	enum THREAD_STATUS { THREAD_STOP = 0, THREAD_RUNNING }; //, THREAD_STOPPING };

public:
	/**
	* @brief    생성자 함수
	*/
	LIGThread();
	/**
	* @brief    소멸자 함수
	*/
	virtual ~LIGThread();

public:
	//LIGThread(ILIGThread *ptr);
	//bool Attach(ILIGThread *ptr);

	/**
	* @brief    외부 함수 형태 정의
	*/
	typedef void (*RUN_ROUTINE)();

	/**
	* @brief     외부 함수 형태 정의 (파라메타 있는 경우)
	* @param     parameter		전달 파라메타
	*/
	typedef void (*RUN_ROUTINE_WITH_PARAMETER)(void* parameter);

	//외부 함수 호출 관련
//	LIGThread(RUN_ROUTINE runRoutine);
	//bool Attach(RUN_ROUTINE runRoutine);

//	LIGThread(RUN_ROUTINE_WITH_PARAMETER runRoutine, void* parameter);
//	bool Attach(RUN_ROUTINE_WITH_PARAMETER runRoutine, void* parameter);

public:
	/**
	* @brief		Thread 시작
	* @return		true : STOP -> RUNNING, RUNNING -> RUNNING 인경우, false : Thread생성에 실패했거나 STOPPING상태임					
	*/
	bool Start();

	/**
	* @brief		Thread 중지
	* @return		true : RUNNING -> STOPPING, STOPPING -> STOPPING 인경우, 	false : STOP상태임
	*/
	bool Stop();

	/**
	* @brief		Thread 종료시까지 대기(SYNC) 함수, STOP상태일 경우 : 바로return, STOPPING상태일 경우 : 쓰레드가 종료될 때까지 무한대기 후 -> STOP
	* @param     lTimeOut		타임 아웃 전달 파라메타
	*/
	//void Join_withMsgWait(const DWORD lTimeOut = INFINITE);

	/**
	* @brief		Thread 종료시까지 대기하는 함수
	*/
	//void Join();		///< LIGThread의 concrete class의 destructor에서 Join()을 호출하면 안됨.
	
	/**
	* @brief		Thread 우선순위 설정 함수
	* @param     priority		쓰레드 우선순위 정의 변수
	*/
	BOOL SetPriority(int priority = THREAD_PRIORITY_NORMAL);

	/**
	* @brief		Thread가 실행 중인지 확인하는 함수
	* @return		true : Thread 실행 중, false : Thread 실행 안함
	*/
	bool IsRunning();	///< Thread가 실행 중인지 확인
	
	/**
	* @brief		Thread가 중지되어 있는지 확인하는 함수
	* @return		true : Thread 실행 중, false : Thread 실행 안함
	*/
	//bool IsStop();

	/**
	* @brief		Thread 핸들을 얻어오기 위한 함수
	* @return		Thread 핸들 값
	*/
	//HANDLE GetThreadHandle();

	/**
	* @brief		Thread ID를 얻어오기 위한 함수
	* @return		Thread ID 값
	*/
	//unsigned int GetThreadID();

protected:
	/**
	* @brief		외부 인터페이스 class를 호출할 경우 사용
	*/
	virtual void Run();

private:
	inline void Init(); //각종변수들 초기화

public:
	///< dwMilliseconds 동안 Sleep
	//static void LIGSleep(int dwMilliseconds);
	///< 같은 프로세스 내의 같은 우선순위 이상을 갖는 스레드가 대기중일 경우 무조건 양보
	//static void YieldThread();
	///< 같은 프로세스 내의 아무 스레드에게 무조건 양보. 약 15ms 정도 자신의 thread를 suspend 시킨다. 2014.05.27. 이정남
	//static void LIGSuspendThread(); 

private:
	static unsigned __stdcall MainProc(void *parameter);

private:
	volatile LIGThread::THREAD_STATUS m_threadStatus; //!< Thread Status 저장
	ILIGThread *m_ptrRun; //!< 외부 인터페이스 호출
	RUN_ROUTINE m_pRunFunc; //!< 외부 함수 호출
	RUN_ROUTINE_WITH_PARAMETER m_pRunFuncWithParameter; //!< 외부 함수 호출
	void* m_parameter; //m_pRunFuncWithParameter용

	HANDLE m_hThread; //!< Thread Handle
	unsigned int m_threadID; //!< Thread ID

	LIGLock m_threadStatusLock;
};

/**
 * [식별자 : CLS-GMU-EL-?-????]
 * [추적성 : SRS-G-SOFR-???]
 * 
 * @class     CGRThread
 * @brief     
 * 
 * (1)　클래스 설명
 *  - 쓰레드 클래스로서 호출 또는 멈춤 기능을 수행한다.
 * 
 * (2)　설계결정사항
 *  - 설계결정사항을 기입해주세요.
 * 
 * (3)　제한 및 예외처리
 *  - 신호 분석, 분석 도구, CED 불러오기 등 많은 시간이 소요되는 기능을 수행한다.
 * 
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-03-29, 오후 2:15 
 * @warning   
 */
class CGRThread
{
protected:
	HANDLE m_hThread;
	HANDLE m_hEvent; 
	DWORD m_nThreadID;

	int m_iCommand;

private:
	static DWORD WINAPI StaticThreadStart( LPVOID lpParam );

protected :
	virtual DWORD RunThread(void)=0;

public:
	CGRThread(void);
	~CGRThread(void);


	bool StartThread();
	//void StopThread();

	//inline HANDLE GetThreadHandle() { return m_hThread; }
	//inline DWORD GetThreadID() { return m_nThreadID; }

	int GetThreadCommand();

	inline void ClearThreadCommand() { m_iCommand=0; }
	inline int GetCommand() { return m_iCommand; }

	//void SetThreadCommand( int iCommand );

	bool IsRunning();
	//void Join();
	//	bool Yield();
	//void GRThreadSleep( DWORD dwMilliseconds );
	DWORD Suspend();
	DWORD Resume();

};

#endif 
