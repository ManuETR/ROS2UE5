#pragma once
#include "ROSBridgeSrv.h"
#include "world_control_msgs/msgs/ModelDescription.h"
#include "world_control_msgs/msgs/ConstraintDescription.h"
#include "world_control_msgs/msgs/RelationDescription.h"


class UROSBRIDGE_API FROSSpawnSemanticMapSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSSpawnSemanticMapSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		TArray<world_control_msgs::ModelDescription> Models;
		TArray<world_control_msgs::ConstraintDescription> Constraints;
		TArray<world_control_msgs::RelationDescription> Relations;


	public:
		Request() {}

		Request(TArray<world_control_msgs::ModelDescription> InModels, TArray<world_control_msgs::ConstraintDescription> InConstraints, TArray<world_control_msgs::RelationDescription> InRelations)
		{
			Models = InModels;
			Constraints = InConstraints;
			Relations = InRelations;
		}

		TArray<world_control_msgs::ModelDescription> GetModels()
		{
			return Models;
		}

		TArray<world_control_msgs::ConstraintDescription> GetConstraints()
		{
			return Constraints;
		}

		TArray<world_control_msgs::RelationDescription> GetRelations()
		{
			return Relations;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Models.Empty();
			TArray<TSharedPtr<FJsonValue>> ModelsPtrArray = JsonObject->GetArrayField(TEXT("models"));
			for (auto &ptr : ModelsPtrArray)
			{
				world_control_msgs::ModelDescription Entry;
				Entry.FromJson(ptr->AsObject());
				Models.Add(Entry);
			}

			Constraints.Empty();
			TArray<TSharedPtr<FJsonValue>> ConstraintsPtrArray = JsonObject->GetArrayField(TEXT("constraints"));
			for (auto &ptr : ConstraintsPtrArray)
			{
				world_control_msgs::ConstraintDescription Entry;
				Entry.FromJson(ptr->AsObject());
				Constraints.Add(Entry);
			}

			Relations.Empty();
			TArray<TSharedPtr<FJsonValue>> RelationsPtrArray = JsonObject->GetArrayField(TEXT("relations"));
			for (auto &ptr : RelationsPtrArray)
			{
				world_control_msgs::RelationDescription Entry;
				Entry.FromJson(ptr->AsObject());
				Relations.Add(Entry);
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
			return "FROSSpawnSemanticMapSrv:Request {models size = " + FString::FromInt(Models.Num()) +
				", constraints size = " + FString::FromInt(Constraints.Num()) +
				", relations size = " + FString::FromInt(Relations.Num()) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			TArray<TSharedPtr<FJsonValue>> ModelsPtrArray;
			for (auto &Entry : Models)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Entry.ToJsonObject()));
				ModelsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("models", ModelsPtrArray);

			TArray<TSharedPtr<FJsonValue>> ConstraintsPtrArray;
			for (auto &Entry : Constraints)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Entry.ToJsonObject()));
				ConstraintsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("constraints", ConstraintsPtrArray);

			TArray<TSharedPtr<FJsonValue>> RelationsPtrArray;
			for (auto &Entry : Relations)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Entry.ToJsonObject()));
				RelationsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("relations", RelationsPtrArray);

			return Object;
		}

	};

	class Response : public SrvResponse
	{
	private:
		bool Success;
		TArray<FString> FailedModelIds;
		TArray<FString> FailedConstraintIds;
		TArray<world_control_msgs::RelationDescription> FailedRelations;


	public:
		Response() {}

		Response(bool InSuccess, TArray<FString> InFailedModelIds, TArray<FString> InFailedConstraintIds, TArray<world_control_msgs::RelationDescription> InFailedRelations)
		{
			Success = InSuccess;
			FailedModelIds = InFailedModelIds;
			FailedConstraintIds = InFailedConstraintIds;
			FailedRelations = InFailedRelations;
		}

		bool GetSuccess()
		{
			return Success;
		}

		TArray<FString> GetFailedModelIds()
		{
			return FailedModelIds;
		}

		TArray<FString> GetFailedConstraintIds()
		{
			return FailedConstraintIds;
		}

		TArray<world_control_msgs::RelationDescription> GetFailedRelations()
		{
			return FailedRelations;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Success = JsonObject->GetBoolField("success");
			FailedModelIds.Empty();
			TArray<TSharedPtr<FJsonValue>> FailedModelIdsPtrArray = JsonObject->GetArrayField(TEXT("failed_model_ids"));
			for (auto &ptr : FailedModelIdsPtrArray)
			{
				FString Entry = ptr->AsString();
				FailedModelIds.Add(Entry);
			}

			FailedConstraintIds.Empty();
			TArray<TSharedPtr<FJsonValue>> FailedConstraintIdsPtrArray = JsonObject->GetArrayField(TEXT("failed_constraint_ids"));
			for (auto &ptr : FailedConstraintIdsPtrArray)
			{
				FString Entry = ptr->AsString();
				FailedConstraintIds.Add(Entry);
			}

			FailedRelations.Empty();
			TArray<TSharedPtr<FJsonValue>> FailedRelationsPtrArray = JsonObject->GetArrayField(TEXT("failed_relations"));
			for (auto &ptr : FailedRelationsPtrArray)
			{
				world_control_msgs::RelationDescription Entry;
				Entry.FromJson(ptr->AsObject());
				FailedRelations.Add(Entry);
			}

		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Response Res;
			Res.FromJson(JsonObject);
			return Res;
		}

		FString ToString() const override
		{
			return "FROSSpawnSemanticMapSrv:Response {success = " + (Success ? FString("True") : FString("False")) +
				", failed_model_ids size = " + FString::FromInt(FailedModelIds.Num()) +
				", failed_constraint_ids size = " + FString::FromInt(FailedConstraintIds.Num()) +
				", failed_relations size = " + FString::FromInt(FailedRelations.Num()) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			TArray<TSharedPtr<FJsonValue>> FailedModelIdsPtrArray;
			for (auto &Entry : FailedModelIds)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueString(Entry));
				FailedModelIdsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("failed_model_ids", FailedModelIdsPtrArray);

			TArray<TSharedPtr<FJsonValue>> FailedConstraintIdsPtrArray;
			for (auto &Entry : FailedConstraintIds)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueString(Entry));
				FailedConstraintIdsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("failed_constraint_ids", FailedConstraintIdsPtrArray);

			TArray<TSharedPtr<FJsonValue>> FailedRelationsPtrArray;
			for (auto &Entry : FailedRelations)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Entry.ToJsonObject()));
				FailedRelationsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("failed_relations", FailedRelationsPtrArray);

			return Object;
		}

	};

};
