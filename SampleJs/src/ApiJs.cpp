/**
 * @file ApiJs.cpp
 * @brief JoyStick管理
 */

#include "ApiJs.h"
// #include "CtlTaskJs.h"

/// @cond
#define _API_JS_C_
/// @endcond

/* DECLARATIONS ***************************************************************/
#define DEBUG_API_JS			(0)			//0:release, 1:debug
	#if DEBUG_API_JS
	#endif

#define JS_CHECK_TIME			(50)		//JoyStickチェック間隔:50msec

const char* str_CENTER = "CENTER";
const char* str_UP = "UP";
const char* str_DOWN = "DOWN";
const char* str_RIGHT = "RIGHT";
const char* str_LEFT = "LEFT";
const char* str_ILL = "ILL";

typedef struct
{
	JsCode_t xKey;
	const char* str;
}DbgMsg_t;

const DbgMsg_t c_xaDbgMsg_t[] = {
	JS_CODE_CENTER, str_CENTER,
	JS_CODE_UP, str_UP,
	JS_CODE_DOWN, str_DOWN,
	JS_CODE_RIGHT, str_RIGHT,
	JS_CODE_LEFT, str_LEFT,
};

static void vDebugJsMsg(const char* str_func, JsCode_t xJsCode, int32_t x, int32_t y)
{
	const char* ptr = str_ILL;

	for(int32_t cnt=0; cnt<(sizeof(c_xaDbgMsg_t)/sizeof(DbgMsg_t)); cnt++)
	{
		if( c_xaDbgMsg_t[cnt].xKey == xJsCode )
		{
			ptr = c_xaDbgMsg_t[cnt].str;
			break;
		}
	}
	Serial.printf("%s - %s - (%d, %d)\n",str_func ,ptr ,x ,y);

	return;
}

typedef struct
{
    JsCode_t xJsCode;
    int32_t x;
    int32_t y;
}JsCrd_t;

/* VARIABLES ******************************************************************/
static JsCrd_t s_xBakJsCrd = {JS_CODE_ILL, (-1), (-1)};
static TimerHandle_t s_xTimerJs = NULL;

/* PRIVATE PROTOTYPES *********************************************************/
static void vTimerCallbackJs( TimerHandle_t xTimer);

/* TABLES ***************************************************************/

/**
 * @brief JoyStick処理初期化関数
 */
ErType_t xInitJs(void)
{
	Serial.printf("%s : run\n",__func__);

	// PIN設定
	pinMode(JS_ASGN_AXIS_X, INPUT);
	pinMode(JS_ASGN_AXIS_Y, INPUT);

	//               [0],[1],[2],[3],[4],[5], [6], [7],
	char cName[8] = {'T','m','r','J','s',0x00,0x00,0x00};

	s_xTimerJs = xTimerCreate
					(	cName,						//text name
						JS_CHECK_TIME,				//timer period
						pdTRUE,						//auto-reload
						( void * ) 0,				//number of times
						vTimerCallbackJs				//callback
					);
	if( NULL == s_xTimerJs )
	{
		Serial.printf("%s : error - xTimerCreate\n",__func__);
		return ER_FAIL;
	}

	if( pdTRUE != xTimerReset(	s_xTimerJs, JS_CHECK_TIME) )
	{
		Serial.printf("%s : error - xTimerReset\n",__func__);
		return ER_FAIL;
	}

	Serial.printf("%s : over\n",__func__);
	return ER_OK;
}

/**
 * @brief JoyStickのタイマーコールバック関数
 */
static void vTimerCallbackJs( TimerHandle_t xTimer)
{
	#if DEBUG_API_JS
	Serial.printf("%s - run\n",__func__);
	#endif

	int32_t x = analogRead(JS_ASGN_AXIS_X);
	int32_t y = analogRead(JS_ASGN_AXIS_Y);

	JsCode_t xJsCode;
	if( x < 500 )
	{
		xJsCode = JS_CODE_LEFT;
	}
	else if( x > 3600 )
	{
		xJsCode = JS_CODE_RIGHT;
	}
	else
	{
		if( y < 500 )
		{
			xJsCode = JS_CODE_DOWN;
		}
		else if( y > 3600 )
		{
			xJsCode = JS_CODE_UP;
		}
		else
		{
			xJsCode = JS_CODE_CENTER;
		}
	}


	// 状態の判定
	if( s_xBakJsCrd.xJsCode != xJsCode )
	{
		vDebugJsMsg(__func__, xJsCode, x, y);
		s_xBakJsCrd.xJsCode = xJsCode;
	}

	#if DEBUG_API_JS
	Serial.printf("%s - over\n",__func__);
	#endif

	return;
}

#undef _API_JS_C_
