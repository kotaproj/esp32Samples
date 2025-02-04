/**
 * @file System.h
 * @brief システムヘッダ
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include "Arduino.h"

/* DECLARATIONS ***************************************************************/

/* DECLARATIONS - typedef *****************************************************/
typedef unsigned char	uc;
typedef unsigned short	us;
typedef unsigned long	ul;
typedef signed char		sc;
typedef signed short	ss;
typedef signed long		sl;


/* DECLARATIONS - enum ********************************************************/

/**
 * @enum ErType_t
 * @brief エラータイプ
 * @note -		
 */
typedef enum
{
	ER_OK     = (0x00000000),	///< OK
	ER_FAIL   = (0x00000001),	///< FAIL
	ER_PARAM  = (0x00000002),	///< PARAM
	ER_STATUS = (0x00000004),	///< STATUS
} ErType_t;

/**
 * @enum UniId_t
 * @brief タスク,割り込み識別子
 * @note -		
 */
typedef enum
{
	UID_DEBUG      = (0),	///< デバッグ用
	UID_CTL        ,		///< 管理タスク
	UID_SW         ,		///< スイッチ
	UID_RE  	   ,		///< ロータリーエンコーダ
	UID_RCV_UART   ,		///< UART受信
	UID_HC         ,		///< HTTP Client
	UID_DISP       ,		///< DISPLAY
	UID_DUMMY      ,		///< ダミー用
	UID_NUM        ,		///< 識別子数
} UniId_t;

/* DECLARATIONS - struct ******************************************************/

/**
 * @struct MessageHead_t
 * @brief メッセージ共通ヘッダ
 * @note		
 */
typedef struct
{
	UniId_t xSrcId;
	UniId_t xDstId;
} MessageHead_t;

#endif	/* _SYSTEM_H_ */
