// Fill out your copyright notice in the Description page of Project Settings.


#include "SrvCallbacks/SpawnProMeshServer.h"
#include "world_control_msgs/msgs/Tag.h"
#include "Conversions.h"
#include "world_control_msgs/msgs/PhysicsProperties.h"


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnProMeshServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
    TSharedPtr<FROSSpawnProMeshSrv::Request> Request =
        MakeShareable(new FROSSpawnProMeshSrv::Request());
    Request->FromJson(JsonObject);
    return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnProMeshServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
    TSharedPtr<FROSSpawnProMeshSrv::Request> SpawnProMeshRequest =
        StaticCastSharedPtr<FROSSpawnProMeshSrv::Request>(Request);

    //UProceduralMeshComponent* ProMesh = NewObject<UProceduralMeshComponent>();
    FActorSpawnParameters SpawnParams;
    ABoundingBox* SpawnedActor;

    //now Spawn it

    double start=FPlatformTime::Seconds();
    FGraphEventRef Task=FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
    {
        SpawnedActor=this->SpawnProMesh(SpawnProMeshRequest);
        if(SpawnedActor)
        {ServiceSuccess=true;}
        else
        {ServiceSuccess=false;}
    },TStatId(),nullptr,ENamedThreads::GameThread);

    //wait for code above to complete (Spawning in GameThread)
    FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
    double end= FPlatformTime::Seconds();
    UE_LOG(LogTemp, Display, TEXT("SpawnRobot executed in %f seconds."), end-start);
    UE_LOG(LogTemp, Display, TEXT("SpawnRobot has the Name %s"), *SpawnedActor->GetName());
    UE_LOG(LogTemp,Display,TEXT("Spawned Robot has the ID %s"),*FString::FromInt(SpawnedActor->GetUniqueID()));



    return MakeShareable<FROSBridgeSrv::SrvResponse>
        (new FROSSpawnProMeshSrv::Response(/*Params.Id, FinalActorName, ServiceSuccess*/));
}

