// Fill out your copyright notice in the Description page of Project Settings.


#include "BoundingBox.h"

#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h" //UMaterialInstanceDynamic


// Sets default values
ABoundingBox::ABoundingBox()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    //RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    //ProMesh=CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProMesh"));
    //Front=CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Front"));

    //ProMesh->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
    //Front->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
    //An Procedual Generated Bounding Box here is some example Code on how it could be used


//    //Testing Purpose have picture for item with AN124036 set the size values accordingly
//    TArray<FVector> Vertices; //Points
//    TArray<int32> Triangles;
//    TArray<FVector>Normals;
//    TArray<FVector2D>UVs;
//    float depth=6.7;
//    float width=13.8;
//    float height=16.2;
//    float d=-depth/2;
//    float w=-width/2;
//    float h=-height/2;

//    //
//    //From Playerstart Camera Postion
//    Vertices.Add(FVector(d,w,h)); //FrontBottomRight 0                                          //From Rotation (0|0|0) FrontButtomLeft
//    Vertices.Add(FVector(d,w+width,h)); //BackBottomRight 1                                       //From Rotation (0|0|0) FrontButtomRight
//    Vertices.Add(FVector(d+depth,w,h)); //FrontBottomLeft 2                                       //From Rotation (0|0|0) BackButtomLeft
//    Vertices.Add(FVector(d+depth,w+width,h)); //BackBottomLeft 3                                    //From Rotation (0|0|0) BackButtonRight

//    Vertices.Add(FVector(d,w,h+height)); //FrontTopRight 4                                        //From Rotation (0|0|0) FrontTopLeft
//    Vertices.Add(FVector(d,w+width,h+height)); //BackTopRight 5                                     //From Rotation (0|0|0) FrontTopRight
//    Vertices.Add(FVector(d+depth,w,h+height)); //FrontTopLeft 6                                     //From Rotation (0|0|0) BackTopLeft
//    Vertices.Add(FVector(d+depth,w+width,h+height)); //BackTopLeft 7                                  //From Rotation (0|0|0) BackTopRight

////    //Bottom
//    Triangles.Add(2);
//    Triangles.Add(3);
//    Triangles.Add(0);

//    Triangles.Add(3);
//    Triangles.Add(1);
//    Triangles.Add(0);

//    //Right
//    Triangles.Add(2);
//    Triangles.Add(0);
//    Triangles.Add(4);

//    Triangles.Add(2);
//    Triangles.Add(4);
//    Triangles.Add(6);

//    //Back
//    Triangles.Add(5);
//    Triangles.Add(1);
//    Triangles.Add(3);

//    Triangles.Add(3);
//    Triangles.Add(7);
//    Triangles.Add(5);

//    //Left
//    Triangles.Add(3);
//    Triangles.Add(2);
//    Triangles.Add(6);

//    Triangles.Add(3);
//    Triangles.Add(6);
//    Triangles.Add(7);

//    //Top
//    Triangles.Add(6);
//    Triangles.Add(4);
//    Triangles.Add(7);

//    Triangles.Add(7);
//    Triangles.Add(4);
//    Triangles.Add(5);

//    //Front
//    Triangles.Add(0);
//    Triangles.Add(1);
//    Triangles.Add(4);

//    Triangles.Add(1);
//    Triangles.Add(5);
//    Triangles.Add(4);


//    ProMesh->CreateMeshSection(1,Vertices,Triangles,Normals,UVs,TArray<FColor>(),TArray<FProcMeshTangent>(),true); //create Mesh? needs to be in Game Thread?

//    //Set a material
//    ConstructorHelpers::FObjectFinder<UMaterial>FoundMat(TEXT("Material'/Game/tmp/Invisible.Invisible'")); //Construction Helper only works in Constructor
//    if(FoundMat.Succeeded())
//    {
//        auto MaterialInstance = UMaterialInstanceDynamic::Create(FoundMat.Object,ProMesh);
//        ProMesh->SetMaterial(1,MaterialInstance);
//    }

//    //In some Usecases it could be interessting to have an specific Front defined here is some example code for it
//    //Front
//    TArray<FVector> FrontVertices; //Points
//    TArray<int32> FrontTriangles;
//    TArray<FVector>FrontNormals;
//    TArray<FVector2D>FrontUVs;

//    //
//    //From Playerstart Camera Postion
//    FrontVertices.Add(FVector(d,w,h)); //FrontBottomRight 0                                          //From Rotation (0|0|0) FrontButtomLeft
//    FrontVertices.Add(FVector(d,w+width,h)); //BackBottomRight 1                                       //From Rotation (0|0|0) FrontButtomRight
//    FrontVertices.Add(FVector(d,w,h+height)); //FrontTopRight 2                                        //From Rotation (0|0|0) FrontTopLeft
//    FrontVertices.Add(FVector(d,w+width,h+height)); //BackTopRight 3                                     //From Rotation (0|0|0) FrontTopRight

//    FrontTriangles.Add(0);
//    FrontTriangles.Add(1);
//    FrontTriangles.Add(2);

//    FrontTriangles.Add(2);
//    FrontTriangles.Add(1);
//    FrontTriangles.Add(3);

//    FrontUVs.Add(FVector2D(0,1));
//    FrontUVs.Add(FVector2D(1,1));
//    FrontUVs.Add(FVector2D(0,0));
//    FrontUVs.Add(FVector2D(1,0));

//    Front->CreateMeshSection(1,FrontVertices,FrontTriangles,FrontNormals,FrontUVs,TArray<FColor>(),TArray<FProcMeshTangent>(),true);


}

// Called when the game starts or when spawned
void ABoundingBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoundingBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

