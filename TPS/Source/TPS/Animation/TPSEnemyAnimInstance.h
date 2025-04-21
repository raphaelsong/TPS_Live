// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSEnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyAttackFinished);

/**
 * 
 */
UCLASS()
class TPS_API UTPSEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UTPSEnemyAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void PlayHitMontage();
	void PlayDeadMontage();
	void PlayAttackMontage();
	void FinishAttackMontage(UAnimMontage* Montage, bool bInterrupted);

public:
	FOnEnemyAttackFinished OnAttackFinished;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	TObjectPtr<class UAnimMontage> AttackMontage;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ATPSEnemy> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove = false;
};
