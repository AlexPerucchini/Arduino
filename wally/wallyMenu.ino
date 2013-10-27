/*
wallyMenu.pde
Author: Alex Perucchini

This .pde file gets concatenated at the end of the Wally.ino file to separate
all functions calls from the main  driver
*/

//=============================================================================
// Menu Functions
//=============================================================================

void showMenu(void)
{
  Serial.println(F(""));
  Serial.println(F("Wally autonomous rover system..."));
  Serial.println(F("-----------"));
  Serial.println(F("b = Blink & Beep"));
  Serial.println(F("e = Explore"));
  Serial.println(F("i = System Information"));
  Serial.println(F("t = System Telemetry"));
  Serial.println(F("? = Menu"));
  Serial.println(F(""));
  Serial.println(F("Type command, followed by Enter"));
}
