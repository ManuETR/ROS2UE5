#pragma once
#include "ROSBridgeSrv.h"

class UROSBRIDGE_API FROSHighlightModelSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSHighlightModelSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString Id;
		uint8 Color;


	public:
		Request() {}

		Request(FString InId, uint8 InColor)
		{
			Id = InId;
			Color = InColor;
		}

		FString GetId()
		{
			return Id;
		}

		uint8 GetColor()
		{
			return Color;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Id = JsonObject->GetStringField("id");
			Color = JsonObject->GetNumberField("color");
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSHighlightModelSrv:Request {id = " + Id +
				", color = " + FString::FromInt(Color) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("id"), Id);
			Object->SetNumberField(TEXT("color"), Color);
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
			return "FROSHighlightModelSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};