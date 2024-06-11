// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "ROSBridgeMsg.h"
#include "geometry_msgs/Vector3.h"



namespace world_control_msgs
{
	class AngularLimits : public FROSBridgeMsg
	{
		uint8 Swing1Motion;
		uint8 Swing2Motion;
		uint8 TwistMotion;
		float Swing1LimitAngle;
		float Swing2LimitAngle;
		float TwistLimitAngle;
		geometry_msgs::Vector3 AngularRoationOffset;
		bool UseAdvanced;
		bool SwingSoftConstraint;
		float SwingStiffness;
		float SwingDamping;
		bool TwistSoftConstraint;
		float TwistStiffness;
		float TwistDamping;


	public:
		AngularLimits() {}

		AngularLimits(uint8 InSwing1Motion, uint8 InSwing2Motion, uint8 InTwistMotion, float InSwing1LimitAngle, float InSwing2LimitAngle, float InTwistLimitAngle, geometry_msgs::Vector3 InAngularRoationOffset, bool InUseAdvanced, bool InSwingSoftConstraint, float InSwingStiffness, float InSwingDamping, bool InTwistSoftConstraint, float InTwistStiffness, float InTwistDamping)
		{
			AngularLimits();
			Swing1Motion = InSwing1Motion;
			Swing2Motion = InSwing2Motion;
			TwistMotion = InTwistMotion;
			Swing1LimitAngle = InSwing1LimitAngle;
			Swing2LimitAngle = InSwing2LimitAngle;
			TwistLimitAngle = InTwistLimitAngle;
			AngularRoationOffset = InAngularRoationOffset;
			UseAdvanced = InUseAdvanced;
			SwingSoftConstraint = InSwingSoftConstraint;
			SwingStiffness = InSwingStiffness;
			SwingDamping = InSwingDamping;
			TwistSoftConstraint = InTwistSoftConstraint;
			TwistStiffness = InTwistStiffness;
			TwistDamping = InTwistDamping;
		}

		uint8 GetSwing1Motion()
		{
			return Swing1Motion;
		}

		uint8 GetSwing2Motion()
		{
			return Swing2Motion;
		}

		uint8 GetTwistMotion()
		{
			return TwistMotion;
		}

		float GetSwing1LimitAngle()
		{
			return Swing1LimitAngle;
		}

		float GetSwing2LimitAngle()
		{
			return Swing2LimitAngle;
		}

		float GetTwistLimitAngle()
		{
			return TwistLimitAngle;
		}

		geometry_msgs::Vector3 GetAngularRoationOffset()
		{
			return AngularRoationOffset;
		}

		bool GetUseAdvanced()
		{
			return UseAdvanced;
		}

		bool GetSwingSoftConstraint()
		{
			return SwingSoftConstraint;
		}

		float GetSwingStiffness()
		{
			return SwingStiffness;
		}

		float GetSwingDamping()
		{
			return SwingDamping;
		}

		bool GetTwistSoftConstraint()
		{
			return TwistSoftConstraint;
		}

		float GetTwistStiffness()
		{
			return TwistStiffness;
		}

		float GetTwistDamping()
		{
			return TwistDamping;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Swing1Motion = JsonObject->GetNumberField("swing_1_motion");
			Swing2Motion = JsonObject->GetNumberField("swing_2_motion");
			TwistMotion = JsonObject->GetNumberField("twist_motion");
			Swing1LimitAngle = JsonObject->GetNumberField("swing_1_limit_angle");
			Swing2LimitAngle = JsonObject->GetNumberField("swing_2_limit_angle");
			TwistLimitAngle = JsonObject->GetNumberField("twist_limit_angle");
			AngularRoationOffset.FromJson(JsonObject->GetObjectField("angular_roation_offset"));
			UseAdvanced = JsonObject->GetBoolField("use_advanced");
			SwingSoftConstraint = JsonObject->GetBoolField("swing_soft_constraint");
			SwingStiffness = JsonObject->GetNumberField("swing_stiffness");
			SwingDamping = JsonObject->GetNumberField("swing_damping");
			TwistSoftConstraint = JsonObject->GetBoolField("twist_soft_constraint");
			TwistStiffness = JsonObject->GetNumberField("twist_stiffness");
			TwistDamping = JsonObject->GetNumberField("twist_damping");
		}

		static AngularLimits GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			AngularLimits Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return "AngularLimits {swing_1_motion = " + FString::FromInt(Swing1Motion) +
				", swing_2_motion = " + FString::FromInt(Swing2Motion) +
				", twist_motion = " + FString::FromInt(TwistMotion) +
				", swing_1_limit_angle = " + FString::SanitizeFloat(Swing1LimitAngle) +
				", swing_2_limit_angle = " + FString::SanitizeFloat(Swing2LimitAngle) +
				", twist_limit_angle = " + FString::SanitizeFloat(TwistLimitAngle) +
				", angular_roation_offset = " + AngularRoationOffset.ToString() +
				", use_advanced = " + (UseAdvanced ? FString("True") : FString("False")) +
				", swing_soft_constraint = " + (SwingSoftConstraint ? FString("True") : FString("False")) +
				", swing_stiffness = " + FString::SanitizeFloat(SwingStiffness) +
				", swing_damping = " + FString::SanitizeFloat(SwingDamping) +
				", twist_soft_constraint = " + (TwistSoftConstraint ? FString("True") : FString("False")) +
				", twist_stiffness = " + FString::SanitizeFloat(TwistStiffness) +
				", twist_damping = " + FString::SanitizeFloat(TwistDamping) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetNumberField(TEXT("swing_1_motion"), Swing1Motion);
			Object->SetNumberField(TEXT("swing_2_motion"), Swing2Motion);
			Object->SetNumberField(TEXT("twist_motion"), TwistMotion);
			Object->SetNumberField(TEXT("swing_1_limit_angle"), Swing1LimitAngle);
			Object->SetNumberField(TEXT("swing_2_limit_angle"), Swing2LimitAngle);
			Object->SetNumberField(TEXT("twist_limit_angle"), TwistLimitAngle);
			Object->SetObjectField(TEXT("angular_roation_offset"), AngularRoationOffset.ToJsonObject());
			Object->SetBoolField(TEXT("use_advanced"), UseAdvanced);
			Object->SetBoolField(TEXT("swing_soft_constraint"), SwingSoftConstraint);
			Object->SetNumberField(TEXT("swing_stiffness"), SwingStiffness);
			Object->SetNumberField(TEXT("swing_damping"), SwingDamping);
			Object->SetBoolField(TEXT("twist_soft_constraint"), TwistSoftConstraint);
			Object->SetNumberField(TEXT("twist_stiffness"), TwistStiffness);
			Object->SetNumberField(TEXT("twist_damping"), TwistDamping);
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
