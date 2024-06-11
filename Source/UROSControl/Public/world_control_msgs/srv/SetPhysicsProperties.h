#pragma once
#include "ROSBridgeSrv.h"
#include "world_control_msgs/msgs/PhysicsProperties.h"

class UROSBRIDGE_API FROSSetPhysicsPropertiesSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSSetPhysicsPropertiesSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString Id;
		world_control_msgs::PhysicsProperties PhysicsProperties;


	public:
		Request() {}

		Request(FString InId, world_control_msgs::PhysicsProperties InPhysicsProperties)
		{
			Id = InId;
			PhysicsProperties = InPhysicsProperties;
		}

		FString GetId()
		{
			return Id;
		}

		world_control_msgs::PhysicsProperties GetPhysicsProperties()
		{
			return PhysicsProperties;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Id = JsonObject->GetStringField("id");
			PhysicsProperties.FromJson(JsonObject->GetObjectField("physics_properties"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSSetPhysicsPropertiesSrv:Request {id = " + Id +
				", physics_properties = " + PhysicsProperties.ToString() + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("id"), Id);
			Object->SetObjectField(TEXT("physics_properties"), PhysicsProperties.ToJsonObject());
			return Object;
		}

	};

	class Response : public SrvResponse
	{
	private:
		bool Success;


	public:
		Response() {}

		Response(bool InSuccess)
		{
			Success = InSuccess;
		}

		bool GetSuccess()
		{
			return Success;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Success = JsonObject->GetBoolField("success");
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Response Res;
			Res.FromJson(JsonObject);
			return Res;
		}

		FString ToString() const override
		{
			return "FROSSetPhysicsPropertiesSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};
