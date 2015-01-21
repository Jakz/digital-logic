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

#endif
