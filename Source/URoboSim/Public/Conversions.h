// Copyright 2017-2020, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once
#include "CoreMinimal.h"
#include "EngineUtils.h"

/**
* Unit and coordinate conversion helper functions
*/
struct FConversions
{
	/**********************************************
	*             Unit conversions
	**********************************************/

	// Conversion by value
	template <class T>
	static FORCEINLINE T CmToM(const T& In)
	{
		return In * 0.01f;
	}

	//// Conversion by reference
	//template <class T>
	//static FORCEINLINE void CmToM(T& Out)
	//{
	//	Out *= 0.01f;
	//}

	// Conversion by value
	template <class T>
	static FORCEINLINE T MToCm(const T& In)
	{
		return In * 100.f;
	}

	//// Conversion by reference
	//template <class T>
	//static FORCEINLINE void MToCm(T& Out)
	//{
	//	Out *= 100.f;
	//}

	/**********************************************
	*          Coordinate conversions
	**********************************************/

	/**
	 * Convert
	 * Unreal's 'Z up', 'X forward', 'Y right' 'left handed' coordinate system
	 * to
	 * ROS's 'Z up', 'X forward', 'Y left' 'right handed' coordinate system
	 *
	 * http://www.ros.org/reps/rep-0103.html
	 */

	// FTransform by value
	static FORCEINLINE FTransform UToROS(const FTransform& InTransform)
	{
		return FTransform(UToROS(InTransform.GetRotation()), UToROS(InTransform.GetLocation()));
	}

	// FQuat by value
	static FORCEINLINE FQuat UToROS(const FQuat& InQuat)
	{
		return FQuat(- InQuat.X, InQuat.Y, - InQuat.Z, InQuat.W);
	}

	// FVector by value
	static FORCEINLINE FVector UToROS(const FVector& InVector)
	{
		return CmToM(FVector(InVector.X, -InVector.Y, InVector.Z));
	}


	// FTransform by reference
	static FORCEINLINE void UToROS(FTransform& OutTransform)
	{
		OutTransform.SetRotation(UToROS(OutTransform.GetRotation()));
		OutTransform.SetLocation(UToROS(OutTransform.GetLocation()));
	}

	// FQuat by reference
	static FORCEINLINE void UToROS(FQuat& OutQuat)
	{
                OutQuat.X *= -1;
		OutQuat.W *= -1;
	}

	// FVector by reference
	static FORCEINLINE void UToROS(FVector& OutVector)
	{
		OutVector.Y *= -1;
		CmToM(OutVector);
	}

	/**
	* Convert
	* ROS's 'Z up', 'X forward', 'Y left' 'right handed' coordinate system
	* to
	* Unreal's 'Z up', 'X forward', 'Y right' 'left handed' coordinate system
	*
	* http://www.ros.org/reps/rep-0103.html
	*/

	// FTransform by value
	static FORCEINLINE FTransform ROSToU(const FTransform& InTransform)
	{
		return FTransform(ROSToU(InTransform.GetRotation()), ROSToU(InTransform.GetLocation()), InTransform.GetScale3D());
	}

	// FQuat by value
	static FORCEINLINE FQuat ROSToU(const FQuat& InQuat)
	{
		return FQuat(- InQuat.X, InQuat.Y, - InQuat.Z, InQuat.W);
	}

	// FRotator by value
  static FORCEINLINE FQuat ROSToU(const float& InRoll, const float& InPitch,const float& InYaw)
        {
          double cy = FMath::Cos(InYaw * 0.5);
          double sy = FMath::Sin(InYaw * 0.5);
          double cp = FMath::Cos(InPitch * 0.5);
          double sp = FMath::Sin(InPitch * 0.5);
          double cr = FMath::Cos(InRoll * 0.5);
          double sr = FMath::Sin(InRoll * 0.5);

          const FQuat q(sr * cp * cy - cr * sp * sy,
                        cr * sp * cy + sr * cp * sy,
                        cr * cp * sy - sr * sp * cy,
                        cr * cp * cy + sr * sp * sy);

          return ROSToU(q);
	}

	// FVector by value
	static FORCEINLINE FVector ROSToU(const FVector& InVector)
	{
		return MToCm(FVector(InVector.X, -InVector.Y, InVector.Z));
	}

