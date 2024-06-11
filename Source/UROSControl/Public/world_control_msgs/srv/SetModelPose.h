#pragma once
#include "ROSBridgeSrv.h"
#include "geometry_msgs/Pose.h"

class UROSBRIDGE_API FROSSetModelPoseSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSSetModelPoseSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString Id;
		geometry_msgs::Pose Pose;


	public:
		Request() {}

		Request(FString InId, geometry_msgs::Pose InPose)
		{
			Id = InId;
			Pose = InPose;
		}

		FString GetId()
		{
			return Id;
		}

		geometry_msgs::Pose GetPose()
		{
			return Pose;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Id = JsonObject->GetStringField("id");
			Pose.FromJson(JsonObject->GetObjectField("pose"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSSetModelPoseSrv:Request {id = " + Id +
				", pose = " + Pose.ToString() + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("id"), Id);
			Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
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
			return "FROSSetModelPoseSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};
