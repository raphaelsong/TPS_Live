// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHpBar(float CurrentHp, float MaxHp);
	void UpdateAmmo(int32 NewAmmoRemainCount, int32 NewAmmoMaxCount);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> HpText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> AmmoRemainText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> AmmoMaxText;
};
