#include "Factory/RModelBuilder.h"
#include "ROSCommunication/RROSCommunicationComponent.h"
#include "Controller/RControllerComponent.h"
#include "Logger/RLoggerComponent.h"
#include "Controller/ControllerType/BaseController/RBaseController.h"


// Sets default values
URModelBuilder::URModelBuilder()
{
  LinkFactory = CreateDefaultSubobject<URLinkFactory>(TEXT("LinkFactory"));
  JointFactory = CreateDefaultSubobject<URJointFactory>(TEXT("JointFactory"));
}

// Dtor
URModelBuilder::~URModelBuilder()
{
}

// Load model
void URModelBuilder::Load(USDFModel* InModelDescription, ARModel* OutModel,FVector InLocation)
{
  ModelDescription = InModelDescription;
  if(OutModel)
    {
      Model = OutModel;
      LoadLinks(InLocation);
      LoadJoints();
      BuildKinematicTree();
      SetupPlugins();
      SetupROS();
      SetupControl();
      SetupLogger();
    }
  else {
    UE_LOG(LogTemp, Error, TEXT("[ModelBuilder] Loading failed!"));
  }
}

// Load links
void URModelBuilder::LoadLinks(FVector InLocation)
{
  for(USDFLink* Link : ModelDescription->Links)
    {
      URLink* TempLink = LinkFactory->Load(Model, Link,InLocation);
      if(TempLink)
        {
          if(!Model->BaseLink)
            {
              Model->BaseLink = TempLink;
            }
          Model->AddLink(TempLink);
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Creation of Link %s failed"), *Link->GetName());
        }
    }
}

// Load joints
void URModelBuilder::LoadJoints()
{
  for(USDFJoint* Joint : ModelDescription->Joints)
    {
      URJoint* TempJoint = JointFactory->Load(Model, Joint);
      if(TempJoint)
        {
          Model->AddJoint(TempJoint);
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Creation of Joint %s failed"), *Joint->GetName());
        }
    }
}

void URModelBuilder::BuildKinematicTree()
{
  for(auto& Joint : Model->Joints)
    {
      UE_LOG(LogTemp, Display, TEXT("Trying to join links %s and %s via joint %s"), *Joint.Value->ParentName, *Joint.Value->ChildName, *Joint.Value->GetName());

      URLink** ResultParent = Model->Links.Find(Joint.Value->ParentName);
      if(!ResultParent)
        {
          UE_LOG(LogTemp, Error, TEXT("Parent %s not found"), *Joint.Value->ParentName);
          continue;
        }
      URLink* Parent = *ResultParent;
      URLink** ResultChild = Model->Links.Find(Joint.Value->ChildName);
      if(!ResultChild)
        {
          UE_LOG(LogTemp, Error, TEXT("Child %s not found"), *Joint.Value->ChildName);
          continue;
        }
      URLink* Child = *ResultChild;

      Joint.Value->SetParentChild(Parent, Child);
      SetConstraintPosition(Joint.Value);
      Joint.Value->Constraint->ConnectToComponents();
      if(!Child->bAttachedToParent)
        {
          Child->GetCollision()->AttachToComponent(Joint.Value->Parent->GetCollision(), FAttachmentTransformRules::KeepWorldTransform);
          // Child->GetCollision()->RegisterComponent();
          // Child->GetCollision()->RegisterComponentWithWorld(Model->GetWorld());
          Child->bAttachedToParent = true;
          // if(Joint.Value->Child->GetName().Equals(TEXT("fr_caster_l_wheel_link")))
          //   {
          //   }
          // UE_LOG(LogTemp, Error, TEXT("Joint: %s Parent: %s Child: %s"), *Joint.Value->GetName(), *Joint.Value->Parent->GetCollision()->GetName(), *Joint.Value->Child->GetCollision()->GetName());
        }
      // else if(!Parent->bAttachedToParent)
      //   {
      //     Parent->GetCollision()->AttachToComponent(Joint.Value->Child->GetCollision(), FAttachmentTransformRules::KeepWorldTransform);
      //     // Parent->GetCollision()->RegisterComponent();
      //     // Parent->GetCollision()->RegisterComponentWithWorld(Model->GetWorld());
      //     Parent->bAttachedToParent = true;
      //   }
      Parent->AddJoint(Joint.Value);
    }
}

void URModelBuilder::SetConstraintPosition(URJoint* InJoint)
{
  if(InJoint->bUseParentModelFrame)
    {
      InJoint->Constraint->AttachToComponent(InJoint->Child->GetCollision(), FAttachmentTransformRules::KeepWorldTransform);
      InJoint->Constraint->AttachToComponent(InJoint->Parent->GetCollision(), FAttachmentTransformRules::KeepWorldTransform);
      InJoint->Constraint->SetWorldLocation(InJoint->Child->GetCollision()->GetComponentLocation());
      InJoint->Constraint->AddRelativeLocation(InJoint->Pose.GetLocation());
      InJoint->Constraint->AddRelativeRotation(InJoint->Pose.GetRotation());
    }
  else
    {
      //InJoint->Constraint->SetPosition(InJoint);
      InJoint->Constraint->AttachToComponent(InJoint->Child->GetCollision(), FAttachmentTransformRules::KeepWorldTransform);
      InJoint->Constraint->AttachToComponent(InJoint->Parent->GetCollision(), FAttachmentTransformRules::KeepWorldTransform);
      InJoint->Constraint->SetWorldLocationAndRotation(InJoint->Child->GetCollision()->GetComponentLocation(), InJoint->Child->GetCollision()->GetComponentQuat());
    }
}

void URModelBuilder::SetupPlugins()
{
  for(USDFPlugin* Plugin : ModelDescription->Plugins)
    {
      URJoint* Joint = Model->Joints[Plugin->Joint];

      if(Joint)
        {
          URJoint* MimicJoint = Model->Joints[Plugin->MimicJoint];
          if(MimicJoint)
            {
              FMimicJointParameter MimicJointParameter;
              MimicJointParameter.MimicJoint = MimicJoint;
              MimicJointParameter.Multiplier = Plugin->Multiplier;
              Joint->MimicJointList.Add(MimicJointParameter);
              Joint->bHasMimic = true;
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("MimicJoint %s not found"), *Plugin->MimicJoint);
            }
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Joint  to Mimic %s not found"), *Plugin->Joint);
        }
    }
}

void URModelBuilder::SetupROS()
{
  URROSCommunicationComponent* ROSCommunicationComponent = NewObject<URROSCommunicationComponent>(Model, TEXT("ROSCommunicationComponent"));
  ROSCommunicationComponent->ROSCommunication.WebsocketIPAddr = TEXT("127.0.0.1");
  ROSCommunicationComponent->ROSCommunication.WebsocketPort = 9090;

  ROSCommunicationComponent->RegisterComponent();
}
void URModelBuilder::SetupControl()
{
  URControllerComponent* ControllerComponent = NewObject<URControllerComponent>(Model, TEXT("ControllerComponent"));
  ControllerComponent->RegisterComponent();
}
void URModelBuilder::SetupLogger()
{
  URLoggerComponent* LoggerComponent = NewObject<URLoggerComponent>(Model, TEXT("LoggerComponent"));
  LoggerComponent->RegisterComponent();
}