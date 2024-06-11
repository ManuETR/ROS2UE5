// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "RWCManager.h"
#include "SrvCallbacks/SpawnModelsServer.h"
#include "SrvCallbacks/SetModelPoseServer.h"
#include "SrvCallbacks/GetModelPoseServer.h"
#include "SrvCallbacks/RemoveModelServer.h"
#include "SrvCallbacks/AttachModelToParentServer.h"
#include "SrvCallbacks/SpawnPhysicsConstraintServer.h"
#include "SrvCallbacks/HighlightModelsServer.h"
#include "SrvCallbacks/SetPhysicsPropertiesServer.h"
#include "SrvCallbacks/SetMaterialServer.h"
#include "SrvCallbacks/SpawnSemanticMapServer.h"
#include "SrvCallbacks/DeleteAllServer.h"
#include "SrvCallbacks/ObjectToObjectStateServer.h"
#include "SrvCallbacks/GetModelSocketPoseServer.h"

void URWCManager::Register(FString DefaultNamespace, UWorld* InWorld)
{
	Namespace = DefaultNamespace;
        if(InWorld)
          {
            World = InWorld;
          }
        else
          {
            World = GetWorld();
          }

	if(!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: GetWorld returned, NULL."), *FString(__FUNCTION__));
		return;
	}

	SetupServiceServers();
}


void URWCManager::SetupServiceServers()
{
	// Add Service Servers

	ServicesToPublish.Add(MakeShareable<FROSSpawnModelServer>(new FROSSpawnModelServer(Namespace, TEXT("spawn_model"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSSetModelPoseServer>(new FROSSetModelPoseServer(Namespace, TEXT("set_model_pose"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSGetModelPoseServer>(new FROSGetModelPoseServer(Namespace, TEXT("get_model_pose"), World, this)));
        ServicesToPublish.Add(MakeShareable<FROSGetModelSocketPoseServer>(new FROSGetModelSocketPoseServer(Namespace, TEXT("get_model_socket_pose"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSRemoveModelServer>(new FROSRemoveModelServer(Namespace, TEXT("delete_model"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSAttachModelToParentServer>(new FROSAttachModelToParentServer(Namespace, TEXT("attach_model_to_parent"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSSpawnPhysicsConstraintServer>(new FROSSpawnPhysicsConstraintServer(Namespace, TEXT("spawn_physics_constraint"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSHighlightModelsServer>(new FROSHighlightModelsServer(Namespace, TEXT("highlight_models"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSSetPhysicsPropertiesServer>(new FROSSetPhysicsPropertiesServer(Namespace, TEXT("set_physics_properties"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSSetMaterialServer>(new FROSSetMaterialServer(Namespace, TEXT("change_material"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSSpawnSemanticMapServer>(new FROSSpawnSemanticMapServer(Namespace, TEXT("spawn_semantic_map"), World, this)));
//    ServicesToPublish.Add(MakeShareable<FROSSpawnRobotServer>(new FROSSpawnRobotServer(Namespace,TEXT("spawn_robot"),World,this)));
	ServicesToPublish.Add(MakeShareable<FROSDeleteAllServer>(new FROSDeleteAllServer(Namespace, TEXT("delete_all"), World, this)));
	ServicesToPublish.Add(MakeShareable<FROSObjectToObjectStateServer>(new FROSObjectToObjectStateServer(Namespace, TEXT("object_to_object_state"), World, this)));
}

void URWCManager::ConnectToHandler(const TSharedPtr<FROSBridgeHandler>& ROSHandler)
{
  for(auto& Service : ServicesToPublish)
    {
      ROSHandler->AddServiceServer(Service);
    }

  for(auto& Publisher : PublisherToPublish)
    {
      ROSHandler->AddPublisher(Publisher);
    }

  for(auto& Subscriber : SubscriberToPublish)
    {
      ROSHandler->AddSubscriber(Subscriber);
    }
}
