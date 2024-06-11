// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "ROSBridgeMsg.h"
#include "PhysicsConstraintDetails.h"
#include "geometry_msgs/Pose.h"

namespace world_control_msgs
{
	class ConstraintDescription : public FROSBridgeMsg
	{
		FString Id;
		world_control_msgs::PhysicsConstraintDetails ConstraintDetails;
		geometry_msgs::Pose Pose;


	public:
		ConstraintDescription() {}

		ConstraintDescription(FString InId, world_control_msgs::PhysicsConstraintDetails InConstraintDetails, geometry_msgs::Pose InPose)
		{
			ConstraintDescription();
			Id = InId;
			ConstraintDetails = InConstraintDetails;
			Pose = InPose;
		}

		FString GetId()
		{
			return Id;
		}

		world_control_msgs::PhysicsConstraintDetails GetConstraintDetails()
		{
			return ConstraintDetails;
		}

		geometry_msgs::Pose GetPose()
		{
			return Pose;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Id = JsonObject->GetStringField("id");
			ConstraintDetails.FromJson(JsonObject->GetObjectField("constraint_details"));
			Pose.FromJson(JsonObject->GetObjectField("pose"));
		}

		static ConstraintDescription GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			ConstraintDescription Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return "ConstraintDescription {id = " + Id +
				", constraint_details = " + ConstraintDetails.ToString() +
				", pose = " + Pose.ToString() + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("id"), Id);
			Object->SetObjectField(TEXT("constraint_details"), ConstraintDetails.ToJsonObject());
			Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
			return Object;
		}

		virtual FString ToYamlString() const override
		{
			FString OutputString;
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(ToJsonObject().ToSharedRef(), Writer);
			return OutputString;
		}
	};
}
