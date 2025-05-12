// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/TPSAnimInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/TPSPlayerHUDWidget.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName("TPSPlayer");

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/_Art/MilitaryCharDark/MW_Style2_Male.MW_Style2_Male'"));

	if (FindMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(FindMesh.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 70.0f, 90.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FClassFinder<AWeapon> WeaponClassRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Weapon/BP_WeaponRifle.BP_WeaponRifle_C'"));
	if (WeaponClassRef.Succeeded())
	{
		WeaponClass = WeaponClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UTPSPlayerHUDWidget> PlayerHUDWIdgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_PlayerHUD.WBP_PlayerHUD_C'"));
	if (PlayerHUDWIdgetRef.Succeeded())
	{
		TPSPlayerHUDWidgetClass = PlayerHUDWIdgetRef.Class;
	}

#pragma region InputSystems
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCDefaultRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (IMCDefaultRef.Succeeded())
	{
		IMCDefault = IMCDefaultRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TurnActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Turn.IA_Turn'"));
	if (TurnActionRef.Succeeded())
	{
		TurnAction = TurnActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RunActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Run.IA_Run'"));
	if (RunActionRef.Succeeded())
	{
		RunAction = RunActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FireActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Fire.IA_Fire'"));
	if (FireActionRef.Succeeded())
	{
		FireAction = FireActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ReloadActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Reload.IA_Reload'"));
	if (ReloadActionRef.Succeeded())
	{
		ReloadAction = ReloadActionRef.Object;
	}
#pragma endregion

}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		TPSPlayerHUDWidget = CreateWidget<UTPSPlayerHUDWidget>(PlayerController, TPSPlayerHUDWidgetClass);

		if (TPSPlayerHUDWidget)
		{
			TPSPlayerHUDWidget->AddToViewport();
		}
	}

	SetHp(MaxHp);

	AttachWeapon(WeaponClass);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	if (PlayerController)
	{
		auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(IMCDefault, 0);
		}
	}
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Input_Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Input_Turn);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Input_Run);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Input_Fire);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Input_Reload);
	}
}

float ATPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	SetHp(CurrentHp - DamageAmount);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		SetDead();
	}
	
	return DamageAmount;
}

void ATPSCharacter::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	if (TPSPlayerHUDWidget)
	{
		TPSPlayerHUDWidget->UpdateHpBar(CurrentHp, MaxHp);
	}
}

void ATPSCharacter::SetDead()
{
	SetActorEnableCollision(false);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void ATPSCharacter::UpdateAmmoCount(int32 AmmoRemainCount, int32 AmmoMaxCount)
{
	if (TPSPlayerHUDWidget)
	{
		TPSPlayerHUDWidget->UpdateAmmo(AmmoRemainCount, AmmoMaxCount);
	}
}

void ATPSCharacter::StartReloading()
{
	if (bIsReload == true)
		return;

	UTPSAnimInstance* AnimInstnace = Cast<UTPSAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstnace)
		return;

	if (nullptr == EquipWeapon)
		return;

	bIsReload = true;
	EquipWeapon->StopFire();
	EquipWeapon->Reloading();
	AnimInstnace->PlayReloadMontage();
}

void ATPSCharacter::FinishReloading()
{
	UTPSAnimInstance* AnimInstnace = Cast<UTPSAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstnace)
		return;

	if (nullptr == EquipWeapon)
		return;

	bIsReload = false;
	EquipWeapon->FinishReloading();
	AnimInstnace->StopAllMontages(false);
}

void ATPSCharacter::AttachWeapon(TSubclassOf<class AWeapon> NewWeapon)
{
	if (NewWeapon)
	{
		FActorSpawnParameters PawnParams;
		PawnParams.Owner = this;
		EquipWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon, PawnParams);

		const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName("WeaponSocket");

		if (EquipWeapon && WeaponSocket)
		{
			WeaponSocket->AttachActor(EquipWeapon, GetMesh());
		}
	}
}

void ATPSCharacter::Input_Move(const FInputActionValue& InputValue)
{
	FVector2D MovementVector = InputValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ATPSCharacter::Input_Turn(const FInputActionValue& InputValue)
{
	FVector2D LookVector = InputValue.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void ATPSCharacter::Input_Run(const FInputActionValue& InputValue)
{
	bool IsRun = InputValue.Get<bool>();

	if (IsRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ATPSCharacter::Input_Fire(const FInputActionValue& InputValue)
{
	if (bIsReload == true)
		return;

	if (EquipWeapon == nullptr)
		return;

	UTPSAnimInstance* AnimInstance = Cast<UTPSAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	bool IsStartFire = InputValue.Get<bool>();

	if (IsStartFire && EquipWeapon->GetAmmoRemainCount() > 0)
	{
		EquipWeapon->StartFire(this);
		AnimInstance->PlayFireMontage();
	}
	else
	{
		EquipWeapon->StopFire();
		AnimInstance->StopAllMontages(false);
	}
}

void ATPSCharacter::Input_Reload(const FInputActionValue& InputValue)
{
	StartReloading();
}

