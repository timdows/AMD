//
//
//

#include "WS28xx.h"

//Constructor
WS28xx::WS28xx(uint8_t childRed, uint8_t childGreen, uint8_t childBlue, uint8_t childAll)
{
    /*for (int i = 0; i < NUMBER_OF_SCENES; i++) {
		this->sceneStatus[i] = false;
	}*/

    this->childRed = childRed;
    this->childGreen = childGreen;
    this->childBlue = childBlue;
    this->childAll = childAll;

    this->strip.begin();
    //this->strip.show(); // Initialize all pixels to 'off', but a bug that it cannot be enabled??
}

void WS28xx::SetAllOff()
{
    Serial.println("SetAllOff");
    send(MyMessage(this->childRed, V_DIMMER).set(0));
    send(MyMessage(this->childGreen, V_DIMMER).set(0));
    send(MyMessage(this->childBlue, V_DIMMER).set(0));
    send(MyMessage(this->childAll, V_DIMMER).set(0));

    for (int scene = 0; scene < NUMBER_OF_SCENES; scene++)
    {
	send(MyMessage(scene + NUMBER_OF_SCENES + 1, V_LIGHT).set(0));
    }

    this->sceneStatus.setAllOff();
}

void WS28xx::Update()
{
    if (this->sceneStatus.scene1 == true)
    {
	this->KnightRider();
    }
    else if (this->sceneStatus.scene2 == true)
    {
	this->RainbowCycle();
    }
    else if (this->sceneStatus.scene3 == true)
    {
	if (this->currentFullRGB == false)
	{
	    this->FullRGBColor(true);
	}
    }
    else if (this->sceneStatus.scene4 == true)
    {
	this->Disco();
    }
}

void WS28xx::ProcessScene(int sceneNumber, bool state)
{
    if (this->sceneStatus.hasRunningScene())
    {
	int runningSceneNumer;
	if (this->sceneStatus.scene1)
	{
	    runningSceneNumer = 1;
	}
	else if (this->sceneStatus.scene2)
	{
	    runningSceneNumer = 2;
	}
	else if (this->sceneStatus.scene3)
	{
	    runningSceneNumer = 3;
	}
	else if (this->sceneStatus.scene4)
	{
	    runningSceneNumer = 4;
	}

	if (runningSceneNumer != sceneNumber)
	{
	    send(MyMessage(NUMBER_OF_SCENES + runningSceneNumer, V_LIGHT).set(0));
	}
    }

    if (state)
    {
	this->sceneStatus.setScene(sceneNumber);
    }
    else
    {
	this->sceneStatus.setAllOff();
	this->FullRGBColor(false);
    }
}

void WS28xx::ColorWipe(uint32_t color)
{
    if (this->currentColorWipe < this->strip.numPixels())
    {
	this->currentColorWipe++;
	this->strip.setPixelColor(this->currentColorWipe, color);
	strip.show();
    }
}

/*
*  KnightRider
*/
void WS28xx::KnightRider()
{
    unsigned long currentMillis = millis();

    if (currentMillis - this->previousMillis >= this->delayKnightRider)
    {
	this->previousMillis = currentMillis; // Remember the time

	uint32_t colorOn;
	if (this->greenRGB == 0 && this->redRGB == 0 && this->blueRGB == 0)
	{
	    colorOn = this->strip.Color(0, 255, 0);
	}
	else
	{
	    colorOn = this->strip.Color(
		(int)(this->greenRGB / 100. * 255),
		(int)(this->redRGB / 100. * 255),
		(int)(this->blueRGB / 100. * 255));
	}

	uint32_t colorOff = this->strip.Color(0, 0, 0);

	if (this->currentKnightRider < this->strip.numPixels())
	{
	    this->currentKnightRider++;
	    this->strip.setPixelColor(this->currentKnightRider, colorOn);

	    //Remove after 5 lights
	    int toRemove = this->currentKnightRider - 10;
	    if (toRemove < 0)
	    {
		toRemove = this->strip.numPixels() + toRemove;
	    }
	    this->strip.setPixelColor(toRemove, colorOff);
	    strip.show();
	}
	else
	{
	    this->currentKnightRider = 0;
	}
    }
}

/*
*  Slightly different, this makes the rainbow equally distributed throughout
*/
void WS28xx::RainbowCycle()
{
    unsigned long currentMillis = millis();

    if (currentMillis - this->previousMillis >= this->delayRainbowCycle)
    {
	this->previousMillis = currentMillis; // Remember the time
	this->currentRainbowCycle++;

	for (int i = 0; i < this->strip.numPixels(); i++)
	{
	    this->strip.setPixelColor(i, this->Wheel(((i * 256 / this->strip.numPixels()) + this->currentRainbowCycle) & 255));
	}

	strip.show();

	if (this->currentRainbowCycle == (256 * 5))
	{
	    this->currentRainbowCycle = 0;
	}
    }
}

