#pragma once
#include "ROSBridgeMsg.h"

namespace std_msgs
{
	class Header : public FROSBridgeMsg
	{
		FROSTime Stamp;
		FString FrameId;

	public:
		Header()
		{
			MsgType = "std_msgs/Header";
		}

		Header(
			FROSTime InStamp,
			FString InFrameId) :
			Stamp(InStamp),
			FrameId(InFrameId)
		{
			MsgType = "std_msgs/Header";
		}

		~Header() override {}



		FROSTime GetStamp() const
		{
			return Stamp;
		}

		FString GetFrameId() const
		{
			return FrameId;
		}


		void SetStamp(FROSTime InStamp)
		{
			Stamp = InStamp;
		}

		void SetFrameId(FString InFrameId)
		{
			FrameId = InFrameId;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override 
		{
			Stamp = FROSTime::GetFromJson(JsonObject->GetObjectField(TEXT("stamp")));
			FrameId = JsonObject->GetStringField(TEXT("frame_id"));
		}

		static Header GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Header Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return TEXT("Header { stamp = ") + Stamp.ToString() +
				TEXT(", frame_id = ") + FrameId + TEXT(" } ");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override 
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField(TEXT("stamp"), Stamp.ToJsonObject());
			Object->SetStringField(TEXT("frame_id"), FrameId);
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
