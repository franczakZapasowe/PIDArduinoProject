#include "PID.h"
float PIDController::calculate(float Sp, float Pv, float dt) {
    float error = Sp - Pv;
    float P = error * kp_;   //człon P
    float I = last_I_ + (ki_ * error * dt);  //człon I
    if (I > anti_windup_max_) I = anti_windup_max_;
    else if (I < anti_windup_min_) I = anti_windup_min_;
    last_I_ = I;
    float D;
    if (dt == 0) D = 0;
    else
        D = kd_ * (error - last_error_)/dt; // człon D
    last_error_ = error;
    //filtr gornoprzepustowy dla d
    return P + I + D;
}
