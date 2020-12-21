#include "tb6612.h"

namespace tb6612
{
TB6612::TB6612(PinName pwm_a, PinName pwm_b, PinName a_in1, PinName a_in2, PinName b_in1, PinName b_in2,
               MotorPolarity a_polarity, MotorPolarity b_polarity)
    : pwm_a_(pwm_a), pwm_b_(pwm_b), a_in1_(a_in1), a_in2_(a_in2), b_in1_(b_in1), b_in2_(b_in2)
{
  a_polarity_ = a_polarity;
  b_polarity_ = b_polarity;
}

void TB6612::setPWMPeriod(float period_s)
{
  pwm_a_.period(period_s);
  pwm_b_.period(period_s);
}

Direction TB6612::handlePolarity(MotorPolarity polarity, Direction new_dir)
{
  Direction ret_val;

  if (STANDARD == polarity)
  {
    ret_val = new_dir;
  }
  else if (FORWARD == new_dir)
  {
    ret_val = REVERSE;
  }
  else if (REVERSE == new_dir)
  {
    ret_val = FORWARD;
  }
  else
  {
    MBED_ASSERT(false);
  }

  return ret_val;
}

void TB6612::setDirection(MotorId motor, Direction new_dir)
{
  Direction dir;

  if (MOTOR_A == motor)
  {
    dir = handlePolarity(a_polarity_, new_dir);
    motor_a_dir_ = new_dir;
  }
  else if (MOTOR_B == motor)
  {
    dir = handlePolarity(b_polarity_, new_dir);
    motor_b_dir_ = new_dir;
  }
  else
  {
    MBED_ASSERT(false);
  }
  switch (dir)
  {
    case FORWARD:
      switch (motor)
      {
        case MOTOR_A:
          a_in1_ = 1;
          a_in2_ = 0;
          break;
        case MOTOR_B:
          b_in1_ = 1;
          b_in2_ = 0;
          break;
        default:
          MBED_ASSERT(false);
      }
      break;
    case REVERSE:
      switch (motor)
      {
        case MOTOR_A:
          a_in1_ = 0;
          a_in2_ = 1;
          break;
        case MOTOR_B:
          b_in1_ = 0;
          b_in2_ = 1;
          break;
        default:
          MBED_ASSERT(false);
      }
      break;
    default:
      MBED_ASSERT(false);
  }
}

Direction TB6612::getDirection(MotorId motor)
{
  Direction dir;

  switch (motor)
  {
    case MOTOR_A:
      dir = motor_a_dir_;
      break;
    case MOTOR_B:
      dir = motor_b_dir_;
      break;
    default:
      MBED_ASSERT(false);
  }

  return (dir);
}

void TB6612::setDutyCycle(MotorId motor, uint8_t percent)
{
  switch (motor)
  {
    case MOTOR_A:
      pwm_a_ = percent / 100.0f;
      break;
    case MOTOR_B:
      pwm_b_ = percent / 100.0f;
      break;
    default:
      MBED_ASSERT(false);
  }
}

void TB6612::stop(MotorId motor)
{
  switch (motor)
  {
    case MOTOR_A:
      a_in1_ = 0;
      a_in2_ = 0;
      break;
    case MOTOR_B:
      b_in1_ = 0;
      b_in2_ = 0;
      break;
    default:
      MBED_ASSERT(false);
  }
}

void TB6612::freewheel(void)
{
  stop(MOTOR_A);
  stop(MOTOR_B);
  setDutyCycle(MOTOR_A, 100);
  setDutyCycle(MOTOR_B, 100);
}

}  // namespace tb6612
