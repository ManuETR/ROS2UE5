// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "SrvCallbacks/SpawnPhysicsConstraintServer.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "ConstraintSpawner.h"
#include "Conversions.h"


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnPhysicsConstraintServer::FromJson(
	TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSpawnPhysicsConstraintSrv::Request> Request =
		MakeShareable(new FROSSpawnPhysicsConstraintSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnPhysicsConstraintServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSpawnPhysicsConstraintSrv::Request> SpawnPhysicsConstraintRequest =
		StaticCastSharedPtr<FROSSpawnPhysicsConstraintSrv::Request>(Request);

	// Setup liniear limits
	FConstraintSpawner::FLinearLimits LinLimits;
	world_control_msgs::LinearLimits LinLimitsMsg = SpawnPhysicsConstraintRequest->GetConstraintDescription().GetConstraintDetails().GetLinearLimits();

	LinLimits.XMotion = LinLimitsMsg.GetXMotion();
	LinLimits.YMotion = LinLimitsMsg.GetYMotion();
	LinLimits.ZMotion = LinLimitsMsg.GetZMotion();
	LinLimits.Limit = LinLimitsMsg.GetLimit();
	LinLimits.UseAdvanced = LinLimitsMsg.GetUseAdvanced();
	LinLimits.SoftConstraint = LinLimitsMsg.GetSoftConstraint();
	LinLimits.Stiffness = LinLimitsMsg.GetStiffness();
	LinLimits.Damping = LinLimitsMsg.GetDamping();


	// Setup angular limits
	FConstraintSpawner::FAngularLimits AngLimits;
	world_control_msgs::AngularLimits AngLimitsMsg = SpawnPhysicsConstraintRequest->GetConstraintDescription().GetConstraintDetails().GetAngularLimits();

	AngLimits.Swing1Motion = AngLimitsMsg.GetSwing1Motion();
	AngLimits.Swing2Motion = AngLimitsMsg.GetSwing2Motion();
	AngLimits.TwistMotion = AngLimitsMsg.GetTwistMotion();
	AngLimits.Swing1LimitAngle = AngLimitsMsg.GetSwing1LimitAngle();
	AngLimits.Swing2LimitAngle = AngLimitsMsg.GetSwing2LimitAngle();
	AngLimits.TwistLimitAngle = AngLimitsMsg.GetTwistLimitAngle();
	AngLimits.AngularRoationOffset = AngLimitsMsg.GetAngularRoationOffset().GetVector();
	AngLimits.UseAdvanced = AngLimitsMsg.GetUseAdvanced();
	AngLimits.SwingSoftConstraint = AngLimitsMsg.GetSwingSoftConstraint();
	AngLimits.SwingStiffness = AngLimitsMsg.GetSwingStiffness();
	AngLimits.SwingDamping = AngLimitsMsg.GetSwingDamping();
	AngLimits.TwistSoftConstraint = AngLimitsMsg.GetTwistSoftConstraint();
	AngLimits.TwistStiffness = AngLimitsMsg.GetTwistStiffness();
	AngLimits.TwistDamping = AngLimitsMsg.GetTwistDamping();


	// Setup Constraint details
	FConstraintSpawner::FPhysicsConstraintDetails Details;
	world_control_msgs::PhysicsConstraintDetails DetailsMsg = SpawnPhysicsConstraintRequest->GetConstraintDescription().GetConstraintDetails();

	Details.IdFirstModel = DetailsMsg.GetIdFirstModel();
	Details.IdSecondModel = DetailsMsg.GetIdSecondModel();
	Details.DisableCollision = DetailsMsg.GetDisableCollision();
	Details.EnableProjection = DetailsMsg.GetEnableProjection();
	Details.ProjectionLinearTolerance = DetailsMsg.GetProjectionLinearTolerance();
	Details.ProjectionAngularTolerance = DetailsMsg.GetProjectionAngularTolerance();
	Details.ParentDominates = DetailsMsg.GetParentDominates();
	Details.LinearLimits = LinLimits;
	Details.AngularLimits = AngLimits;
	   

	FVector Location = FConversions::ROSToU(SpawnPhysicsConstraintRequest->GetConstraintDescription().GetPose().GetPosition().GetVector());
	FRotator Rotator = FRotator(FConversions::ROSToU(SpawnPhysicsConstraintRequest->GetConstraintDescription().GetPose().GetOrientation().GetQuat()));


	//Execute spawning on GameThreade
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		ServiceSuccess = FConstraintSpawner::SpawnPhysicsConstraintActor(World, SpawnPhysicsConstraintRequest->GetConstraintDescription().GetId(), Details, Location, Rotator);
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return TSharedPtr<FROSBridgeSrv::SrvResponse>
		(new FROSSpawnPhysicsConstraintSrv::Response(ServiceSuccess));

}
