/*
 * SAMD RTC
 * Seeeduino XIAO
 * October 16, 2022
 */

void setup() {

  SerialUSB.begin(9600);
  while (! SerialUSB) {;}
//  delay(5000);
  SerialUSB.println("Serial started");
  
  // prescale GCLK4
  REG_GCLK_GENDIV = 
      GCLK_GENDIV_ID(0x04)
    | GCLK_GENDIV_DIV(4);
  while (GCLK->STATUS.bit.SYNCBUSY) {;}
  // select external 32K crystal as source
  REG_GCLK_GENCTRL =
      GCLK_GENCTRL_ID(0x04)
    | GCLK_GENCTRL_SRC_XOSC32K 
    | GCLK_GENCTRL_DIVSEL;
  while (GCLK->STATUS.bit.SYNCBUSY) {;}
  GCLK->GENCTRL.bit.GENEN = 1; // enable the generator
  while (GCLK->STATUS.bit.SYNCBUSY) {;}
  // route output to the RTC peripheral
  GCLK->CLKCTRL.reg = 
      GCLK_CLKCTRL_ID_RTC
    | GCLK_CLKCTRL_GEN_GCLK4; // component/gclk.h line 202
  while (GCLK->STATUS.bit.SYNCBUSY) {;}
  GCLK->CLKCTRL.bit.CLKEN = 1; // enable the GCLK
  while (GCLK->STATUS.bit.SYNCBUSY) {;}
  // apply power to the RTC
  PM->APBASEL.bit.APBADIV = 0; // don't prescale the PM clock
  PM->APBAMASK.bit.RTC_ = 1; // unmask the RTC
  
  // Configure RTC
  REG_RTC_MODE2_CTRL &= ~(RTC_MODE2_CTRL_ENABLE); // disable RTC
  /*
   * Set RTC to clock mode
   * ClockRep = 0 = 24-hour mode
   * MatchClr = 0 = do not clear counter on alarm match
   * Also sets...
   * Enable = 0 = keep it disabled
   * SWreset = 0 = no effect
   * Prescale = 0, to be set next
   */
  REG_RTC_MODE2_CTRL = 
     RTC_MODE2_CTRL_MODE(RTC_MODE2_CTRL_MODE_CLOCK_Val)
   | RTC_MODE2_CTRL_PRESCALER(RTC_MODE2_CTRL_PRESCALER_DIV1024_Val);
  while (REG_RTC_STATUS & 0b10000000) {;} 
  REG_RTC_MODE2_CTRL |= (RTC_MODE2_CTRL_ENABLE); // enable RTC
  while (REG_RTC_STATUS & 0b10000000) {;}
  // set the time to 7:30 a.m. Oct 16 2022
  // this code defines ref to be 2000
  REG_RTC_MODE2_CLOCK = 
     RTC_MODE2_CLOCK_YEAR(22) // offset from ref 2000
   | RTC_MODE2_CLOCK_MONTH(10)
   | RTC_MODE2_CLOCK_DAY(16)
   | RTC_MODE2_CLOCK_HOUR(7)
   | RTC_MODE2_CLOCK_MINUTE(30)
   | RTC_MODE2_CLOCK_SECOND(0);
  while (REG_RTC_STATUS & 0b10000000) {;}
  // read back the and print the clock time
  REG_RTC_READREQ |= 1; // start read request
  while (REG_RTC_STATUS & 0b10000000) {;}
  uint32_t myDT = REG_RTC_MODE2_CLOCK;
  SerialUSB.println("Initial:");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_MONTH_Msk) >> RTC_MODE2_CLOCK_MONTH_Pos );
  SerialUSB.print("/");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_DAY_Msk) >> RTC_MODE2_CLOCK_DAY_Pos );
  SerialUSB.print("/");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_YEAR_Msk) >> RTC_MODE2_CLOCK_YEAR_Pos );
  SerialUSB.print(" ");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_HOUR_Msk) >> RTC_MODE2_CLOCK_HOUR_Pos );
  SerialUSB.print(":");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_MINUTE_Msk) >> RTC_MODE2_CLOCK_MINUTE_Pos );
  SerialUSB.print(":");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_SECOND_Msk) >> RTC_MODE2_CLOCK_SECOND_Pos );
  SerialUSB.println();

  
}

void loop() {
  // wait 5 seconds
  delay(5000);

  // read the clock again
  REG_RTC_READREQ |= 1; // start read request
  while (REG_RTC_STATUS & 0b10000000) {;}
  uint32_t myDT = REG_RTC_MODE2_CLOCK;
  SerialUSB.println("Update:");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_MONTH_Msk) >> RTC_MODE2_CLOCK_MONTH_Pos );
  SerialUSB.print("/");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_DAY_Msk) >> RTC_MODE2_CLOCK_DAY_Pos );
  SerialUSB.print("/");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_YEAR_Msk) >> RTC_MODE2_CLOCK_YEAR_Pos );
  SerialUSB.print(" ");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_HOUR_Msk) >> RTC_MODE2_CLOCK_HOUR_Pos );
  SerialUSB.print(":");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_MINUTE_Msk) >> RTC_MODE2_CLOCK_MINUTE_Pos );
  SerialUSB.print(":");
  SerialUSB.print( (myDT & RTC_MODE2_CLOCK_SECOND_Msk) >> RTC_MODE2_CLOCK_SECOND_Pos );
  SerialUSB.println();


}
