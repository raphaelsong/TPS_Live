// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UTPSHpBarWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateHp(float NewCurrentHp);
	void UpdateMaxHp(float NewMaxHp);
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;

	float CurrentHp;
	float MaxHp;
};
