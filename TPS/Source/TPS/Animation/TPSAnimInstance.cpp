// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TPSAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/TPSCharacter.h"

UTPSAnimInstance::UTPSAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_Rifle_Fire.AM_Rifle_Fire'"));
	if (FireMontageRef.Succeeded())
	{
		FireMontage = FireMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_Rifle_Reload.AM_Rifle_Reload'"));
	if (ReloadMontageRef.Succeeded())
	{
		ReloadMontage = ReloadMontageRef.Object;
	}
}

void UTPSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ATPSCharacter>(TryGetPawnOwner());

	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character == nullptr)
		return;

	if (MovementComponent == nullptr)
		return;

	Velocity = MovementComponent->Velocity;

	FVector ForwardVector = Character->GetActorForwardVector();
	GroundSpeed = FVector::DotProduct(ForwardVector, Velocity);
	FVector RightVector = Character->GetActorRightVector();
	GroundDirection = FVector::DotProduct(RightVector, Velocity);

	bShouldMove = (Velocity.Size2D() > 3.0f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);

	bIsFalling = MovementComponent->IsFalling();
}

void UTPSAnimInstance::PlayFireMontage()
{
	Montage_Play(FireMontage);
}

void UTPSAnimInstance::PlayReloadMontage()
{
	Montage_Play(ReloadMontage);
	Montage_GetEndedDelegate(ReloadMontage)->BindUObject(this, &UTPSAnimInstance::FinishReloading);
}

void UTPSAnimInstance::FinishReloading(UAnimMontage* Montage, bool bInterrupted)
{
	if (Character)
	{
		Character->FinishReloading();
	}
}
