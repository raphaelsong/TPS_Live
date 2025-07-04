// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class TPS_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Stat System
public:
	void SetHp(float NewHp);
	void SetDead();
	void UpdateAmmoCount(int32 AmmoRemainCount, int32 AmmoMaxCount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UTPSPlayerHUDWidget> TPSPlayerHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UTPSPlayerHUDWidget> TPSPlayerHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHp = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp = 100.0f;

// Reload
public:
	void StartReloading();
	void FinishReloading();

// Weapon System
public:
	void AttachWeapon(TSubclassOf<class AWeapon> NewWeapon);

protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AWeapon> EquipWeapon;

// Input System
private:
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Turn(const FInputActionValue& InputValue);
	void Input_Run(const FInputActionValue& InputValue);
	void Input_Fire(const FInputActionValue& InputValue);
	void Input_Reload(const FInputActionValue& InputValue);

protected:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> IMCDefault;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> TurnAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ReloadAction;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

protected:
	UPROPERTY(EditAnywhere, Category = PlayerStat)
	float WalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = PlayerStat)
	float RunSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = Reload)
	bool bIsReload = false;
};
