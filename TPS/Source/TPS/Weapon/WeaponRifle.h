// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "WeaponRifle.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API AWeaponRifle : public AWeapon
{
	GENERATED_BODY()
	
public:
	AWeaponRifle();

public:
	virtual void StartFire(TWeakObjectPtr<class ATPSCharacter> OwnerCharacter) override;
	virtual void StopFire() override;
	virtual void Reloading() override;
	virtual void FinishReloading() override;

	void FireWithProjectile(TWeakObjectPtr<class ATPSCharacter> OwnerCharacter);
	void FireWithLineTrace(TWeakObjectPtr<class ATPSCharacter> OwnerCharacter);

private:
	UPROPERTY(EditAnywhere, Category = Bullet)
	TSubclassOf<class ABullet> Bullet;

	FTimerHandle FireTimerHandle;
};
