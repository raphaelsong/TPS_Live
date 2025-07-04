// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponRifle.h"
#include "Character/TPSCharacter.h"
#include "Bullet.h"
#include "GameFramework/Character.h"
#include "Engine/DamageEvents.h"

AWeaponRifle::AWeaponRifle()
{
	Bullet = ABullet::StaticClass();
	FireType = EFireType::EF_Projectile;
}

void AWeaponRifle::StartFire(TWeakObjectPtr<class ATPSCharacter> OwnerCharacter)
{
	Super::StartFire(OwnerCharacter);

	ATPSCharacter* Character = OwnerCharacter.Get();
	if (Character)
	{
		switch (FireType)
		{
		case EFireType::EF_LineTrace:
			GetWorld()->GetTimerManager().SetTimer(
				FireTimerHandle,
				[Character, this]() { FireWithLineTrace(Character); },
				FireInterval,
				true);
			break;
		case EFireType::EF_Projectile:
			GetWorld()->GetTimerManager().SetTimer(
				FireTimerHandle,
				[Character, this]() { FireWithProjectile(Character); },
				FireInterval,
				true);
			break;
		default:
			break;
		}
	}
}

void AWeaponRifle::StopFire()
{
	Super::StopFire();

	if (FireTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}
}

void AWeaponRifle::Reloading()
{
	Super::Reloading();
}

void AWeaponRifle::FinishReloading()
{
	Super::FinishReloading();
}

void AWeaponRifle::FireWithProjectile(TWeakObjectPtr<class ATPSCharacter> OwnerCharacter)
{
	ATPSCharacter* Character = OwnerCharacter.Get();
	if (Character == nullptr)
		return;

	if (AmmoRemainCount <= 0)
	{
		Character->StopAnimMontage(Character->GetCurrentMontage());
		StopFire();
		return;
	}

	FTransform FireTransform = WeaponMesh->GetSocketTransform("FireSocket");
	FVector Direction = FireTransform.GetRotation().GetForwardVector()* TraceDistance;

	FActorSpawnParameters SpawnParameter;
	SpawnParameter.Owner = Character;
	ABullet* SpawnBullet = GetWorld()->SpawnActor<ABullet>(Bullet, SpawnParameter);
	if (SpawnBullet)
	{
		SpawnBullet->SetActorLocation(FireTransform.GetLocation());
		SpawnBullet->SetActorRotation(FireTransform.GetRotation());
		SpawnBullet->SetAttackDamage(AttackDamage);

		if (Direction.Normalize())
		{
			SpawnBullet->Fire(Direction);
		}
		
		SetAmmoRemainCount(AmmoRemainCount - 1);

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Ammo Remain : %d"), AmmoRemainCount));

		if (AmmoRemainCount <= 0)
		{
			Character->StopAnimMontage(Character->GetCurrentMontage());
			StopFire();
			Character->StartReloading();
		}
	}
}

void AWeaponRifle::FireWithLineTrace(TWeakObjectPtr<class ATPSCharacter> OwnerCharacter)
{
	ATPSCharacter* Character = OwnerCharacter.Get();
	if (Character == nullptr)
		return;

	if (AmmoRemainCount <= 0)
	{
		Character->StopAnimMontage(Character->GetCurrentMontage());
		StopFire();
		return;
	}

	APlayerCameraManager* PlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	const FVector Start = PlayerCameraManager->GetCameraLocation();
	const FVector End = Start + PlayerCameraManager->GetActorForwardVector() * TraceDistance;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Character);

	bool HitDetected = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams);

	if (HitDetected)
	{
		ACharacter* HitCharacter = Cast<ACharacter>(HitResult.GetActor());
		if (HitCharacter)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("LineTraceHit"));

			FDamageEvent DamageEvent;
			HitCharacter->TakeDamage(AttackDamage, DamageEvent, Character->GetController(), Character);
		}

		FTransform HitTransform;
		HitTransform.SetLocation(HitResult.ImpactPoint);
		PlayHitEffect(HitTransform);
	}

	SetAmmoRemainCount(AmmoRemainCount - 1);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Ammo Remain : %d"), AmmoRemainCount));

	if (AmmoRemainCount <= 0)
	{
		Character->StopAnimMontage(Character->GetCurrentMontage());
		StopFire();
		Character->StartReloading();
	}

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, End, DrawColor, false, 1.0f);
#endif
}
