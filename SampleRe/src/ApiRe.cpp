/**
 * @file ApiRe.c
 * @brief ロータリーエンコーダ管理
 */

#include "ApiRe.h"
#include <RotaryEncoder.h>
// #include "CtlTaskRe.h"

/// @cond
#define _API_RE_C_
/// @endcond

/* DECLARATIONS ***************************************************************/
#define DEBUG_API_RE			(0)			//0:release, 1:debug
#define RE_CHECK_TIME			(50)		//ロータリーエンコーダチェック間隔:50msec

/* VARIABLES ******************************************************************/
static RotaryEncoder s_xRotaryEncoder(RE_ASGN_SIGNAL_A, RE_ASGN_SIGNAL_B);
static int32_t s_slBakRePosition;
static TimerHandle_t s_xTimerRe = NULL;

/* PRIVATE PROTOTYPES *********************************************************/
static void IRAM_ATTR vIsrCallbackRe();
static void vTimerCallbackRe( TimerHandle_t xTimer);

/* TABLES ***************************************************************/

/**
 * @brief ロータリーエンコーダ処理初期化関数
 */
ErType_t xInitRe(void)
{
	Serial.printf("%s : run\n",__func__);

	// 管理データの初期化
	s_slBakRePosition = 0;

	// 割り込み関連の設定
	attachInterrupt(digitalPinToInterrupt(RE_ASGN_SIGNAL_A), vIsrCallbackRe, CHANGE);
	attachInterrupt(digitalPinToInterrupt(RE_ASGN_SIGNAL_B), vIsrCallbackRe, CHANGE);

	//               [0],[1],[2],[3],[4],[5], [6], [7],
	char cName[8] = {'T','m','r','R','e',0x00,0x00,0x00};

	s_xTimerRe = xTimerCreate
					(	cName,						//text name
						RE_CHECK_TIME,				//timer period
						pdTRUE,						//auto-reload
						( void * ) 0,				//number of times
						vTimerCallbackRe				//callback
					);
	if( NULL == s_xTimerRe )
	{
		Serial.printf("%s : error - xTimerCreate\n",__func__);
		return ER_FAIL;
	}

	if( pdTRUE != xTimerReset(	s_xTimerRe, RE_CHECK_TIME) )
	{
		Serial.printf("%s : error - xTimerReset\n",__func__);
		return ER_FAIL;
	}

	Serial.printf("%s : over\n",__func__);
	return ER_OK;
}

/**
 * @brief ロータリーエンコーダのタイマーコールバック関数
 */
static void vTimerCallbackRe( TimerHandle_t xTimer)
{
	static int32_t pos;
	#if DEBUG_API_RE
	Serial.printf("%s - run\n",__func__);
	#endif

	pos = s_xRotaryEncoder.getPosition();
	if( pos != s_slBakRePosition )
	{
		Serial.printf("%s - (%d), (%d)\n",__func__ ,pos ,(pos - s_slBakRePosition));
		s_slBakRePosition = pos;
	}

	#if DEBUG_API_RE
	Serial.printf("%s - over\n",__func__);
	#endif

	return;
}

/**
 * @brief ロータリーエンコーダ割り込み処理
 */
static void IRAM_ATTR vIsrCallbackRe()
{
	s_xRotaryEncoder.tick(); // just call tick() to check the state.
}

#undef _API_RE_C_
