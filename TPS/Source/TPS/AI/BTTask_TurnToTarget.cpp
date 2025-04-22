// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
		return EBTNodeResult::Failed;

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if(nullptr == TargetPawn)
		return EBTNodeResult::Failed;

	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	TargetRotation = FRotationMatrix::MakeFromX(LookVector).Rotator();

	return EBTNodeResult::InProgress;
}

void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	CurrentRotation = ControllingPawn->GetActorRotation();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, TurnSpeed));

	if (ControllingPawn->GetActorRotation() == TargetRotation)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
