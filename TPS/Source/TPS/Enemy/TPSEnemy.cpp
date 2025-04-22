// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TPSEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Animation/TPSEnemyAnimInstance.h"
#include "AI/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
ATPSEnemy::ATPSEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyAIController::StaticClass();

	GetCapsuleComponent()->SetCollisionProfileName("TPSEnemy");
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

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
	
	SetHp(MaxHp);

	UTPSEnemyAnimInstance* AnimInstance = Cast<UTPSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnAttackFinished.AddUObject(this, &ATPSEnemy::AttackEnded);
	}
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

float ATPSEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	SetHp(CurrentHp - DamageAmount);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		SetDead();
	}
	else
	{
		SetDamage();
	}

	return DamageAmount;
}

void ATPSEnemy::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
}

void ATPSEnemy::SetDamage()
{
	UTPSEnemyAnimInstance* AnimInstance = Cast<UTPSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayHitMontage();
	}
}

void ATPSEnemy::SetDead()
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	UTPSEnemyAnimInstance* AnimInstance = Cast<UTPSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayDeadMontage();
	}

	SetActorEnableCollision(false);

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, 
		FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
		),
		5.0f, false);
}

void ATPSEnemy::Attack()
{
	UTPSEnemyAnimInstance* EnemyAnimInstance = Cast<UTPSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (EnemyAnimInstance)
	{
		EnemyAnimInstance->PlayAttackMontage();
	}
}

void ATPSEnemy::AttackEnded()
{
}

void ATPSEnemy::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const float AttackRange = 80.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected)
	{
		ACharacter* HitCharacter = Cast<ACharacter>(OutHitResult.GetActor());
		if (HitCharacter)
		{
			FDamageEvent DamageEvent;
			HitCharacter->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif
}