	// FTransform by reference
	static FORCEINLINE void ROSToU(FTransform& OutTransform)
	{
		OutTransform.SetRotation(ROSToU(OutTransform.GetRotation()));
		OutTransform.SetLocation(ROSToU(OutTransform.GetLocation()));
	}

	// FQuat by reference
	static FORCEINLINE void ROSToU(FQuat& OutQuat)
	{
		OutQuat.X *= -1;
		OutQuat.W *= -1;
	}

	// FVector by reference
	static FORCEINLINE void ROSToU(FVector& OutVector)
	{
		OutVector.Y *= -1;
		MToCm(OutVector);
	}


	/**
	 * Convert
	 * Unreal's 'Z up', 'X forward', 'Y right' 'left handed' coordinate system
	 * to
	 * ROS's Camera 'Z forward', 'X right', 'Y down' 'right handed' coordinate system
	 *
	 * http://www.ros.org/reps/rep-0103.html
	 */

	 // FTransform by value
	static FORCEINLINE FTransform UToROSCamera(const FTransform& InTransform)
	{
		// TODO, dummy
		return FTransform(UToROSCamera(InTransform.GetRotation()), UToROSCamera(InTransform.GetLocation()));

	}

	// FQuat by value
	static FORCEINLINE FQuat UToROSCamera(const FQuat& InQuat)
	{
		// TODO, dummy
		return FQuat( - InQuat.X, InQuat.Y, - InQuat.Z, InQuat.W);
	}

	// FVector by value
	static FORCEINLINE FVector UToROSCamera(const FVector& InVector)
	{
		// TODO
		return CmToM(FVector(InVector.X, -InVector.Y, InVector.Z));
	}

	// FTransform by reference
	static FORCEINLINE void UToROSCamera(FTransform& OutTransform)
	{
		// TODO
		OutTransform.SetRotation(UToROSCamera(OutTransform.GetRotation()));
		OutTransform.SetLocation(UToROSCamera(OutTransform.GetLocation()));
	}

	// FQuat by reference
	static FORCEINLINE void UToROSCamera(FQuat& OutQuat)
	{
		// TODO
		OutQuat.X *= -1;
		OutQuat.W *= -1;
	}

	// FVector by reference
	static FORCEINLINE void UToROSCamera(FVector& OutVector)
	{
		OutVector.Y *= -1;
		MToCm(OutVector);
	}

	/**
	* Convert
	* ROS's Camera 'Z forward', 'X right', 'Y down' 'right handed' coordinate system
	* to
	* Unreal's 'Z up', 'X forward', 'Y right' 'left handed' coordinate system
	*
	* http://www.ros.org/reps/rep-0103.html
	*/

	// FTransform by value
	static FORCEINLINE FTransform ROSCameraToU(const FTransform& InTransform)
	{
		// TODO, dummy
		return FTransform(ROSCameraToU(InTransform.GetRotation()), ROSCameraToU(InTransform.GetLocation()));
	}

	// FQuat by value
	static FORCEINLINE FQuat ROSCameraToU(const FQuat& InQuat)
	{
		// TODO, dummy
		return FQuat( - InQuat.X, InQuat.Y, - InQuat.Z, InQuat.W);
	}

	// FVector by value
	static FORCEINLINE FVector ROSCameraToU(const FVector& InVector)
	{
		// TODO, dummy
		return MToCm(FVector(InVector.X, - InVector.Y, InVector.Z));
	}

	// FTransform by reference
	static FORCEINLINE void ROSCameraToU(FTransform& OutTransform)
	{
		// TODO, dummy
		OutTransform.SetRotation(ROSCameraToU(OutTransform.GetRotation()));
		OutTransform.SetLocation(ROSCameraToU(OutTransform.GetLocation()));
	}

	// FQuat by reference
	static FORCEINLINE void ROSCameraToU(FQuat& OutQuat)
	{
		// TODO, dummy
		OutQuat.X *= -1;
		OutQuat.W *= -1;
	}

	// FVector by reference
	static FORCEINLINE void ROSCameraToU(FVector& OutVector)
	{
		OutVector.Y *= -1;
		MToCm(OutVector);
	}
};
