// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "ROSBridgeMsg.h"

namespace world_control_msgs
{
	class PhysicsProperties : public FROSBridgeMsg
	{

		uint8 Mobility;
		bool Gravity;
		bool GenerateOverlapEvents;
		float Mass;


	public:
		// static enum  MobilityTypes { Static, Stationary, Dynamic, Kinematic};
		enum  MobilityTypes { Static, Stationary, Dynamic, Kinematic};

		PhysicsProperties() {}

		PhysicsProperties( bool InGravity, bool InGenerateOverlapEvents, float InMass)
		{
			PhysicsProperties();
			Gravity = InGravity;
			GenerateOverlapEvents = InGenerateOverlapEvents;
			Mass = InMass;
		}


		EComponentMobility::Type GetMobility()
		{
			switch(Mobility)
			{
				case Static:
					return EComponentMobility::Static;
				case Stationary:
					return  EComponentMobility::Stationary;
				case Dynamic:
				case Kinematic:
					return  EComponentMobility::Movable;
				default:
					return EComponentMobility::Static;
			}
		}

		bool GetGravity()
		{
			return Gravity;
		}

		bool GetGenerateOverlapEvents()
		{
			return GenerateOverlapEvents;
		}

		bool IsSimulatePhysics()
		{
			return Mobility == Dynamic;
		}


		float GetMass()
		{
			return Mass;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Mobility = JsonObject->GetNumberField("mobility");
			Gravity = JsonObject->GetBoolField("gravity");
			GenerateOverlapEvents = JsonObject->GetBoolField("generate_overlap_events");
			Mass = JsonObject->GetNumberField("mass");
		}

		static PhysicsProperties GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PhysicsProperties Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return "PhysicsProperties {Mobility: " + FString::FromInt(Mobility) +
				"gravity = " + (Gravity ? FString("True") : FString("False")) +
				", generate_overlap_events = " + (GenerateOverlapEvents ? FString("True") : FString("False")) +
				", mass = " + FString::SanitizeFloat(Mass) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetNumberField(TEXT("mobility"), Mobility);
			Object->SetBoolField(TEXT("gravity"), Gravity);
			Object->SetBoolField(TEXT("generate_overlap_events"), GenerateOverlapEvents);
			Object->SetNumberField(TEXT("mass"), Mass);
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
