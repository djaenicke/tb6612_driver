#ifndef TB6612_H_
#define TB6612_H_

#include "mbed.h"

namespace tb6612
{
static const float vdrop = 0.1;

typedef enum
{
  REVERSE = -1,
  UNKNOWN_DIR = 0,
  FORWARD = 1
} Direction;

typedef enum
{
  MOTOR_A = 0,
  MOTOR_B
} MotorId;

typedef enum
{
  REVERSED = -1,
  STANDARD = 1
} MotorPolarity;

class TB6612
{
 public:
  TB6612(PinName pwm_a, PinName pwm_b, PinName a_in1, PinName a_in2, PinName b_in1, PinName b_in2,
         MotorPolarity a_polarity, MotorPolarity b_polarity);
  void setPWMPeriod(float period_s);
  void setDirection(MotorId motor, Direction new_dir);
  Direction getDirection(MotorId motor);
  void setDutyCycle(MotorId motor, uint8_t percent);
  void stop(MotorId motor);
  void freewheel(void);

 private:
  Direction handlePolarity(MotorPolarity polarity, Direction new_dir);

  DigitalOut a_in1_;
  DigitalOut a_in2_;
  DigitalOut b_in1_;
  DigitalOut b_in2_;
  PwmOut pwm_a_;
  PwmOut pwm_b_;
  MotorPolarity a_polarity_;
  MotorPolarity b_polarity_;
  Direction motor_a_dir_;
  Direction motor_b_dir_;
};

}  // namespace tb6612

#endif  // TB6612_H_
