#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <vector>
#include <array>
#include <functional>

enum class Status
{
  HIGH,
  LOW,
  FLOATING
};

class Device;

class Link
{
  Device* from;
  Device* to;
  size_t fromPin;
  size_t toPin;
};

class Device
{
private:

public:
  virtual size_t inputCount() const = 0;
  virtual size_t outputCount() const = 0;
  
  virtual Device* inputAt(size_t index) = 0;
  virtual Device* outputAt(size_t index) = 0;
  
  virtual void linkInput(size_t index, Device* device) = 0;
  virtual void linkOutput(size_t index, Device* device) = 0;
  
  //virtual void fetchInputStatus() = 0;
  
  static void link(Device* from, size_t output, Device* to, size_t input)
  {
    from->linkOutput(output, to);
    to->linkInput(input, from);
  }
  
  //virtual void update() = 0;
  //virtual void propagate() = 0;
};

template<size_t I, size_t O>
class FixedDevice : public Device
{
public:
  typedef std::function<void(Status (&input)[I], Status (&output)[O])> update_function;

private:
  std::array<Device*, I> inputs;
  std::array<Device*, O> outputs;
  update_function lambda;
  
  Status inputStatus[I];
  Status outputStatus[O];
  
protected:
  FixedDevice(update_function lambda) : lambda(lambda)
  {
    static_assert(I > 0 || O > 0, "FixedDevice must have atleast one input or one output!");
  }

public:
  size_t inputCount() const override { return I; }
  size_t outputCount() const override { return O; }
  
  void linkInput(size_t index, Device* device) override { inputs[index] = device; }
  void linkOutput(size_t index, Device* device) { outputs[index] = device; }
  
  inline Device* inputAt(size_t index) override { return inputs[index]; }
  inline Device* outputAt(size_t index) override { return outputs[index]; }
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
    this->status = status;
  }), status(Status::FLOATING)
  {
    
  }
  
  Status getStatus() const { return status; }
};


#endif
