#ifndef __EXTERNALICDCOMMON_H__
#define __EXTERNALICDCOMMON_H__

#pragma pack(1)
#if defined(BIG_ENDIAN)
	typedef union
	{
		UINT				uiTData;
		struct
		{
			UINT		iYear				: 6	;	// 년 (예: 2013년 -> 13)
			UINT		iMonth				: 4	;	// 월
			UINT		iDayOfMonth			: 5	;	// 일
			UINT		iHour				: 5	;	// 시 (24시 기준, 예:오후 1시-> 13)
			UINT		iMinute				: 6	;	// 분
			UINT		iSecond				: 6	;	// 초
		}BIT_FIELD;	
	}unTime;	
#else
	typedef union
	{
		UINT				uiTData;
		struct
		{
			UINT		iSecond				: 6	;	// 초
			UINT		iMinute				: 6	;	// 분
			UINT		iHour				: 5	;	// 시 (24시 기준, 예:오후 1시-> 13)
			UINT		iDayOfMonth			: 5	;	// 일
			UINT		iMonth				: 4	;	// 월
			UINT		iYear				: 6	;	// 년 (예: 2013년 -> 13)
		}BIT_FIELD;	
	}unTime;	
#endif	//	#if defined(BIG_ENDIAN)

	
#pragma pack()

#endif // #ifndef __EXTERNALICDCOMMON_H__
