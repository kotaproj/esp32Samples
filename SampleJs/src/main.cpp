#include <Arduino.h>
#include "ApiJs.h"

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	delay(100);
	Serial.printf("%s - run",__func__);

	ErType_t xErType;

	xErType = xInitJs();
	if( ER_OK != xErType )
	{
		Serial.printf("%s - error - %d\n",__func__ ,xErType);
	}
	Serial.printf("%s - over\n",__func__);
}

void loop() {
	// put your main code here, to run repeatedly:
}
