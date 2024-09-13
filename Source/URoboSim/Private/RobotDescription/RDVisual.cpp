#include "RobotDescription/RDVisual.h"

URDVisual::URDVisual()
{
	Geometry = CreateDefaultSubobject<URDGeometry>(TEXT("Geometry"));
}
