// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSHpBarWidget.h"
#include "Components/ProgressBar.h"

UTPSHpBarWidget::UTPSHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentHp = -1.f;
	MaxHp = -1.f;
}

void UTPSHpBarWidget::UpdateHp(float NewCurrentHp)
{
	CurrentHp = FMath::Clamp<float>(NewCurrentHp, 0.0f, MaxHp);

	if (HpBar)
	{
		HpBar->SetPercent(FMath::Clamp<float>(CurrentHp/MaxHp, 0.0, 1.0f));
	}
}

void UTPSHpBarWidget::UpdateMaxHp(float NewMaxHp)
{
	MaxHp = FMath::Clamp<float>(NewMaxHp, 0.0f, NewMaxHp);

	if (HpBar)
	{
		HpBar->SetPercent(FMath::Clamp<float>(CurrentHp / MaxHp, 0.0, 1.0f));
	}
}

void UTPSHpBarWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	MaxHp = FMath::Clamp<float>(NewMaxHp, 0.0f, NewMaxHp);
	CurrentHp = FMath::Clamp<float>(NewCurrentHp, 0.0f, MaxHp);

	if (HpBar)
	{
		HpBar->SetPercent(FMath::Clamp<float>(CurrentHp / MaxHp, 0.0, 1.0f));
	}
}
