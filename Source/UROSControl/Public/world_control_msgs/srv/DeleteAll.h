#pragma once
#include "ROSBridgeSrv.h"

class UROSCONTROL_API FROSDeleteAllSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSDeleteAllSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString type_to_delete;
		FString key_to_delete;
		FString value_to_delete;
		bool ignore_value;

	public:
		Request() {}

		Request(FString in_type_to_delete, FString in_key_to_delete, FString in_value_to_delete, bool in_ignore_value)
		{
			type_to_delete = in_type_to_delete;
			key_to_delete = in_key_to_delete;
			value_to_delete = in_value_to_delete;
			ignore_value = in_ignore_value;
		}

		FString GetType()
		{
			return type_to_delete;
		}

		FString GetKey()
		{
			return key_to_delete;
		}

		FString GetValue()
		{
			return value_to_delete;
		}

		bool GetIgnore()
		{
			return ignore_value;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			type_to_delete = JsonObject->GetStringField("type_to_delete");
			key_to_delete = JsonObject->GetStringField("key_to_delete");
			value_to_delete = JsonObject->GetStringField("value_to_delete");
			ignore_value = JsonObject->GetBoolField("ignore_value");
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSDeleteAllSrv:Request {type_to_delete = }" + type_to_delete + ", key_to_delete = " + key_to_delete +
				", value_to_delete = " + value_to_delete + ", ignore_value = " + (ignore_value ? "True" : "False") + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("type_to_delete"), type_to_delete);
			Object->SetStringField(TEXT("key_to_delete"), key_to_delete);
			Object->SetStringField(TEXT("value_to_delete"), value_to_delete);
			Object->SetBoolField(TEXT("ignore_value"), ignore_value);
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
			return "FROSDeleteAllSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}
	};
};