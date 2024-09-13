#include "RobotDescription/RDJointAxis.h"
#include "RobotDescription/RDJoint.h"

URDJointAxis::URDJointAxis()
{
  Xyz = FVector(0.0, 0.0, 1.0);
}

void URDJointAxis::SetLowerLimitFrom(float LowerLimit)
{
  Lower = LowerLimit;
}

void URDJointAxis::SetUpperLimitFrom(float UpperLimit)
{
  Upper = UpperLimit;
}
