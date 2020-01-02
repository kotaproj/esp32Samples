/**
 * @file ApiSw.c
 * @brief スイッチ管理
 */

#include "ApiSw.h"
// #include "CtlTaskSw.h"

/// @cond
#define _API_SW_C_
/// @endcond

/* DECLARATIONS ***************************************************************/
#define DEBUG_API_SW			(0)			//0:release, 1:debug
#define SW_CHATTERING_TIME		(30)		//チャダリング除去時間:30msec

/* VARIABLES ******************************************************************/
static SwCode_t s_xBakSwCode;

static TimerHandle_t s_xTimers[] = {
	#if USE_SW_NO1
	NULL,
	#endif
	#if USE_SW_NO2
	NULL,
	#endif
	#if USE_SW_NO3
	NULL,
	#endif
	#if USE_SW_NO4
	NULL,
	#endif
	#if USE_SW_NO5
	NULL,
	#endif
};

/* PRIVATE PROTOTYPES *********************************************************/
static SwCode_t xReadApiSw( void);

static void isrSwCode(SwCode_t xSwCode);
static void IRAM_ATTR isrSwCodeNo1();
static void IRAM_ATTR isrSwCodeNo2();
static void IRAM_ATTR isrSwCodeNo3();
static void IRAM_ATTR isrSwCodeNo4();
static void IRAM_ATTR isrSwCodeNo5();

static void vTimerCallback(SwCode_t xCmp);
static void vTimerCallbackNo1( TimerHandle_t xTimer);
static void vTimerCallbackNo2( TimerHandle_t xTimer);
static void vTimerCallbackNo3( TimerHandle_t xTimer);
static void vTimerCallbackNo4( TimerHandle_t xTimer);
static void vTimerCallbackNo5( TimerHandle_t xTimer);

static void vSendSw(SwCode_t xCode, SwType_t xType);
static int32_t ulSearchSwGpioAsgn(SwCode_t xCode);

/* TABLES ***************************************************************/
typedef struct
{
	SwCode_t xSwCode;
	uint8_t ucPin;
	int input;	//INPUT, INPUT_PULLUP, INPUT_PULLDOWN
	int mode;	//LOW, CHANGE, RISING,FALLING
	void (*vIrqCallback)();
	void (*vTimerCallback)(TimerHandle_t);
}SwGpioAsgn_t;

const SwGpioAsgn_t c_xaSwGpioAsgn_t[] = {
	#if USE_SW_NO1
	{
		SW_CODE_NO1,
		SW_GPIO_ASGN_NO1,
		INPUT_PULLUP,
		FALLING,
		isrSwCodeNo1,
		vTimerCallbackNo1
	},
	#endif	//USE_SW_NO1
	#if USE_SW_NO2
	{
		SW_CODE_NO2,
		SW_GPIO_ASGN_NO2,
		INPUT_PULLUP,
		FALLING,
		isrSwCodeNo2,
		vTimerCallbackNo2
	},
	#endif	//USE_SW_NO2
	#if USE_SW_NO3
	{
		SW_CODE_NO3,
		SW_GPIO_ASGN_NO3,
		INPUT_PULLUP,
		FALLING,
		isrSwCodeNo3,
		vTimerCallbackNo3
	},
	#endif	//USE_SW_NO3
	#if USE_SW_NO4
	{
		SW_CODE_NO4,
		SW_GPIO_ASGN_NO4,
		INPUT_PULLUP,
		FALLING,
		isrSwCodeNo4,
		vTimerCallbackNo4
	},
	#endif	//USE_SW_NO4
	#if USE_SW_NO5
	{
		SW_CODE_NO5,
		SW_GPIO_ASGN_NO5,
		INPUT_PULLUP,
		FALLING,
		isrSwCodeNo5,
		vTimerCallbackNo5
	},
	#endif	//USE_SW_NO5
};

/**
 * @brief スイッチ処理初期化関数
 */
ErType_t xInitSw(void)
{
	Serial.printf("%s : run\n",__func__);

	// 管理データの初期化
	s_xBakSwCode = SW_CODE_NON;

	ErType_t xErType = ER_OK;
	//               [0],[1],[2],[3],[4],[5],[6],[7],
	char cName[8] = {'T','m','r','S','w','0',0x00,0x00};

	// GPIO PIN設定
	for(us usCnt=0; usCnt<sizeof(c_xaSwGpioAsgn_t)/sizeof(SwGpioAsgn_t); usCnt++)
	{
		// GPIO Input Mode、プルアップで設定
		// ->MPU内部のプルアップ抵抗を使用する
		pinMode(c_xaSwGpioAsgn_t[usCnt].ucPin, c_xaSwGpioAsgn_t[usCnt].input);

		// 割り込み用のコールバック関数を登録する
		attachInterrupt(digitalPinToInterrupt(c_xaSwGpioAsgn_t[usCnt].ucPin),
						c_xaSwGpioAsgn_t[usCnt].vIrqCallback,
						c_xaSwGpioAsgn_t[usCnt].mode);

		// タイマーの作成
		cName[5] = '1' + usCnt;
		s_xTimers[usCnt] = xTimerCreate
					(	cName,						//text name
						SW_CHATTERING_TIME,		//timer period
						pdFALSE,					//auto-reload
						( void * ) 0,				//number of times
						c_xaSwGpioAsgn_t[usCnt].vTimerCallback	//callback
					);
		if( NULL == s_xTimers[usCnt] )
		{
			Serial.printf("xInitSw : error - xTimerCreate %d\n",usCnt);
			return ER_FAIL;
		}
		Serial.printf("xInitSw : reg %d\n",usCnt);
	}

	Serial.printf("%s : over\n",__func__);
	return xErType;
}

