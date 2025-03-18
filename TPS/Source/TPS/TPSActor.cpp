// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSActor.h"

// Sets default values
ATPSActor::ATPSActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = Body;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FindMesh(TEXT("/Script/Engine.StaticMesh'/Game/_Art/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));

	if (FindMesh.Succeeded())
	{
		Body->SetStaticMesh(FindMesh.Object);
	}

	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));
	Water->SetupAttachment(Body);
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FindWater(TEXT("/Script/Engine.StaticMesh'/Game/_Art/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));

	if (FindWater.Succeeded())
	{
		Water->SetStaticMesh(FindWater.Object);
	}
}

// Called when the game starts or when spawned
void ATPSActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPSActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

