#pragma once
class PIDController {
    float kp_, ki_, kd_;
    float last_I_{}, last_error_{};
    float anti_windup_min_, anti_windup_max_;
public:
    PIDController(float kp, float ki, float kd, float anti_windup_min, float anti_windup_max)
    : kp_(kp), ki_(ki), kd_(kd), anti_windup_min_(anti_windup_min),anti_windup_max_(anti_windup_max)
    {}

    float calculate(float Sp, float Pv,float dt);
};