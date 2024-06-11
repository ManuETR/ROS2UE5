#pragma once
#include "ROSBridgeSrv.h"

class UROSCONTROL_API FROSObjectToObjectSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSObjectToObjectSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString source_object_id;
		FString other_object_id;

	public:
		Request(){}

		Request(FString in_source_object_id, FString in_other_object_id) 
		{
			source_object_id = in_source_object_id;
			other_object_id = in_other_object_id;
		}

		FString GetSourceObjectId()
		{
			return source_object_id;
		}

		FString GetOtherObjectId()
		{
			return other_object_id;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			source_object_id = JsonObject->GetStringField("source_object_id");
			other_object_id = JsonObject->GetStringField("other_object_id");
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSObjectToObjectSrv:Request {source_object_id = " + source_object_id + ", other_object_id = " + other_object_id + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("source_object_id"), source_object_id);
			Object->SetStringField(TEXT("other_object_id"), other_object_id);
			return Object;
		}
	};

	class Response : public SrvResponse
	{
	private:
		FString State;
		bool Success;

	public:
		Response(){}

		Response(FString InState, bool InSuccess)
		{
			State = InState;
			Success = InSuccess;
		}

		FString GetState()
		{
			return State;
		}

		bool GetSuccess()
		{
			return Success;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			State = JsonObject->GetStringField("state");
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
			return "FRosObjectToObjectState:Response {state = " + State + ", success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("state"), State);
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}
	};
};