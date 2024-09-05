#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalID.h"
#include "control_msgs/FollowJointTrajectoryGoal.h"

namespace control_msgs
{
	namespace action {

		class FollowJointTrajectory_SendGoal : public FROSBridgeMsg
		{
			std_msgs::Header Header;
			actionlib_msgs::GoalID GoalId;
			control_msgs::FollowJointTrajectoryGoal Goal;
		public:
			FollowJointTrajectory_SendGoal()
			{
				MsgType = "control_msgs/action/FollowJointTrajectory_SendGoal";
			}

			FollowJointTrajectory_SendGoal
			(
				std_msgs::Header InHeader,
				actionlib_msgs::GoalID InGoalId,
				control_msgs::FollowJointTrajectoryGoal InGoal
			):
				Header(InHeader),
				GoalId(InGoalId),
				Goal(InGoal)
			{
				MsgType = "control_msgs/action/FollowJointTrajectory_SendGoal";
			}

			~FollowJointTrajectory_SendGoal() override {}

			std_msgs::Header GetHeader() const
			{
				return Header;
			}

			actionlib_msgs::GoalID GetGoalId() const
			{
				return GoalId;
			}

			control_msgs::FollowJointTrajectoryGoal GetGoal() const
			{
				return Goal;
			}

			void SetHeader(std_msgs::Header InHeader)
			{
				Header = InHeader;
			}

			void SetGoalId(actionlib_msgs::GoalID InGoalId)
			{
				GoalId = InGoalId;
			}

			void SetGoal(control_msgs::FollowJointTrajectoryGoal InGoal)
			{
				Goal = InGoal;
			}

			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

				GoalId = actionlib_msgs::GoalID::GetFromJson(JsonObject->GetObjectField(TEXT("goal_id")));

				Goal = control_msgs::FollowJointTrajectoryGoal::GetFromJson(JsonObject->GetObjectField(TEXT("goal")));

			}

			static FollowJointTrajectory_SendGoal GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				FollowJointTrajectory_SendGoal Result;
				Result.FromJson(JsonObject);
				return Result;
			}

			virtual TSharedPtr<FJsonObject> ToJsonObject() const override
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				Object->SetObjectField(TEXT("header"), Header.ToJsonObject());
				Object->SetObjectField(TEXT("goal_id"), GoalId.ToJsonObject());
				Object->SetObjectField(TEXT("goal"), Goal.ToJsonObject());
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