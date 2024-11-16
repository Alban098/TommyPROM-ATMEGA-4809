// This controls the shift register that generates the address lines for A0..A15 for most
// chip families.  This is not used by the PromDevice8755A code.
//
// Note that this uses direct port control instead of digitalWrite calls so that the code
// can run fast enough to meet the tBLC requirements for SDP and block writes.  This
// sacrifices portability and readability for speed. //
//
// This code will only work on Arduino Uno and Nano hardware.  The ports for other
// Arduinos map to different IO pins.

#include "PromAddressDriver.h"

#define ADDR_CLK_HI     A0
#define ADDR_CLK_LO     A6
#define ADDR_DATA       A7

// Define masks for the address clk and data lines on PC3..PC5 for direct port control.
#define ADDR_CLK_HI_MASK    0x08
#define ADDR_CLK_LO_MASK    0x10
#define ADDR_DATA_MASK      0x20

void PromAddressDriver::begin()
{
    // The address control pins are always outputs.
    pinMode(ADDR_DATA, OUTPUT);
    pinMode(ADDR_CLK_LO, OUTPUT);
    pinMode(ADDR_CLK_HI, OUTPUT);
    digitalWrite(ADDR_DATA, LOW);
    digitalWrite(ADDR_CLK_LO, LOW);
    digitalWrite(ADDR_CLK_HI, LOW);

    // To save time, the setAddress only writes the hi byte if it has changed.
    // The value used to detect the change is initialized to a non-zero value,
    // so set an initial address to avoid the the case where the first address
    // written is the 'magic' initial address.
    setAddress(0x0000);
}


// Set a 16 bit address in the two address shift registers and
// the upper bits on the extened address pins.
void PromAddressDriver::setAddress(uint32_t address)
{
    static byte lastHi = 0xca; 
    byte hi = (address >> 8) & 0xff;
    byte lo = address & 0xff;

    if (hi != lastHi)
    {
        setAddressRegister(ADDR_CLK_HI, hi);
        lastHi = hi;
    }
    setAddressRegister(ADDR_CLK_LO, lo);
}


// Shift an 8-bit value into one of the address shift registers.  Note that
// the data pins are tied together, selecting the high or low address register
// is a matter of using the correct clock pin to shift the data in.
void PromAddressDriver::setAddressRegister(uint8_t clkPin, byte addr)
{
    byte mask = 0;
    if (clkPin == ADDR_CLK_HI)
        mask = ADDR_CLK_HI_MASK;
    else if (clkPin == ADDR_CLK_LO)
        mask = ADDR_CLK_LO_MASK;

    // Make sure the clock is low to start.
    PORTD.OUT &= ~mask;

    // Shift 8 bits in, starting with the MSB.
    for (int ix = 0; (ix < 8); ix++)
    {
        // Set the data bit
        if (addr & 0x80)
        {
            PORTD.OUT |= ADDR_DATA_MASK;
        }
        else
        {
            PORTD.OUT &= ~ADDR_DATA_MASK;
        }

        // Toggle the clock high then low
        PORTD.OUT |= mask;
        delayMicroseconds(3);
        PORTD.OUT &= ~mask;
        addr <<= 1;
    }
}
