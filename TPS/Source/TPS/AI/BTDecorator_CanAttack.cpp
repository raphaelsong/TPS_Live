// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CanAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (nullptr == TargetPawn)
		return false;

	return (ControllingPawn->GetDistanceTo(TargetPawn) <= AttackRange);
}
