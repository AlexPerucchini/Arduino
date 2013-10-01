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
  Serial.println(F("b = Blink LED"));
  Serial.println(F("e = Explore"));
  Serial.println(F("i = Show system information"));
  Serial.println(F("h = Show menu"));
  Serial.println(F("? = Show menu"));
  Serial.println(F(""));
  Serial.println(F("Type command, followed by Enter"));
}
