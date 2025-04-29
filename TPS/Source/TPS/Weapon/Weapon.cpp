// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Character/TPSCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/_Art/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (WeaponMeshRef.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(WeaponMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/_Art/Effect/Particles/P_HitEffect.P_HitEffect'"));
	if (HitEffectRef.Succeeded())
	{
		HitEffect = HitEffectRef.Object;
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	SetAmmoRemainCount(AmmoMaxCount);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::SetAmmoRemainCount(int32 NewAmmoRemainCount)
{
	AmmoRemainCount = NewAmmoRemainCount;

	ATPSCharacter* OwnerCharacter = Cast<ATPSCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->UpdateAmmoCount(AmmoRemainCount, AmmoMaxCount);
	}
}

void AWeapon::StartFire(TWeakObjectPtr<class ATPSCharacter> OwnerCharacter)
{
}

void AWeapon::StopFire()
{
}

void AWeapon::Reloading()
{
}

void AWeapon::FinishReloading()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Reload"));
	SetAmmoRemainCount(AmmoMaxCount);
}

void AWeapon::PlayHitEffect(FTransform HitTransform)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitTransform);
}

