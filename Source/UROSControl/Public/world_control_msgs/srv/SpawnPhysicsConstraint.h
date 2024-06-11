#pragma once
#include "ROSBridgeSrv.h"
#include "world_control_msgs/msgs/PhysicsConstraintDetails.h"
#include "geometry_msgs/Pose.h"
#include "world_control_msgs/msgs/ConstraintDescription.h"


class UROSBRIDGE_API FROSSpawnPhysicsConstraintSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSSpawnPhysicsConstraintSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		world_control_msgs::ConstraintDescription ConstraintDescription;


	public:
		Request() {}

		Request(world_control_msgs::ConstraintDescription InConstraintDescription)
		{
			ConstraintDescription = InConstraintDescription;
		}

	

		world_control_msgs::ConstraintDescription GetConstraintDescription()
		{
			return ConstraintDescription;
		}


		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			ConstraintDescription.FromJson(JsonObject->GetObjectField("constraint_description"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSSpawnPhysicsConstraintSrv:Request {" + ConstraintDescription.ToString() + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField(TEXT("constraint_description"), ConstraintDescription.ToJsonObject());
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
			return "FROSSpawnPhysicsConstraintSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};
