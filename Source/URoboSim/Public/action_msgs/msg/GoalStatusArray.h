#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "action_msgs/msg/GoalStatus.h"

namespace action_msgs
{
	namespace msg {

		class GoalStatusArray : public FROSBridgeMsg
		{
			std_msgs::Header Header;
			TArray<action_msgs::msg::GoalStatus> StatusList;
		public:
			GoalStatusArray()
			{
				MsgType = "action_msgs/msg/GoalStatusArray";
			}

			GoalStatusArray
			(
				std_msgs::Header InHeader,
				const TArray<action_msgs::msg::GoalStatus>& InStatusList
			):
				Header(InHeader),
				StatusList(InStatusList)
			{
				MsgType = "action_msgs/msg/GoalStatusArray";
			}

			~GoalStatusArray() override {}

			std_msgs::Header GetHeader() const
			{
				return Header;
			}

			TArray<action_msgs::msg::GoalStatus> GetStatusList() const
			{
				return StatusList;
			}

			void SetHeader(std_msgs::Header InHeader)
			{
				Header = InHeader;
			}

			void SetStatusList(TArray<action_msgs::msg::GoalStatus>& InStatusList)
			{
				StatusList = InStatusList;
			}

			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

				TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;

				StatusList.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("status_list"));
				for (auto &ptr : ValuesPtrArr)
					StatusList.Add(action_msgs::msg::GoalStatus::GetFromJson(ptr->AsObject()));

			}

			static GoalStatusArray GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				GoalStatusArray Result;
				Result.FromJson(JsonObject);
				return Result;
			}

			virtual TSharedPtr<FJsonObject> ToJsonObject() const override
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				Object->SetObjectField(TEXT("header"), Header.ToJsonObject());
				TArray<TSharedPtr<FJsonValue>> StatusListArray;
				for (auto &val : StatusList)
					StatusListArray.Add(MakeShareable(new FJsonValueObject(val.ToJsonObject())));
				Object->SetArrayField(TEXT("status_list"), StatusListArray);
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
}