/*
*  Get color for RGB values, and set full ledstrip to this color
*/
void WS28xx::FullRGBColor(boolean state)
{
    uint32_t color;

    //Turn them on
    if (state)
    {
	this->currentFullRGB = true;
	color = this->strip.Color(
	    (int)(this->greenRGB / 100. * 255),
	    (int)(this->redRGB / 100. * 255),
	    (int)(this->blueRGB / 100. * 255));
    }
    //Turn them off
    else
    {
	this->currentFullRGB = false;
	color = 0;
    }

    for (int i = 0; i < this->strip.numPixels(); i++)
    {
	this->strip.setPixelColor(i, color);
    }

    strip.show();
}

/*
*
*/
void WS28xx::Disco()
{
    unsigned long currentMillis = millis();

    if (currentMillis - this->previousMillis >= this->delayDisco)
    {
	this->previousMillis = currentMillis; // Remember the time

	//Turn all leds off
	for (int i = 0; i < this->strip.numPixels(); i++)
	{
	    this->strip.setPixelColor(i, 0);
	}

	uint32_t color = this->strip.Color(
	    (int)(this->greenRGB / 100. * 255),
	    (int)(this->redRGB / 100. * 255),
	    (int)(this->blueRGB / 100. * 255));

	//Control the amount of leds going on
	for (int i = 0; i < 5; i++)
	{
	    int randomLed = random(1, NUM_LEDS);
	    this->strip.setPixelColor(randomLed, color);
	}

	strip.show();
    }
}

/*
*  Part for updating RGB values
*/
void WS28xx::SetRGBValue(const MyMessage &message)
{
    //  Retrieve the power or dim level from the incoming request message
    int requestedLevel = atoi(message.data);

    // Adjust incoming level if this is a V_LIGHT variable update [0 == off, 1 == on]
    requestedLevel *= (message.type == V_LIGHT ? 100 : 1);

    // Clip incoming level to valid range of 0 to 100
    requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
    requestedLevel = requestedLevel < 0 ? 0 : requestedLevel;

    if (message.sensor == 0)
    {
	this->redRGB = requestedLevel;
    }
    else if (message.sensor == 1)
    {
	this->greenRGB = requestedLevel;
    }
    else if (message.sensor == 2)
    {
	this->blueRGB = requestedLevel;
    }
    else if (message.sensor == 3)
    {   //all
	//Get the highest value to work with
	int highest = this->redRGB;
	if (this->greenRGB > highest)
	{
	    highest = this->greenRGB;
	}
	if (this->blueRGB > highest)
	{
	    highest = this->blueRGB;
	}

	//Calculate the percentage to change all the leds
	double percentToChange = (double)requestedLevel / (double)highest;

	if (highest == 0)
	{
	    this->redRGB = requestedLevel;
	    this->greenRGB = requestedLevel;
	    this->blueRGB = requestedLevel;
	}
	else if (percentToChange > 0)
	{
	    this->redRGB = floor(this->redRGB * percentToChange);
	    this->greenRGB = floor(this->greenRGB * percentToChange);
	    this->blueRGB = floor(this->blueRGB * percentToChange);
	}
	//Never gets here?
	else
	{
	    this->redRGB = requestedLevel;
	    this->greenRGB = requestedLevel;
	    this->blueRGB = requestedLevel;
	}

	Serial.print("requestedLevel ");
	Serial.print(requestedLevel);
	Serial.print(" highest ");
	Serial.print(highest);
	Serial.print(" percent change is ");
	this->PrintDouble(percentToChange, 4);
	Serial.println();

	//ledAll.SetDesiredLedLevel(requestedLevel);
	//ledAll.ForceGwUpdate();*/

	//Update RGB values via gateway
	send(MyMessage(0, V_DIMMER).set(this->redRGB));
	send(MyMessage(1, V_DIMMER).set(this->greenRGB));
	send(MyMessage(2, V_DIMMER).set(this->blueRGB));
    }

    //Make sure to update other running scenes
    this->currentFullRGB = false;
}

void WS28xx::PrintDouble(double val, byte precision)
{
    Serial.print(int(val)); // Print int part
    if (precision > 0)
    { // Print decimal part
	Serial.print(".");
	unsigned long frac, mult = 1;
	byte padding = precision - 1;
	while (precision--)
	    mult *= 10;
	if (val >= 0)
	    frac = (val - int(val)) * mult;
	else
	    frac = (int(val) - val) * mult;
	unsigned long frac1 = frac;
	while (frac1 /= 10)
	    padding--;
	while (padding--)
	    Serial.print("0");
	Serial.print(frac, DEC);
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t WS28xx::Wheel(byte WheelPos)
{
    if (WheelPos < 85)
    {
	return this->strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if (WheelPos < 170)
    {
	WheelPos -= 85;
	return this->strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
	WheelPos -= 170;
	return this->strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}
