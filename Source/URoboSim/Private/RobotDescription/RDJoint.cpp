#include "RobotDescription/RDJoint.h"

// Constructor
URDJoint::URDJoint()
{
	Axis = CreateDefaultSubobject<URDJointAxis>(TEXT("Axis"));
}
