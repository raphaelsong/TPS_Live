// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T2Actor.generated.h"

class UT2Object;
class UStaticMeshComponent;

UCLASS()
class T2_API AT2Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AT2Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BoxMeshComponent;

protected:
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 50.0f;

	UPROPERTY(EditAnywhere)
	float RotateRate = 45.0f;

protected:
	UPROPERTY()
	UT2Object* Obj;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Hp = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Mp = 50;

	float Speed = 3.5f;
};
