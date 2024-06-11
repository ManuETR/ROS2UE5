// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "ROSBridgeMsg.h"
#include "LinearLimits.h"
#include "AngularLimits.h"

namespace world_control_msgs
{
	class PhysicsConstraintDetails : public FROSBridgeMsg
	{
		FString IdFirstModel;
		FString IdSecondModel;
		bool DisableCollision;
		bool EnableProjection;
		float ProjectionLinearTolerance;
		float ProjectionAngularTolerance;
		bool ParentDominates;
		world_control_msgs::LinearLimits LinearLimits;
		world_control_msgs::AngularLimits AngularLimits;

	public:
		PhysicsConstraintDetails() {}

		PhysicsConstraintDetails(FString InIdFirstModel, FString InIdSecondModel, bool InDisableCollision, bool InEnableProjection, float InProjectionLinearTolerance, float InProjectionAngularTolerance, bool InParentDominates, world_control_msgs::LinearLimits InLinearLimits, world_control_msgs::AngularLimits InAngularLimits)
		{
			PhysicsConstraintDetails();
			IdFirstModel = InIdFirstModel;
			IdSecondModel = InIdSecondModel;
			DisableCollision = InDisableCollision;
			EnableProjection = InEnableProjection;
			ProjectionLinearTolerance = InProjectionLinearTolerance;
			ProjectionAngularTolerance = InProjectionAngularTolerance;
			ParentDominates = InParentDominates;
			LinearLimits = InLinearLimits;
			AngularLimits = InAngularLimits;
		}

		FString GetIdFirstModel()
		{
			return IdFirstModel;
		}

		FString GetIdSecondModel()
		{
			return IdSecondModel;
		}

		bool GetDisableCollision()
		{
			return DisableCollision;
		}

		bool GetEnableProjection()
		{
			return EnableProjection;
		}

		float GetProjectionLinearTolerance()
		{
			return ProjectionLinearTolerance;
		}

		float GetProjectionAngularTolerance()
		{
			return ProjectionAngularTolerance;
		}

		bool GetParentDominates()
		{
			return ParentDominates;
		}

		world_control_msgs::LinearLimits GetLinearLimits()
		{
			return LinearLimits;
		}

		world_control_msgs::AngularLimits GetAngularLimits()
		{
			return AngularLimits;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			IdFirstModel = JsonObject->GetStringField("id_first_model");
			IdSecondModel = JsonObject->GetStringField("id_second_model");
			DisableCollision = JsonObject->GetBoolField("disable_collision");
			EnableProjection = JsonObject->GetBoolField("enable_projection");
			ProjectionLinearTolerance = JsonObject->GetNumberField("projection_linear_tolerance");
			ProjectionAngularTolerance = JsonObject->GetNumberField("projection_angular_tolerance");
			ParentDominates = JsonObject->GetBoolField("parent_dominates");
			LinearLimits.FromJson(JsonObject->GetObjectField("linear_limits"));
			AngularLimits.FromJson(JsonObject->GetObjectField("angular_limits"));
		}

		static PhysicsConstraintDetails GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PhysicsConstraintDetails Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return "PhysicsConstraintDetails {id_first_model = " + IdFirstModel +
				", id_second_model = " + IdSecondModel +
				", disable_collision = " + (DisableCollision ? FString("True") : FString("False")) +
				", enable_projection = " + (EnableProjection ? FString("True") : FString("False")) +
				", projection_linear_tolerance = " + FString::SanitizeFloat(ProjectionLinearTolerance) +
				", projection_angular_tolerance = " + FString::SanitizeFloat(ProjectionAngularTolerance) +
				", parent_dominates = " + (ParentDominates ? FString("True") : FString("False")) +
				", linear_limits = " + LinearLimits.ToString() +
				", angular_limits = " + AngularLimits.ToString() + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("id_first_model"), IdFirstModel);
			Object->SetStringField(TEXT("id_second_model"), IdSecondModel);
			Object->SetBoolField(TEXT("disable_collision"), DisableCollision);
			Object->SetBoolField(TEXT("enable_projection"), EnableProjection);
			Object->SetNumberField(TEXT("projection_linear_tolerance"), ProjectionLinearTolerance);
			Object->SetNumberField(TEXT("projection_angular_tolerance"), ProjectionAngularTolerance);
			Object->SetBoolField(TEXT("parent_dominates"), ParentDominates);
			Object->SetObjectField(TEXT("linear_limits"), LinearLimits.ToJsonObject());
			Object->SetObjectField(TEXT("angular_limits"), AngularLimits.ToJsonObject());
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