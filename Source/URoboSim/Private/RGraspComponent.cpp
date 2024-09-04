#include "RGraspComponent.h"
#include "Physics/RModel.h"
#include "Physics/RLink.h"
#include "ROSCommunication/Publisher/RTFPublisher.h"

URGraspComponent::URGraspComponent()
{
  InitSphereRadius(GraspRadius);
  SetGenerateOverlapEvents(true);
  SetEnableGravity(false);
}


UPrimitiveComponent* URGraspComponent::GetComponent(FString ComponentName) {
  UPrimitiveComponent* PrimComp = nullptr;
  for (UActorComponent* Comp : GetOwner()->GetComponents())
  {
    if (Comp->GetFName() == ComponentName)
    {
      if (UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Comp))
      {
        if (AActor* ChildActor = ChildActorComp->GetChildActor())
        {
          PrimComp = Cast<UPrimitiveComponent>(ChildActor->GetRootComponent());
        }
      }
      else
      {
        PrimComp = Cast<UPrimitiveComponent>(Comp);
      }
      break;
    }
  }

  return PrimComp;
}

void URGraspComponent::BeginPlay()
{
  Super::BeginPlay();

  Finger1 = GetComponent(FingerName1);
  Finger2 = GetComponent(FingerName2);

  if (!Finger1) {
    UE_LOG(LogTemp, Error, TEXT("Finger1 not found"));
    return;
  }

  if (!Finger2) {
    UE_LOG(LogTemp, Error, TEXT("Finger2 not found"));
    return;
  }
  
  bObjectGrasped = false;

  bFingerReady1 = false;
  bFingerReady2 = false;

  Finger1->OnComponentBeginOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaBeginOverlap);
  Finger2->OnComponentBeginOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaBeginOverlap);

  Finger1->OnComponentEndOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaEndOverlap);
  Finger2->OnComponentEndOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaEndOverlap);

  FString ConstraintName = TEXT("Constraint_") + GetName();
  Constraint1 = NewObject<UPhysicsConstraintComponent>(Finger1, FName(*ConstraintName));
  Constraint1->AttachToComponent(Finger1, FAttachmentTransformRules::KeepRelativeTransform);
  Constraint1->RegisterComponent();

  // FString ConstraintName2 = TEXT("Constraint2_") + GetName();
  // Constraint2 = NewObject<UPhysicsConstraintComponent>(Finger2, FName(*ConstraintName2));
  // Constraint2->AttachToComponent(Finger2, FAttachmentTransformRules::KeepRelativeTransform);
  // Constraint2->RegisterComponent();
}

void URGraspComponent::OnFixationGraspAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                       class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
  if (ARModel* SMA = Cast<ARModel>(OtherActor)) {
      return;
    }
  if (AStaticMeshActor* OtherSMA = Cast<AStaticMeshActor>(OtherActor)) {
    UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with %s"), *OtherActor->GetName());

    if (HitComp->GetFName() == FingerName1) {
      bFingerReady1 = true;
      UE_LOG(LogTemp, Warning, TEXT("Finger1 in Collision"));
    }
    else if(HitComp->GetFName() == FingerName2) {
      bFingerReady2 = true;
      UE_LOG(LogTemp, Warning, TEXT("Finger2 in Collision"));
    }

    if (bFingerReady1 && bFingerReady2) {
      GraspObject(OtherSMA);
    }
  }
}

void URGraspComponent::OnFixationGraspAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  // Remove actor from array (if present)
  if (AStaticMeshActor* SMA = Cast<AStaticMeshActor>(OtherActor)) {
    if (HitComp->GetFName() == FingerName1) {
      bFingerReady1 = false;
      UE_LOG(LogTemp, Warning, TEXT("Finger1 out Collision"));
    }
    else if (HitComp->GetFName() == FingerName2) {
      bFingerReady2 = false;
      UE_LOG(LogTemp, Warning, TEXT("Finger2 out Collision"));
    }

    if (!bFingerReady1 || !bFingerReady2) {
      ReleaseObject();
    }
  }
}


// Fixate object to hand
void URGraspComponent::GraspObject(AStaticMeshActor* InSMA)
{
  // AStaticMeshActor* ConstrainedActor = Cast<AStaticMeshActor>(InSMA->GetAttachParentActor());
  AStaticMeshActor* ConstrainedActor = InSMA;

  //If the grasped object is attached to another object (door handle), connecting via constraints moves
  // the gripper to the root object
  bool bParentFound = false;
  int NumIter = 0;
  while(!bParentFound)
    {
      AStaticMeshActor* TempActor = Cast<AStaticMeshActor>(ConstrainedActor->GetAttachParentActor());
      if(TempActor)
        {
          ConstrainedActor = TempActor;
          NumIter++;
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("ConstraintActor %s iter %d"),*ConstrainedActor->GetName(), NumIter);
          bParentFound = true;
        }
    }

  UStaticMeshComponent* SMC = nullptr;
  SMC = ConstrainedActor->GetStaticMeshComponent();

  if(!SMC)
    {
      UE_LOG(LogTemp, Error, TEXT("RootComponent of InSMA has no static mesh"));
      return;
    }

  FixatedObject = ConstrainedActor;
  if(Finger1 != nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("Finger1 %s"), *Finger1->GetName());
    Constraint1->SetConstrainedComponents(Finger1, NAME_None, SMC, NAME_None);
    SMC->SetSimulatePhysics(false);
    SMC->AttachToComponent(Constraint1, FAttachmentTransformRules::KeepWorldTransform);
  }

  if(Finger2 != nullptr)
    {
      // Constraint2->SetConstrainedComponents(Finger2, NAME_None, SMC, NAME_None);
    }
  bObjectGrasped = true;


}

// Detach fixation
void URGraspComponent::ReleaseObject() {

  if(FixatedObject)
  {
    if (Constraint1) {
      Constraint1->BreakConstraint();
    }
    if (Constraint2) {
      // Constraint2->BreakConstraint();
    }
    UStaticMeshComponent* SMC = FixatedObject->GetStaticMeshComponent();

    FixatedObject = nullptr;
  }
  bObjectGrasped = false;
}
