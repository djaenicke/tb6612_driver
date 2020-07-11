#include "tb6612.h"

namespace tb6612 {

TB6612::TB6612(PinName pwm_a, PinName pwm_b, PinName a_in1, \
           PinName a_in2, PinName b_in1, PinName b_in2,
           Motor_Polarity_T a_polarity, Motor_Polarity_T b_polarity):
           pwm_a_(pwm_a), pwm_b_(pwm_b), a_in1_(a_in1),
           a_in2_(a_in2), b_in1_(b_in1), b_in2_(b_in2) {
  a_polarity_ = a_polarity;
  b_polarity_ = b_polarity;
}

void TB6612::SetPWMPeriod(float period_s) {
    pwm_a_.period(period_s);
    pwm_b_.period(period_s);
}

Direction_T TB6612:: HandlePolarity(Motor_Polarity_T polarity, Direction_T new_dir) {
  Direction_T ret_val;

  if (STANDARD == polarity) {
    ret_val = new_dir;
  } else if (FORWARD == new_dir) {
    ret_val = REVERSE;
  } else if (REVERSE == new_dir) {
    ret_val = FORWARD;
  } else {
    MBED_ASSERT(false);
  }

  return ret_val;
}

void TB6612::SetDirection(Motor_Id_T motor, Direction_T new_dir) {
  Direction_T dir;

  if (MOTOR_A == motor) {
    dir = HandlePolarity(a_polarity_, new_dir);
    motor_a_dir_ = new_dir;
  } else if (MOTOR_B == motor) {
    dir = HandlePolarity(b_polarity_, new_dir);
    motor_b_dir_ = new_dir;
  } else {
    MBED_ASSERT(false);
  }
  switch (dir) {
    case FORWARD:
      switch (motor) {
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
      switch (motor) {
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

Direction_T TB6612::GetDirection(Motor_Id_T motor) {
  Direction_T dir;

  switch (motor) {
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

void TB6612::SetDC(Motor_Id_T motor, uint8_t percent) {
  switch (motor) {
    case MOTOR_A:
      pwm_a_ = percent/100.0f;
      break;
    case MOTOR_B:
      pwm_b_ = percent/100.0f;
      break;
    default:
      MBED_ASSERT(false);
  }
}

void TB6612::Stop(Motor_Id_T motor) {
  switch (motor) {
    case MOTOR_A:
      a_in1_ = 1;
      a_in2_ = 1;
      break;
    case MOTOR_B:
      b_in1_ = 1;
      b_in2_ = 1;
      break;
    default:
      MBED_ASSERT(false);
  }
}

void TB6612::Freewheel(void) {
  SetDC(MOTOR_A, 0);
  SetDC(MOTOR_B, 0);
}

}  // namespace tb6612
