// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TPSEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Animation/TPSEnemyAnimInstance.h"
#include "AI/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
	CurrentHp = MaxHp;

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

	CurrentHp -= DamageAmount;
	if (CurrentHp <= 0)
	{
		SetDead();
	}
	else
	{
		SetDamage();
	}

	return DamageAmount;
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