/**
 * @brief スイッチのタイマーコールバック関数
 */
static void vTimerCallback(SwCode_t xCmp)
{
	SwCode_t xCode = xReadApiSw();
	if( xCmp == xCode && xCode == s_xBakSwCode)
	{
		vSendSw(xCode, SW_TYPE_DOWN);
	}
	s_xBakSwCode = SW_CODE_NON;

	uint32_t ulRef = ulSearchSwGpioAsgn(xCmp);

	// 割り込み用のコールバック関数を登録する
	attachInterrupt(digitalPinToInterrupt(c_xaSwGpioAsgn_t[ulRef].ucPin),
					c_xaSwGpioAsgn_t[ulRef].vIrqCallback,
					c_xaSwGpioAsgn_t[ulRef].mode);

	return;
}

/**
 * @brief スイッチNo1のタイマーコールバック関数
 */
static void vTimerCallbackNo1( TimerHandle_t xTimer)
{
	vTimerCallback(SW_CODE_NO1);
	xTimerStop( xTimer, 0);
}

/**
 * @brief スイッチNo2のタイマーコールバック関数
 */
static void vTimerCallbackNo2( TimerHandle_t xTimer)
{
	vTimerCallback(SW_CODE_NO2);
	xTimerStop( xTimer, 0);
}

/**
 * @brief スイッチNo3のタイマーコールバック関数
 */
static void vTimerCallbackNo3( TimerHandle_t xTimer)
{
	vTimerCallback(SW_CODE_NO3);
	xTimerStop( xTimer, 0);
}

/**
 * @brief スイッチNo4のタイマーコールバック関数
 */
static void vTimerCallbackNo4( TimerHandle_t xTimer)
{
	vTimerCallback(SW_CODE_NO4);
	xTimerStop( xTimer, 0);
}

/**
 * @brief スイッチNo5のタイマーコールバック関数
 */
static void vTimerCallbackNo5( TimerHandle_t xTimer)
{
	vTimerCallback(SW_CODE_NO5);
	xTimerStop( xTimer, 0);
}

/**
 * @brief 管理タスクへスイッチイベントを通知
 */
static void vSendSw(SwCode_t xCode, SwType_t xType)
{
	static int32_t s_count = 0;
	s_count++;
	Serial.printf("%s : run\n",__func__);
	Serial.printf("%s : xCode:%d, xType:%d, count:%d\n",__func__ ,xCode, xType ,s_count);

	// static SwMessage_t s_xMessage;

	// s_xMessage.ulCode = ulCode;
	// s_xMessage.ulType = ulType;

	// vCallbackCtlMsgSw(UID_TRG_SW, UID_CTL, &s_xMessage);
}

/**
 * @brief スイッチ状態の取得
 */
static SwCode_t xReadApiSw( void)
{
	int sw_sts;

	// GPIO PIN設定
	for(us usCnt=0; usCnt<sizeof(c_xaSwGpioAsgn_t)/sizeof(SwGpioAsgn_t); usCnt++)
	{
		sw_sts = digitalRead(c_xaSwGpioAsgn_t[usCnt].ucPin);
		if( SW_STS_PRESSED == sw_sts )
		{
			return c_xaSwGpioAsgn_t[usCnt].xSwCode;
		}
	}

	return SW_CODE_NON;
}

/**
 * @brief スイッチ割り込み処理
 */
static void isrSwCode(SwCode_t xSwCode)
{
	// volatile int sw_sts;
	volatile uint32_t ulRef;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	ulRef = ulSearchSwGpioAsgn(xSwCode);
	
	// 割り込み時のスイッチ状態
	s_xBakSwCode = xReadApiSw();
	xTimerStartFromISR(s_xTimers[ulRef], &xHigherPriorityTaskWoken);

	// 割り込みデタッチ
	detachInterrupt(digitalPinToInterrupt(c_xaSwGpioAsgn_t[ulRef].ucPin));

	return;
}

/**
 * @brief スイッチ割り込み処理 - NO1
 */
static void IRAM_ATTR isrSwCodeNo1()
{
	isrSwCode(SW_CODE_NO1);
}

/**
 * @brief スイッチ割り込み処理 - NO2
 */
static void IRAM_ATTR isrSwCodeNo2()
{
	isrSwCode(SW_CODE_NO2);
}

/**
 * @brief スイッチ割り込み処理 - NO3
 */
static void IRAM_ATTR isrSwCodeNo3()
{
	isrSwCode(SW_CODE_NO3);
}

/**
 * @brief スイッチ割り込み処理 - NO4
 */
static void IRAM_ATTR isrSwCodeNo4()
{
	isrSwCode(SW_CODE_NO4);
}

/**
 * @brief スイッチ割り込み処理 - NO5
 */
static void IRAM_ATTR isrSwCodeNo5()
{
	isrSwCode(SW_CODE_NO5);
}

/**
 * @brief スイッチ管理配列の参照位置の取得
 */
static int32_t ulSearchSwGpioAsgn(SwCode_t xCode)
{
	for(int32_t ulCnt=0; ulCnt<( sizeof(c_xaSwGpioAsgn_t)/sizeof(SwGpioAsgn_t) ); ulCnt++ )
	{
		if( c_xaSwGpioAsgn_t[ulCnt].xSwCode == xCode )
		{
			return ulCnt;
		}
	}

	return 0;
}

#undef _API_SW_C_
