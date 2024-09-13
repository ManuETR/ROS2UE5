#include "RobotDescription/RDLink.h"

URDLink::URDLink()
{
	bGravity = 1;
	bSelfCollide = 0;
	Inertial = CreateDefaultSubobject<URDLinkInertial>(TEXT("Inertial"));
}
