// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(6.0f);
	SphereCollision->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetRelativeLocation(FVector(-5.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/_Art/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_40mm_G.SM_Shell_40mm_G'"));
	if (MeshRef.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(MeshRef.Object);
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->InitialSpeed = InitSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = false;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Fire(const FVector& Direction) const
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);
	if (HitCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("HitCharacter"));
		Destroy();
	}
}

