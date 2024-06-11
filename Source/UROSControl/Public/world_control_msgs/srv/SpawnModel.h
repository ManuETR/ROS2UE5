#pragma once
#include "ROSBridgeSrv.h"
#include "geometry_msgs/Pose.h"
#include "world_control_msgs/msgs/Tag.h"
#include "world_control_msgs/msgs/PhysicsProperties.h"

class UROSBRIDGE_API FROSSpawnModelSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSSpawnModelSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString Name;
		geometry_msgs::Pose Pose;
		FString Id;
		TArray<world_control_msgs::Tag> Tags;
		FString Path;
		FString ActorLabel;
		FString OverrideName;
		world_control_msgs::PhysicsProperties PhysicsProperties;
		TArray<FString> MaterialNames;
		TArray<FString> MaterialPaths;
		FString ParentId;
		bool bSpawnCollisionCheck;


	public:
		Request() {}

		Request(FString InName, geometry_msgs::Pose InPose, FString InId, TArray<world_control_msgs::Tag> InTags, FString InPath, FString InActorLabel, FString InOverrideName, world_control_msgs::PhysicsProperties InPhysicsProperties, TArray<FString> InMaterialNames, TArray<FString> InMaterialPaths, FString InParentId, bool bInSpawnCollisionCheck)
		{
			Name = InName;
			Pose = InPose;
			Id = InId;
			Tags = InTags;
			Path = InPath;
			ActorLabel = InActorLabel;
			OverrideName = InOverrideName;
			PhysicsProperties = InPhysicsProperties;
			MaterialNames = InMaterialNames;
			MaterialPaths = InMaterialPaths;
			ParentId = InParentId;
			bSpawnCollisionCheck = bInSpawnCollisionCheck;
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

		FString GetOverrideName()
		{
			return OverrideName;
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

		FString GetParentId()
		{
			return ParentId;
		}

		bool GetSpawnCollisionCheck()
		{
			return bSpawnCollisionCheck;
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
			ActorLabel = JsonObject->GetStringField("actor_label");
			OverrideName = JsonObject->GetStringField("override_name");
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

			ParentId = JsonObject->GetStringField("parent_id");
			bSpawnCollisionCheck = JsonObject->GetBoolField("spawn_collision_check");
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSSpawnModelSrv:Request {name = " + Name +
				", pose = " + Pose.ToString() +
				", id = " + Id +
				", tags size = " + FString::FromInt(Tags.Num()) +
				", path = " + Path +
				", actor_label = " + ActorLabel +
				", override_name = " + OverrideName +
				", physics_properties = " + PhysicsProperties.ToString() +
				", material_names size = " + FString::FromInt(MaterialNames.Num()) +
				", material_paths size = " + FString::FromInt(MaterialPaths.Num()) +
				", parent_id = " + ParentId + 
				", spawn_collision_check = " + (bSpawnCollisionCheck ? "true" : "false") + "}";
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
			Object->SetStringField(TEXT("override_name"), OverrideName);
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

			Object->SetStringField(TEXT("parent_id"), ParentId);
			Object->SetBoolField("spawn_collision_check", bSpawnCollisionCheck);
			return Object;
		}

	};

	class Response : public SrvResponse
	{
	private:
		FString Id;
		FString Name;
		FString ErrType;
		bool Success;


	public:
		Response() {}

		Response(FString InId, FString InName, FString InErrorType, bool InSuccess)
		{
			Id = InId;
			Name = InName;
			ErrType = InErrorType;
			Success = InSuccess;
		}

		FString GetId()
		{
			return Id;
		}

		FString GetName()
		{
			return Name;
		}

		FString GetErrorType()
		{
			return ErrType;
		}

		bool GetSuccess()
		{
			return Success;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Id = JsonObject->GetStringField("id");
			Name = JsonObject->GetStringField("name");
			ErrType = JsonObject->GetStringField("etype");
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
			return "FROSSpawnModelSrv:Response {id = " + Id +
				", name = " + Name + ", success = " + (Success ? FString("True") : FString("False")) + ", ErrorType = " + ErrType + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("id"), Id);
			Object->SetStringField(TEXT("name"), Name);
			Object->SetStringField(TEXT("etype"), ErrType);
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};
