// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "ROSBridgeMsg.h"
#include "geometry_msgs/Pose.h"
#include "Tag.h"
#include "PhysicsProperties.h"

namespace world_control_msgs
{
	class ModelDescription : public FROSBridgeMsg
	{
		FString Name;
		geometry_msgs::Pose Pose;
		FString Id;
		TArray<world_control_msgs::Tag> Tags;
		FString Path;
		FString ActorLabel;
		world_control_msgs::PhysicsProperties PhysicsProperties;
		TArray<FString> MaterialNames;
		TArray<FString> MaterialPaths;


	public:
		ModelDescription() {}

		ModelDescription(FString InName, geometry_msgs::Pose InPose, FString InId, TArray<world_control_msgs::Tag> InTags, FString InPath, FString InActorLabel, int InMobility, world_control_msgs::PhysicsProperties InPhysicsProperties, TArray<FString> InMaterialNames, TArray<FString> InMaterialPaths)
		{
			ModelDescription();
			Name = InName;
			Pose = InPose;
			Id = InId;
			Tags = InTags;
			Path = InPath;
			ActorLabel = InActorLabel;
			PhysicsProperties = InPhysicsProperties;
			MaterialNames = InMaterialNames;
			MaterialPaths = InMaterialPaths;
		}

		FString GetName()
		{
			return Name;
		}

		geometry_msgs::Pose GetPose()
		{
			return Pose;
		}

		FString GetId()
		{
			return Id;
		}

		TArray<world_control_msgs::Tag> GetTags()
		{
			return Tags;
		}

		FString GetPath()
		{
			return Path;
		}

		FString GetActorLabel()
		{
			return ActorLabel;
		}

		
		world_control_msgs::PhysicsProperties GetPhysicsProperties()
		{
			return PhysicsProperties;
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
			Name = JsonObject->GetStringField("name");
			Pose.FromJson(JsonObject->GetObjectField("pose"));
			Id = JsonObject->GetStringField("id");
			Tags.Empty();
			TArray<TSharedPtr<FJsonValue>> TagsPtrArray = JsonObject->GetArrayField(TEXT("tags"));
			for (auto &ptr : TagsPtrArray)
			{
				world_control_msgs::Tag Entry;
				Entry.FromJson(ptr->AsObject());
				Tags.Add(Entry);
			}

			Path = JsonObject->GetStringField("path");
			PhysicsProperties.FromJson(JsonObject->GetObjectField("physics_properties"));
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

		static ModelDescription GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			ModelDescription Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return "ModelDescription {name = " + Name +
				", pose = " + Pose.ToString() +
				", id = " + Id +
				", tags size = " + FString::FromInt(Tags.Num()) +
				", path = " + Path +
				", actor_label = " + ActorLabel +
				", physics_properties = " + PhysicsProperties.ToString() +
				", material_names size = " + FString::FromInt(MaterialNames.Num()) +
				", material_paths size = " + FString::FromInt(MaterialPaths.Num()) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("name"), Name);
			Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
			Object->SetStringField(TEXT("id"), Id);
			TArray<TSharedPtr<FJsonValue>> TagsPtrArray;
			for (auto &Entry : Tags)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Entry.ToJsonObject()));
				TagsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("tags", TagsPtrArray);

			Object->SetStringField(TEXT("path"), Path);
			Object->SetStringField(TEXT("actor_label"), ActorLabel);
			Object->SetObjectField(TEXT("physics_properties"), PhysicsProperties.ToJsonObject());
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

		virtual FString ToYamlString() const override
		{
			FString OutputString;
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(ToJsonObject().ToSharedRef(), Writer);
			return OutputString;
		}
	};
}
