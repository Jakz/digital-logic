#ifndef __GATES_H__
#define __GATES_H__

#include "device.h"

class AndGate : public FixedDevice<2,1>
{
public:
  AndGate() : FixedDevice( [] (Status (&input)[2], Status (&output)[1]) {
    if (input[0] == Status::HIGH && input[1] == Status::HIGH)
      output[0] = Status::HIGH;
    else if (input[0] == Status::LOW || input[1] == Status::LOW)
      output[0] = Status::LOW;
    else
      output[0] = Status::FLOATING;
  })
  {
    
  }
};

class OrGate : public FixedDevice<2,1>
{
private:
  static constexpr Status truthTable[static_cast<int>(Status::COUNT)][static_cast<int>(Status::COUNT)] =
  {
    { Status::LOW, Status::HIGH, Status::LOW },
    { Status::HIGH, Status::HIGH, Status::HIGH },
    { Status::LOW, Status::HIGH, Status::LOW}
  };

public:
  OrGate() : FixedDevice(truthTable)
  {
    
  }
};

#endif
