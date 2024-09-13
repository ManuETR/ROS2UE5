#include "RobotDescription/RDCollision.h"

// Constructor
URDCollision::URDCollision()
{
	Geometry = CreateDefaultSubobject<URDGeometry>(TEXT("Geometry"));
}
