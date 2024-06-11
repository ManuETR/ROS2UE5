#pragma once
#include "ROSBridgeSrv.h"
#include "geometry_msgs/Pose.h"

class UROSBRIDGE_API FROSGetModelSocketPoseSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSGetModelSocketPoseSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString Id;

		FString Socket;


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
		
		FString GetSocket()                     
																																																																																																																																																																																																																			
		{
			return Socket;
		}


		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Id = JsonObject->GetStringField("id");
			Socket = JsonObject->GetStringField("socket");
		}

		

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSGetModelSocketPoseSrv:Request {id = " + Id +
				", socket = " + Socket + "}";
		}



		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("id"), Id);
			Object->SetStringField(TEXT("socket"), Socket);
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
			return "FROSGetModelSocketPoseSrv:Response {success = " + (Success ? FString("True") : FString("False")) + 
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
