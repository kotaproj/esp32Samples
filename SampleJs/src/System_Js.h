/**
 * @file System_Js.h
 * @brief JoyStick定義ヘッダ
 */

#ifndef _SYSTEM_JS_H_
#define _SYSTEM_JS_H_

#include "System.h"

/* DECLARATIONS ***************************************************************/

#define USE_JS			1
	#if USE_JS
	#define JS_ASGN_AXIS_X		(26)
	#define JS_ASGN_AXIS_Y		(25)
	#endif	//USE_JS


/* DECLARATIONS - enum ********************************************************/

/**
 * @enum JsCode_t
 * @brief JoyStick識別子
 * @note -		
 */
typedef enum
{
	JS_CODE_CENTER  = (0),
	JS_CODE_UP ,
	JS_CODE_DOWN ,
	JS_CODE_RIGHT ,
	JS_CODE_LEFT ,
	JS_CODE_ILL = (0xFF),
} JsCode_t;

/* DECLARATIONS - struct ******************************************************/

/**
 * @struct JsMessage_t
 * @brief メッセージ - JoyStick用メッセージデータ
 * @note		
 */
typedef struct
{
	ul	ulCode;
	sl	slCount;
	uc	ucJsserve[24];
} JsMessage_t;

/**
 * @struct MessageCtlJs_t
 * @brief メッセージ - JoyStick用メッセージ
 * @note		
 */
typedef struct
{
	MessageHead_t			Head_t;			///< 全体ソース指定
	JsMessage_t				Data_t;			///< 全体ソース指定
} MessageCtlJs_t;

#endif	/* _SYSTEM_JS_H_ */
