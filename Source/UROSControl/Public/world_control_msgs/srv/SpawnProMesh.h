#pragma once
#include "ROSBridgeSrv.h"
#include "geometry_msgs/Pose.h"
#include "world_control_msgs/msgs/Tag.h"
#include "world_control_msgs/msgs/PhysicsProperties.h"


class UROSBRIDGE_API FROSSpawnProMeshSrv : public FROSBridgeSrv
{
        protected:
            FString Type;

        public:
            FROSSpawnProMeshSrv(FString InType)
            {
                Type = InType;
            }

            class Request : public SrvRequest
            {
            private:
                float Height;
                float Width;
                float Depth;
                geometry_msgs::Pose Pose;
                FString Id;
                TArray<world_control_msgs::Tag> Tags;
                FString ActorLabel;
                world_control_msgs::PhysicsProperties PhysicsProperties;
                FString ParentId;


            public:
                Request() {}

                Request(float InHeight,float InWidth, float InDepth, geometry_msgs::Pose InPose, FString InId, TArray<world_control_msgs::Tag> InTags, FString InActorLabel, world_control_msgs::PhysicsProperties InPhysicsProperties, FString InParentId)
                {
                    Height=InHeight;
                    Width=InWidth;
                    Depth=InDepth;
                    Pose = InPose;
                    Id = InId;
                    Tags = InTags;
                    ActorLabel = InActorLabel;
                    PhysicsProperties = InPhysicsProperties;
                    ParentId = InParentId;
                }

                float GetHeight()
                {
                    return Height;
                }

                float GetWidth()
                {
                    return Width;
                }

                float GetDepth()
                {
                    return Depth;
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

                FString GetActorLabel()
                {
                    return ActorLabel;
                }

                world_control_msgs::PhysicsProperties GetPhysicsProperties()
                {
                    return PhysicsProperties;
                }

                FString GetParentId()
                {
                    return ParentId;
                }

                virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
                {
                    Height=JsonObject->GetNumberField("height");
                    Width=JsonObject->GetNumberField("width");
                    Depth=JsonObject->GetNumberField("depth");
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
                    ActorLabel = JsonObject->GetStringField("actor_label");
                    PhysicsProperties.FromJson(JsonObject->GetObjectField("physics_properties"));
                    ParentId = JsonObject->GetStringField("parent_id");
                }

                static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
                {
                    Request Req;
                    Req.FromJson(JsonObject);
                    return Req;
                }

                FString ToString() const override
                {
                    return "FROSSpawnModelSrv:Request {Height = " + FString::SanitizeFloat(Height) +
                        ", Width = " + FString::SanitizeFloat(Width) +
                        ", Depth = " + FString::SanitizeFloat(Depth) +
                        ", pose = " + Pose.ToString() +
                        ", id = " + Id +
                        ", tags size = " + FString::FromInt(Tags.Num()) +
                        ", actor_label = " + ActorLabel +
                        ", physics_properties = " + PhysicsProperties.ToString() +
                        ", parent_id = " + ParentId + "}";
                }

                virtual TSharedPtr<FJsonObject> ToJsonObject() const override
                {
                    TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
                    Object->SetNumberField(TEXT("height"),Height);
                    Object->SetNumberField(TEXT("width"),Width);
                    Object->SetNumberField(TEXT("depth"),Depth);
                    Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
                    Object->SetStringField(TEXT("id"), Id);
                    TArray<TSharedPtr<FJsonValue>> TagsPtrArray;
                    for (auto &Entry : Tags)
                    {
                        TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Entry.ToJsonObject()));
                        TagsPtrArray.Add(Ptr);
                    }
                    Object->SetArrayField("tags", TagsPtrArray);
                    Object->SetStringField(TEXT("actor_label"), ActorLabel);
                    Object->SetObjectField(TEXT("physics_properties"), PhysicsProperties.ToJsonObject());
                    Object->SetStringField(TEXT("parent_id"), ParentId);
                    return Object;
                }

            };

            class Response : public SrvResponse
            {
            private:
                FString Id;
                FString Name;
                bool Success;


            public:
                Response() {}

                Response(FString InId, FString InName, bool InSuccess)
                {
                    Id = InId;
                    Name = InName;
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

                bool GetSuccess()
                {
                    return Success;
                }

                virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
                {
                    Id = JsonObject->GetStringField("id");
                    Name = JsonObject->GetStringField("Name");
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
                        ", name = " + Name + ", success = " + (Success ? FString("True") : FString("False")) + "}";
                }

                virtual TSharedPtr<FJsonObject> ToJsonObject() const override
                {
                    TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
                    Object->SetStringField(TEXT("id"), Id);
                    Object->SetStringField(TEXT("name"), Name);
                    Object->SetBoolField(TEXT("success"), Success);
                    return Object;
                }

            };

        };