ABoundingBox* FROSSpawnProMeshServer::SpawnProMesh(TSharedPtr<FROSSpawnProMeshSrv::Request> SpawnProMeshRequest)
{

    //Check if World is avialable
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s]: Couldn't find the World."), *FString(__FUNCTION__));
        return nullptr;
    }
    ABoundingBox* SpawnedActor=NewObject<ABoundingBox>();
    //    //Code what to do :D
    TArray<FVector> Vertices; //Points
    TArray<int32> Triangles;
    TArray<FVector>Normals;
    TArray<FVector2D>UVs;
    float height=SpawnProMeshRequest->GetHeight();
    float width=SpawnProMeshRequest->GetWidth();
    float depth=SpawnProMeshRequest->GetDepth();

    //Assumption --> Boundingboxes are always square
    //We want the Mesh starting point to be in the middel of the Object so we will -1/2*height width depth to each of it
    float h=-height/2;  //heigh emphasis helper
    float w=-width/2;   //width emphasis helper
    float d=-depth/2;   //depth emphasis helper

    //
    //From Playerstart Camera Postion
    Vertices.Add(FVector(d,w,h)); //FrontBottomRight 0                                          //From Rotation (0|0|0) FrontButtomLeft
    Vertices.Add(FVector(d,w+width,h)); //BackBottomRight 1                                       //From Rotation (0|0|0) FrontButtomRight
    Vertices.Add(FVector(d+depth,w,h)); //FrontBottomLeft 2                                       //From Rotation (0|0|0) BackButtomLeft
    Vertices.Add(FVector(d+depth,w+width,h)); //BackBottomLeft 3                                    //From Rotation (0|0|0) BackButtonRight

    Vertices.Add(FVector(d,w,h+height)); //FrontTopRight 4                                        //From Rotation (0|0|0) FrontTopLeft
    Vertices.Add(FVector(d,w+width,h+height)); //BackTopRight 5                                     //From Rotation (0|0|0) FrontTopRight
    Vertices.Add(FVector(d+depth,w,h+height)); //FrontTopLeft 6                                     //From Rotation (0|0|0) BackTopLeft
    Vertices.Add(FVector(d+depth,w+width,h+height)); //BackTopLeft 7                                  //From Rotation (0|0|0) BackTopRight

    //Bottom
    Triangles.Add(2);
    Triangles.Add(3);
    Triangles.Add(0);

    Triangles.Add(3);
    Triangles.Add(1);
    Triangles.Add(0);

    //Right
    Triangles.Add(0);
    Triangles.Add(1);
    Triangles.Add(4);

    Triangles.Add(5);
    Triangles.Add(4);
    Triangles.Add(1);

    //Back
    Triangles.Add(5);
    Triangles.Add(1);
    Triangles.Add(3);

    Triangles.Add(3);
    Triangles.Add(7);
    Triangles.Add(5);

    //Left
    Triangles.Add(3);
    Triangles.Add(2);
    Triangles.Add(6);

    Triangles.Add(3);
    Triangles.Add(6);
    Triangles.Add(7);

    //Top
    Triangles.Add(6);
    Triangles.Add(4);
    Triangles.Add(7);

    Triangles.Add(7);
    Triangles.Add(4);
    Triangles.Add(5);

    //Front
    Triangles.Add(2);
    Triangles.Add(0);
    Triangles.Add(6);

    Triangles.Add(6);
    Triangles.Add(0);
    Triangles.Add(5);


    UVs.Add(FVector2D(0,0));
    UVs.Add(FVector2D(0,1));
    UVs.Add(FVector2D(1,0));
    UVs.Add(FVector2D(1,1));

    FVector SpawnLocation=FConversions::ROSToU(SpawnProMeshRequest->GetPose().GetPosition().GetVector());
    FRotator SpawnRotation= FRotator(FConversions::ROSToU(SpawnProMeshRequest->GetPose().GetOrientation().GetQuat()));
    FActorSpawnParameters SpawnParams;

    FString Label = SpawnProMeshRequest->GetActorLabel();

    //Check if Id is used already
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsWithTag(World, FName(*SpawnProMeshRequest->GetId()), Actors);

    if (!Actors.IsValidIndex(0))
    {
        //Actual Spawning MeshComponent
        SpawnedActor = World->SpawnActor<ABoundingBox>(SpawnLocation, SpawnRotation, SpawnParams);

        //SpawnedActor->ProMesh->CreateMeshSection(1,Vertices,Triangles,Normals,UVs,TArray<FColor>(),TArray<FProcMeshTangent>(),true); //create Mesh? needs to be in Game Thread?
        //// Needs to be movable if the game is running.
        //SpawnedActor->ProMesh->SetMobility(EComponentMobility::Movable);
        ////Assigning the Mesh and Material to the Component
        //SpawnedActor->ProMesh->SetupAttachment(SpawnedActor->GetRootComponent());
        //UE_LOG(LogTemp, Warning, TEXT("[%s]: Attached to new RootComponent."), *FString(__FUNCTION__));
        //SpawnedActor->ProMesh->SetSimulatePhysics(SpawnProMeshRequest->GetPhysicsProperties().IsSimulatePhysics());
        //SpawnedActor->ProMesh->SetGenerateOverlapEvents(SpawnProMeshRequest->GetPhysicsProperties().GetGenerateOverlapEvents());
        //SpawnedActor->ProMesh->SetEnableGravity(SpawnProMeshRequest->GetPhysicsProperties().GetGravity());
        //SpawnedActor->ProMesh->SetMassOverrideInKg(NAME_None,SpawnProMeshRequest->GetPhysicsProperties().GetMass());

        //SpawnedActor->ProMesh->SetMobility(SpawnProMeshRequest->GetPhysicsProperties().GetMobility());
    }
    else
    {
        //ID is already taken
        UE_LOG(LogTemp, Error, TEXT("[%s]: Semlog id: \"%s\" is not unique, therefore nothing was spawned."), *FString(__FUNCTION__), *SpawnProMeshRequest->GetId());
        return nullptr;
    }

    //Id tag to Actor

    SpawnedActor->Tags.Add(FName(*SpawnProMeshRequest->GetId()));

    FString FinalActorName = SpawnedActor->GetName();

    return SpawnedActor;

}
