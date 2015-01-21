//
//  main.cpp
//  DigitalLogic
//
//  Created by Jack on 1/21/15.
//  Copyright (c) 2015 Jack. All rights reserved.
//

#include <iostream>

#include "gates.h"

std::ostream& operator<< (std::ostream& stream, const Status& status) {
  if (status == Status::LOW) stream << "LOW";
  else if (status == Status::HIGH) stream << "HIGH";
  else if (status == Status::FLOATING) stream << "FLOATING";
  return stream;
}

int main(int argc, const char * argv[])
{
  PinInput *input1 = new PinInput(Status::LOW);
  PinInput *input2 = new PinInput(Status::LOW);
  PinOutput *output = new PinOutput();
  
  OrGate *gate = new OrGate();
  
  Link::create(input1, gate, 0, 0);
  Link::create(input2, gate, 0, 1);
  Link::create(gate, output, 0, 0);
  
  input1->propagate();
  input2->propagate();
  
  std::cout << output->getStatus() << std::endl;
  
  return 0;
}

