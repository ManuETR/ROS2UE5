#pragma once
#include "ROSBridgeSrv.h"

class UROSBRIDGE_API FROSSetMaterialSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSSetMaterialSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString Id;
		TArray<FString> MaterialNames;
		TArray<FString> MaterialPaths;


	public:
		Request() {}

		Request(FString InId, TArray<FString> InMaterialNames, TArray<FString> InMaterialPaths)
		{
			Id = InId;
			MaterialNames = InMaterialNames;
			MaterialPaths = InMaterialPaths;
		}

		FString GetId()
		{
			return Id;
		}

		TArray<FString> GetMaterialNames()
		{
			return MaterialNames;
		}

		TArray<FString> GetMaterialPaths()
		{
			return MaterialPaths;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Id = JsonObject->GetStringField("id");
			MaterialNames.Empty();
			TArray<TSharedPtr<FJsonValue>> MaterialNamesPtrArray = JsonObject->GetArrayField(TEXT("material_names"));
			for (auto &ptr : MaterialNamesPtrArray)
			{
				FString Entry = ptr->AsString();
				MaterialNames.Add(Entry);
			}

			MaterialPaths.Empty();
			TArray<TSharedPtr<FJsonValue>> MaterialPathsPtrArray = JsonObject->GetArrayField(TEXT("material_paths"));
			for (auto &ptr : MaterialPathsPtrArray)
			{
				FString Entry = ptr->AsString();
				MaterialPaths.Add(Entry);
			}

		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSSetMaterialSrv:Request {id = " + Id +
				", material_names size = " + FString::FromInt(MaterialNames.Num()) +
				", material_paths size = " + FString::FromInt(MaterialPaths.Num()) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("id"), Id);
			TArray<TSharedPtr<FJsonValue>> MaterialNamesPtrArray;
			for (auto &Entry : MaterialNames)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueString(Entry));
				MaterialNamesPtrArray.Add(Ptr);
			}
			Object->SetArrayField("material_names", MaterialNamesPtrArray);

			TArray<TSharedPtr<FJsonValue>> MaterialPathsPtrArray;
			for (auto &Entry : MaterialPaths)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueString(Entry));
				MaterialPathsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("material_paths", MaterialPathsPtrArray);

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
			return "FROSSetMaterialSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};