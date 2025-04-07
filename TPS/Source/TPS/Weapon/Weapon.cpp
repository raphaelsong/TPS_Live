// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

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
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

