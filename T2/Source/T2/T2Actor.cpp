// Fill out your copyright notice in the Description page of Project Settings.


#include "T2Actor.h"
#include "T2Object.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AT2Actor::AT2Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	SetRootComponent(BoxMeshComponent);
}

// Called when the game starts or when spawned
void AT2Actor::BeginPlay()
{
	Super::BeginPlay();
	
	Obj = NewObject<UT2Object>();

	GEngine->ForceGarbageCollection(true);
}

// Called every frame
void AT2Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Obj == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Obj Deleted!"));
	}

	FVector Location = GetActorLocation();
	float Distance = MoveSpeed * DeltaTime;

	//SetActorLocation(Location + FVector::ForwardVector * Distance);

	AddActorWorldOffset(FVector::ForwardVector * Distance);

	FRotator Rotation = GetActorRotation();
	FRotator NewRotation = FRotator(Rotation.Pitch, Rotation.Yaw + RotateRate * DeltaTime, Rotation.Roll);
	//SetActorRotation(NewRotation);

	AddActorWorldRotation(FRotator(0.0f, RotateRate * DeltaTime, 0.0f));
}

