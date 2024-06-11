// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "ROSBridgeMsg.h"

namespace world_control_msgs
{
	class Tag : public FROSBridgeMsg
	{
		FString Type;
		FString Key;
		FString Value;

	public:
		Tag() {}

		Tag(FString InType, FString InKey, FString InValue)
		{
			Tag();
			Type = InType;
			Key = InKey;
			Value = InValue;
		}

		FString GetType()
		{
			return Type;
		}

		FString GetKey()
		{
			return Key;
		}

		FString GetValue()
		{
			return Value;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Type = JsonObject->GetStringField("type");
			Key = JsonObject->GetStringField("key");
			Value = JsonObject->GetStringField("value");
		}

		static Tag GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Tag Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return "Tag {type = " + Type +
				", key = " + Key +
				", value = " + Value + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("type"), Type);
			Object->SetStringField(TEXT("key"), Key);
			Object->SetStringField(TEXT("value"), Value);
			return Object;
		}

		virtual FString ToYamlString() const override
		{
			FString OutputString;
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(ToJsonObject().ToSharedRef(), Writer);
			return OutputString;
		}
	};
}