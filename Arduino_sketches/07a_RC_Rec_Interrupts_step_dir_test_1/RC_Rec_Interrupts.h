/*
  RC Transmitter output to Arduino
  Transmitter: Radiolink AT9 , Receiver: Radiolink R9DS
  but code below should work for other Transmitters and Receivers

  Current Channels in Use
  -----------------------
  Radiolink R9DS  <---> Arduino <---> Fn
  --------------------------------------
  Ch3             <---> A8      <---> LJS UD 
  Ch2             <---> A9      <---> RJS UD
  Ch4             <---> A10     <---> RJS LR
  Ch5             <---> A11     <---> Right Side back, Slider UD
  Ch9             <---> A12     <---> Left Side switch B: Up~1082, Down~1900 (two way switch)

  Ch3 is Failsafe, pwr off ~ 1920 (so If <1880, RCTx is ALIVE)
  HOWEVER, this ~1920 only happens AFTER the LJS UD is moved at least once after the initial power-on
  Because when sketch inits, input[] are zeroed.
  HENCE, either set the inital Failsafe Chn inited to 1920 OR during each ISR cycle, test for "aliveness" is [ch3 > 0 && ch3 <1880]

  Info on R9DS all 9 channels
  ---------------------------
  RC ch 1: LJS_LR, L~1071   C~1486 to 1495   R~1893 
  RC Ch 2: AT9 RJS_UD, U ~ 1120, D ~ 1870, C ~ 1480 to 1520 
  RC Ch 3: AT9 LJS_UD, U ~ 1120, D ~ 1870, C ~ 1480 to 1520 / Failsafe: ~1900
  ... center pos: RC_Active_CA_OFF, Up pos: RC_Active_CA_ON, Down: RC_OFF_Inactive or
  ... center pos: RC_Inactive_Navi_On, Up pos: RC_Active_CA_ON_Navi_OFF, Down: RC_OFF_Inactive
  RC Ch 4: AT9 RJS_LR, L ~ 1120, R ~ 1870, C ~ 1510 to 1560  
  RD9S ch 5 is Right Side slider at back of control, Up ~ 1080, Down ~ 1900 // used for CA on or off
  RD9S ch 6 is left side black small knob
  RD9S ch 7 is Left Side slider at back of control, Up ~ 1080 , Down ~ 1900 // used for NAV On or OFF
  RD9S ch 8 is Right side black small knob, Full Left ~ 1085, Center ~1460 to 1475, Full Right ~ 1900
  RD9S ch 9 is Left Side switch B: Up~1082, Down~1900 (two way switch)
 
*/

/*
  Public Functions
  ================
  //To enable Pin Change Interrupts
  //choices: 2 (PortB) || 11 (PortK)
  //PortB: Ard 10 to 13, 50 to 53 (total 8 pins)
  //PortK: Ard A8 to A15 (total 8 pins)
  void RC_Rec_Int_Enable(11); // 2 or 11 

  // To view the inputs from RC
  RC_Rec_Int_Print_Inputs_Pb();
  or
  RC_Rec_Int_Print_Inputs_Pk();

  All other functions should be treated as private and changed only if req

*/

// Public Vars
volatile unsigned long RC_Rec_Int_Timer_Pb[6];
volatile byte RC_Rec_Int_Last_Channel_Pb[5];
volatile int RC_Rec_Int_Input_Pb[5] = {1920,0,0,0,0};

volatile unsigned long RC_Rec_Int_Timer_Pk[6];
volatile byte RC_Rec_Int_Last_Channel_Pk[5];
volatile int RC_Rec_Int_Input_Pk[5] = {1920,0,0,0,0};



