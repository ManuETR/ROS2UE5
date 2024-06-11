// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "ROSBridgeMsg.h"

namespace world_control_msgs
{
	class RelationDescription : public FROSBridgeMsg
	{
		FString ParentId;
		FString ChildId;


	public:
		RelationDescription() {}

		RelationDescription(FString InParentId, FString InChildId)
		{
			RelationDescription();
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

		static RelationDescription GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			RelationDescription Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return "RelationDescription {parent_id = " + ParentId +
				", child_id = " + ChildId + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("parent_id"), ParentId);
			Object->SetStringField(TEXT("child_id"), ChildId);
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