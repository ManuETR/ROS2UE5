#pragma once
#include "ROSBridgeSrv.h"

class UROSBRIDGE_API FROSAttachModelToParentSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSAttachModelToParentSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString ParentId;
		FString ChildId;


	public:
		Request() {}

		Request(FString InParentId, FString InChildId)
		{
			ParentId = InParentId;
			ChildId = InChildId;
		}

		FString GetParentId()
		{
			return ParentId;
		}

		FString GetChildId()
		{
			return ChildId;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			ParentId = JsonObject->GetStringField("parent_id");
			ChildId = JsonObject->GetStringField("child_id");
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSAttachModelToParentSrv:Request {parent_id = " + ParentId +
				", child_id = " + ChildId + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("parent_id"), ParentId);
			Object->SetStringField(TEXT("child_id"), ChildId);
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
			return "FROSAttachModelToParentSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};