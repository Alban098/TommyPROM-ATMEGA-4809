#include "Configure.h"
#if defined(PROM_IS_BREADBOARD)

#include "PromAddressDriver.h"

#define RAM_IN          16

// Set the status of the device control pins
static void commit() { 
  digitalWrite(RAM_IN, HIGH); 
  delayMicroseconds(1);
  digitalWrite(RAM_IN, LOW); 
}

// Pin order is reversed, so it's easier to switch here than to redo the hardware ...
static byte transformData(byte data) {
  byte transformed = 0x00;
  transformed |= (data & 0x80) >> 7;
  transformed |= (data & 0x40) >> 5;
  transformed |= (data & 0x20) >> 3;
  transformed |= (data & 0x10) >> 1;
  transformed |= (data & 0x08) << 1;
  transformed |= (data & 0x04) << 3;
  transformed |= (data & 0x02) << 5;
  transformed |= (data & 0x01) << 7;
  return transformed;
}

Breadboard::Breadboard(uint32_t size, word blockSize)
    : PromDevice(size, blockSize, 1, false)
{
}


void Breadboard::begin()
{
    // Define the data bus as input initially so that it does not put out a
    // signal that could collide with output on the data pins of the EEPROM.
    setDataBusMode(INPUT);

    // Define the EEPROM control pins as output, making sure they are all
    // in the disabled state.
    digitalWrite(RAM_IN, LOW);
    pinMode(RAM_IN, OUTPUT);

    // This chip uses the shift register hardware for addresses, so initialize that.
    PromAddressDriver::begin();
}


// BEGIN PRIVATE METHODS
//

// Use the PromAddressDriver to set an address in the two address shift registers.
void Breadboard::setAddress(uint32_t address)
{
    PromAddressDriver::setAddress(address);
}


// Not supported
byte Breadboard::readByte(uint32_t address)
{
    return 0;
}


// Burn a byte to the chip and verify that it was written.
bool Breadboard::burnByte(byte value, uint32_t address)
{
    bool status = false;

    setAddress(address);
    setDataBusMode(OUTPUT);
    writeDataBus(transformData(value));
    commit();

    status = waitForWriteCycleEnd(value);

    return status;
}


bool Breadboard::burnBlock(byte data[], uint32_t len, uint32_t address)
{
    bool status = false;
    if (len == 0)  return true;

    ++debugBlockWrites;

    // Write all of the bytes in the block out to the chip.  The chip will
    // program them all at once as long as they are written fast enough.
    setDataBusMode(OUTPUT);
    for (uint32_t ix = 0; (ix < len); ix++)
    {
        setAddress(address + ix);
        writeDataBus(transformData(data[ix]));

        commit();
    }

    status = waitForWriteCycleEnd(data[len - 1]);

    if (!status) {
        debugLastAddress = address + len - 1;
    }
    return status;
}


bool Breadboard::waitForWriteCycleEnd(byte lastValue)
{
    // Read not supported, skipping
    delayMicroseconds(5);
    return true;
}


// Set an address and data value and toggle the write control.  This is used
// to write control sequences, like the software write protect.  This is not a
// complete byte write function because it does not set the chip enable or the
// mode of the data bus.
void Breadboard::setByte(byte value, uint32_t address)
{
    setAddress(address);
    writeDataBus(transformData(value));
    commit();
}

#endif // #if defined(PROM_IS_BREDBOARD)