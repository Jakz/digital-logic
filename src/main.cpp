//
//  main.cpp
//  DigitalLogic
//
//  Created by Jack on 1/21/15.
//  Copyright (c) 2015 Jack. All rights reserved.
//

#include <iostream>

#include "gates.h"

int main(int argc, const char * argv[])
{
  PinInput *input1 = new PinInput(Status::LOW);
  PinInput *input2 = new PinInput(Status::HIGH);
  PinOutput *output = new PinOutput();
  
  AndGate *gate = new AndGate();
  
  Device::link(input1, 0, gate, 0);
  Device::link(input2, 0, gate, 1);
  Device::link(gate, 0, output, 0);
  
  
  // insert code here...
  std::cout << "Hello, World!\n";
    return 0;
}

