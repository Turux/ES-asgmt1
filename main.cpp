//*********************************************************************
//
//  Embedded Software
//  B31DG
//
//  Assignment 1
//
//  Author: Lorenzo Stilo
//  Date: Jan 2016
//
//*********************************************************************
//
//  main.cpp
//  Version: 1.4 (Milestone 3)
//
//  Device: mbed LPC1768
//  WattBob I
//
//  Requirements: MCP23017.h, MCP23017.cpp, alphabet.h, alphabet.cpp
//
//
//          WattBob I
//          -------------
//          |           |
//          |  LPC1768  |
//          |           |
//          |        P21|--------------<SigA>---------|Pulses |----
//          |        P22|-----------------<SigB>------|Trigger|----
//          |           |
//***********************************************************************
//
//  This code will generate a sequence of pulses called sigA (p21) and
//  and a trigger called sigB (p22).
//  The sequence is dectated by the first 5 letters of the surname of
//  the student.
//  Any letter has a correspondent integer number from 1 to 13 as been
//  defined in the library 'alphabet.h'.
//  -First letter will control the width of the first pulse
//  -Second letter will control the spaces between pulses
//  -Third letter will control the number of pulses
//  -Forth letter will control the space between sequences
//  -Fifth letter will control the alternative mode to generate the sequence
//
//  This code has two inputs.
//  In this version the inputs are switches 1 and 2 in WattBob board
//  * If SW1 is pressed -----------> NO OUTPUT
//                      
//  * If SW1 is released |
//                       |
//                       |* If SW2 is pressed -------> Alternative mode
//                       |
//                       |* If SW2 is released -------> Normal mode  
//
//*********************************************************************

#include "mbed.h"
#include "MCP23017.h"
#include "alphabet.h"

//
// Setting up Output Pins
//

// Set Pin 21 on LPC1768 as output for main signal (sigA)

DigitalOut  sigA(p21);

// Set Pin 22 on LPC1768 as output for trigger signal (sigB) 

DigitalOut  sigB(p22);

//-------------------------------------------------------------------

/* Constants */

//
// Defining constants based on surname letters valid in normal mode
//

// Width of the 1st pulse

int const NORM_WIDTH(alphabet('S') * 100);

// Space between pulses in a sequence

int const NORM_PULSE_SPACE(alphabet('T') * 100);

// Number of pulses in a sequence

int const NORM_N_PULSE(alphabet('I') + 7);

// Space between sequences 

int const NORM_SEQ_SPACE(alphabet('L') * 500);

// Define the alternative mode to use (Rage 1-4)

int const MODE((alphabet('O')%4)+1);

// --------------------------------------------------------------

/* Functions */

/* Fixes the timing between sequences */
// Returns desired_value-1000 to fix delayed due to the use 
// of MCP23017 module.
// @param   desired_value   Deseried value in us
// @returns                 Fixed_value in us 
int fix_time(int desired_value)
{
    if (desired_value > 1000)
        return desired_value-1000;
    return 0;
}

/*  Generate a pulses */
//  @param  pin     Output pin
//  @param  width   Width of the first pulse (in us)
//  @param  space   Space between pulses (in us)
//  @param  n       Number of pulses
void pulse(DigitalOut pin, int width, int space, int n)
{
    volatile int i;
    
    //Initialize the output to Low
    pin = 0;
    
    // Loop for n times
    for (i=0; i<n; i++)
    {
        //Set the output to Hi
        pin = 1;
        //Wait (increases at every loop)
        wait_us(width+(50*i));
        //Set the pin to Lo
        pin = 0;
        //Wait 
        wait_us(space);
    }
}

/*  Genrate a Trigger signal */
//  @param  pin     Output pin
//  @param  width   Width of the trigger
void trigger(DigitalOut pin, int width)
{
    //Set the output pin to Hi
    pin = 1;
    //Wait
    wait_us(width);
    //Set the output pin to Low
    pin = 0;
}

/*  Generate a sequence in normal mode */
//  
//  NB: To be called in a loop
void sequence()
{    
    //Generate the trigger signal (10 us width)
    trigger(sigB, 10);
    //Generate the sequence as expressed by constants
    pulse(sigA, NORM_WIDTH, NORM_PULSE_SPACE, NORM_N_PULSE);
    //Wait between sequences
    wait_us(fix_time(NORM_SEQ_SPACE));
}

/*  Generate a sequence */
//  @param  width_1st       Width of the 1st pulse (in us)
//  @param  space_pulse     Space between pulses (in us)
//  @param  n_pulse         Number of pulses in a sequence
//  @param  space_sequence  Space between sequences
//
//  NB: To be called in a loop 
void sequence(int width_1st, int space_pulse, int n_pulse, int space_sequence)
{
    //Generate the trigger signal (10 us width)
    trigger(sigB, 10);
    //Generate the sequence
    pulse(sigA, width_1st, space_pulse, n_pulse);
    //Wait between sequences
    wait_us(fix_time(space_sequence));
}

/*  Generates an inverted sequence */
//
//  NB: To be called in a loop
void inverted_sequence()
{    
    volatile int i;
    
    // Generate the Trigger signal
    trigger(sigB, 10);
    //Set the output to Hi
    sigA = 1;
    //Loop 
    for (i=0; i<NORM_N_PULSE; i++)
    {
        //Set the output to Low
        sigA = 0;
        //Wait
        wait_us(NORM_WIDTH+(50*i));
        //Set the output to Hi
        sigA = 1;
        //Wait
        wait_us(NORM_PULSE_SPACE);
    }
    //Wait between sequences
    wait_us(fix_time(NORM_SEQ_SPACE));
}

int main() 
{        
    //
    //Initialize the MCP23017 module to use switches 
    //
    
    //Define an object MCP23017
    MCP23017 *par_port;
    par_port = new MCP23017(p9, p10, 0x40);
    //Configure switches
    par_port->config(0x0F00, 0x0F00, 0x0F00);
    
    while(1) 
    {
        //Read value from switches (Pin 8 and Pin 9)
        bool sw1 = par_port->read_bit(8);
        bool sw2 = par_port->read_bit(9);
        
        //If switch 1 is relesed
        if(sw1)
        {
            //Disable pulses
            sigA = 0;
            sigB = 0;
        }
        //If switch 1 is pressed 
        else
        {
            //If switch 2 is pressed
            if(sw2)
            {
                //Modified mode (depending on constant mode)
                switch (MODE)
                {
                    case 1: 
                            //Genrate a sequence with less pulses
                            sequence(NORM_WIDTH, NORM_PULSE_SPACE, NORM_N_PULSE-3, NORM_SEQ_SPACE); 
                            break;
                    case 2: 
                            //Generate an inverted sequence
                            inverted_sequence(); 
                            break;
                    case 3:
                            //Generate a sequence with more pulses
                            sequence(NORM_WIDTH, NORM_PULSE_SPACE, NORM_N_PULSE+3, NORM_SEQ_SPACE); 
                            break;
                    case 4:
                            //Generate a more compressed sequence
                            sequence(NORM_WIDTH, NORM_PULSE_SPACE/2, NORM_N_PULSE, NORM_SEQ_SPACE/2); 
                            break;
                    default:
                            //Safe call to Normal mode
                            sequence();
                }   
            }
            //If switch 2 is relesed
            else
            {
                //Normal mode
                sequence();
            }
        }
    }
    
    return 0;
}
