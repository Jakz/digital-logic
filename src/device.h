#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <vector>
#include <array>
#include <functional>
#include <type_traits>

#include "memory_pool.h"

enum class Status
{
  LOW = 0,
  HIGH,
  FLOATING,
  
  COUNT
};

class Link;

class Device
{
private:

public:
  virtual size_t inputCount() const = 0;
  virtual size_t outputCount() const = 0;
  
  virtual Device* inputAt(size_t index) = 0;
  virtual Device* outputAt(size_t index) = 0;
  
  virtual void linkOutput(size_t index, Link* link) = 0;
  virtual void linkInput(size_t index, Link* link) = 0;
  
  virtual void setInputStatus(size_t index, Status status) = 0;
  virtual Status outputStatusAt(size_t index) const = 0;
  
  virtual void propagate() = 0;
  

  //virtual void fetchInputStatus() = 0;

  //virtual void update() = 0;
  //virtual void propagate() = 0;
};

class Link
{
private:
  Device* from;
  Device* to;
  size_t fromPin;
  size_t toPin;
  
public:
  Link(Device* from, Device* to, size_t fromPin, size_t toPin) : from(from), to(to), fromPin(fromPin), toPin(toPin)
  {
    from->linkOutput(fromPin, this);
    to->linkInput(toPin, this);
  }
  
  ~Link()
  {
    from->linkOutput(fromPin, nullptr);
    to->linkInput(toPin, nullptr);
  }
  
  size_t getInputPin() const { return toPin; }
  
  Device* getInput() { return to; }
  Device* getOutput() { return from; }
  
  Device* otherEnd(Device* device) { return from == device ? to : from; }
  
  static Link* create(Device* from, Device* to, size_t fromPin, size_t toPin)
  {
    return new Link(from,to,fromPin,toPin);
  }
};

template<size_t I, size_t O>
class FixedDevice : public Device
{
public:
  typedef std::function<void(Status (&input)[I], Status (&output)[O])> update_function;

private:
  std::array<Link*, I> inputs{};
  std::array<Link*, O> outputs{};
  update_function lambda;
  
  Status inputStatus[I];
  Status outputStatus[O];
  
protected:
  FixedDevice(update_function lambda) : lambda(lambda)
  {
    static_assert(I > 0 || O > 0, "FixedDevice must have atleast one input or one output!");
  }
  
  template<size_t T = I>
  FixedDevice(const Status truthTable[static_cast<int>(Status::COUNT)][static_cast<int>(Status::COUNT)], typename std::enable_if<(T == 2 && O == 1)>::type* = 0)
  {
    lambda = [truthTable] (Status (&input)[2], Status (&output)[1])
    {
      output[0] = truthTable[static_cast<int>(input[0])][static_cast<int>(input[1])];
    };
  }
  
public:
  size_t inputCount() const override { return I; }
  size_t outputCount() const override { return O; }
  
  void setInputStatus(size_t index, Status status) override { inputStatus[index] = status; }
  
  Status outputStatusAt(size_t index) const override { return outputStatus[index]; }
  
  void linkInput(size_t index, Link* link) override { inputs[index] = link; }
  void linkOutput(size_t index, Link* link) { outputs[index] = link; }
  
  inline Device* inputAt(size_t index) override { return inputs[index] ? inputs[index]->getOutput() : nullptr; }
  inline Device* outputAt(size_t index) override { return outputs[index] ? outputs[index]->getInput() : nullptr; }
  
  void propagate() override
  {
    lambda(inputStatus, outputStatus);
    
    for (size_t i = 0; i < outputCount(); ++i)
    {
      Link* link = outputs[i];
      
      if (link)
      {
        Device* device = link->getInput();
        device->setInputStatus(link->getInputPin(), outputStatusAt(i));
        device->propagate();
      }
    }
  }
};

class PinInput : public FixedDevice<0, 1>
{
private:
  Status status;
  
public:
  PinInput(Status status) : FixedDevice([this](Status (&input)[0], Status (&output)[1]) {
    output[0] = this->status;
  }), status(status)
  {
    
  }
  
  void setStatus(Status status) { this->status = status; }
  Status getStatus() const { return status; }
};

class PinOutput : public FixedDevice<1, 0>
{
private:
  Status status;
  
public:
  PinOutput() : FixedDevice([this](Status (&input)[1], Status (&output)[0]) {
    this->status = input[0];
  }), status(Status::FLOATING)
  {
    
  }
  
  Status getStatus() const { return status; }
};


#endif
