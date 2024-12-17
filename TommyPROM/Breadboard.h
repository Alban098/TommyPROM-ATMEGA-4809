#ifndef INCLUDE_BREADBOARD_H
#define INCLUDE_BREADBOARD_H

#include "Arduino.h"
#include "PromDevice.h"

/*****************************************************************************/
/*****************************************************************************/
/**
 * Breadboard CPU class
 */
class Breadboard : public PromDevice
{
  public:
    Breadboard(uint32_t size, word blockSize);
    void begin();
    const char * getName() { return "Breadboard CPU"; }

  protected:
    void setAddress(uint32_t address);
    byte readByte(uint32_t address);
    bool burnByte(byte value, uint32_t address);
    bool burnBlock(byte data[], uint32_t len, uint32_t address);
    bool waitForWriteCycleEnd(byte lastValue);
    void setByte(byte value, uint32_t address);
};

#endif  // #define INCLUDE_BREADBOARD_H