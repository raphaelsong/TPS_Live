// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TPS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void Fire(const FVector& Direction) const;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = Properties)
	float InitSpeed = 30000.0f;

	UPROPERTY(EditAnywhere, Category = Properties)
	float MaxSpeed = 30000.0f;
};
