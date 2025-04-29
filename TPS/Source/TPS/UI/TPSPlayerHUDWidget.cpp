// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSPlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UTPSPlayerHUDWidget::UpdateHpBar(float CurrentHp, float MaxHp)
{
	if (HpBar)
	{
		HpBar->SetPercent(FMath::Clamp(CurrentHp / MaxHp, 0.0f, 1.0f));
	}

	if (HpText)
	{
		HpText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), static_cast<int32>(CurrentHp), static_cast<int32>(MaxHp))));
	}
}

void UTPSPlayerHUDWidget::UpdateAmmo(int32 NewAmmoRemainCount, int32 NewAmmoMaxCount)
{
	if (AmmoRemainText)
	{
		AmmoRemainText->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewAmmoRemainCount)));		
	}

	if (AmmoMaxText)
	{
		AmmoMaxText->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewAmmoMaxCount)));
	}
}
