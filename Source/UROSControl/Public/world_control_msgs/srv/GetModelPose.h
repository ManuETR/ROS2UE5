#pragma once
#include "ROSBridgeSrv.h"
#include "geometry_msgs/Pose.h"

class UROSBRIDGE_API FROSGetModelPoseSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSGetModelPoseSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString Id;
		// geometry_msgs::Pose Pose;


	public:
		Request() {}

		Request(FString InId)
		{
			Id = InId;
		}

		FString GetId()
		{
			return Id;
		}


		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Id = JsonObject->GetStringField("id");
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSGetModelPoseSrv:Request {id = " + Id + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("id"), Id);
			return Object;
		}

	};

	class Response : public SrvResponse
	{
	private:
		bool Success;
		geometry_msgs::Pose Pose;


	public:
		Response() {}

		Response(bool InSuccess, geometry_msgs::Pose InPose)
		{
			Success = InSuccess;
			Pose = InPose;
		}

		bool GetSuccess()
		{
			return Success;
		}

		geometry_msgs::Pose GetPose()
		{
			return Pose;
		}
		

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Success = JsonObject->GetBoolField("success");
			Pose.FromJson(JsonObject->GetObjectField("pose"));
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Response Res;
			Res.FromJson(JsonObject);
			return Res;
		}

		FString ToString() const override
		{
			return "FROSGetModelPoseSrv:Response {success = " + (Success ? FString("True") : FString("False")) + 
						 ", pose = " + Pose.ToString() + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
			return Object;
		}

	};

};
