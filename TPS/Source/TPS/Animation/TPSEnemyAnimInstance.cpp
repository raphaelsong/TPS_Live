// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TPSEnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/TPSEnemy.h"

UTPSEnemyAnimInstance::UTPSEnemyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_EnemyHit.AM_EnemyHit'"));
	if (HitMontageRef.Succeeded())
	{
		HitMontage = HitMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_EnemyAttack.AM_EnemyAttack'"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}
}

void UTPSEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ATPSEnemy>(TryGetPawnOwner());
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UTPSEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character == nullptr)
		return;

	if (MovementComponent == nullptr)
		return;

	Velocity = MovementComponent->Velocity;

	bShouldMove = (Velocity.Size2D() > 3.0f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);
}

void UTPSEnemyAnimInstance::PlayHitMontage()
{
	int32 RandomIndex = FMath::RandRange(1, 2);
	FString SectionName = FString::Printf(TEXT("Hit%d"), RandomIndex);
	Montage_Play(HitMontage);
	Montage_JumpToSection(*SectionName);
}

void UTPSEnemyAnimInstance::PlayDeadMontage()
{
	FString SectionName = TEXT("Dead");
	Montage_Play(HitMontage);
	Montage_JumpToSection(*SectionName);
}

void UTPSEnemyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage);
	Montage_GetEndedDelegate(AttackMontage)->BindUObject(this, &UTPSEnemyAnimInstance::FinishAttackMontage);
}

void UTPSEnemyAnimInstance::FinishAttackMontage(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackFinished.Broadcast();
}
