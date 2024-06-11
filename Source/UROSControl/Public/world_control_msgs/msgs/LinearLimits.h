// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "ROSBridgeMsg.h"

namespace world_control_msgs
{
	class LinearLimits : public FROSBridgeMsg
	{
		uint8 XMotion;
		uint8 YMotion;
		uint8 ZMotion;
		float Limit;
		bool UseAdvanced;
		bool SoftConstraint;
		float Stiffness;
		float Damping;


	public:
		LinearLimits() {}

		LinearLimits(uint8 InXMotion, uint8 InYMotion, uint8 InZMotion, float InLimit, bool InUseAdvanced, bool InSoftConstraint, float InStiffness, float InDamping)
		{
			LinearLimits();
			XMotion = InXMotion;
			YMotion = InYMotion;
			ZMotion = InZMotion;
			Limit = InLimit;
			UseAdvanced = InUseAdvanced;
			SoftConstraint = InSoftConstraint;
			Stiffness = InStiffness;
			Damping = InDamping;
		}

		uint8 GetXMotion()
		{
			return XMotion;
		}

		uint8 GetYMotion()
		{
			return YMotion;
		}

		uint8 GetZMotion()
		{
			return ZMotion;
		}

		float GetLimit()
		{
			return Limit;
		}

		bool GetUseAdvanced()
		{
			return UseAdvanced;
		}

		bool GetSoftConstraint()
		{
			return SoftConstraint;
		}

		float GetStiffness()
		{
			return Stiffness;
		}

		float GetDamping()
		{
			return Damping;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			XMotion = JsonObject->GetNumberField("x_motion");
			YMotion = JsonObject->GetNumberField("y_motion");
			ZMotion = JsonObject->GetNumberField("z_motion");
			Limit = JsonObject->GetNumberField("limit");
			UseAdvanced = JsonObject->GetBoolField("use_advanced");
			SoftConstraint = JsonObject->GetBoolField("soft_constraint");
			Stiffness = JsonObject->GetNumberField("stiffness");
			Damping = JsonObject->GetNumberField("damping");
		}

		static LinearLimits GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			LinearLimits Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return "LinearLimits {x_motion = " + FString::FromInt(XMotion) +
				", y_motion = " + FString::FromInt(YMotion) +
				", z_motion = " + FString::FromInt(ZMotion) +
				", limit = " + FString::SanitizeFloat(Limit) +
				", use_advanced = " + (UseAdvanced ? FString("True") : FString("False")) +
				", soft_constraint = " + (SoftConstraint ? FString("True") : FString("False")) +
				", stiffness = " + FString::SanitizeFloat(Stiffness) +
				", damping = " + FString::SanitizeFloat(Damping) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetNumberField(TEXT("x_motion"), XMotion);
			Object->SetNumberField(TEXT("y_motion"), YMotion);
			Object->SetNumberField(TEXT("z_motion"), ZMotion);
			Object->SetNumberField(TEXT("limit"), Limit);
			Object->SetBoolField(TEXT("use_advanced"), UseAdvanced);
			Object->SetBoolField(TEXT("soft_constraint"), SoftConstraint);
			Object->SetNumberField(TEXT("stiffness"), Stiffness);
			Object->SetNumberField(TEXT("damping"), Damping);
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