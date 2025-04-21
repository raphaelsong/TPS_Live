// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Enemy/TPSEnemy.h"
#include "Animation/TPSEnemyAnimInstance.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
		return EBTNodeResult::Failed;

	ATPSEnemy* ControllingPawn = Cast<ATPSEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	UTPSEnemyAnimInstance* EnemyAnimInstance = Cast<UTPSEnemyAnimInstance>(ControllingPawn->GetMesh()->GetAnimInstance());
	if(nullptr == EnemyAnimInstance)
		return EBTNodeResult::Failed;
	
	EnemyAnimInstance->OnAttackFinished.AddLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	ControllingPawn->Attack();
	return EBTNodeResult::InProgress;
}
