// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TPSEnemy.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATPSEnemy::ATPSEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName("TPSEnemy");

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/_Art/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}
}

// Called when the game starts or when spawned
void ATPSEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPSEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

