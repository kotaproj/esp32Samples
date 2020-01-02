#include <Arduino.h>
#include "ApiSw.h"

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);

	ErType_t xErType;

	xErType = xInitSw();
	if( ER_OK != xErType )
	{
		Serial.printf("%s - error - %d",__func__ ,xErType);
	}
}

void loop() {
	// put your main code here, to run repeatedly:
}
