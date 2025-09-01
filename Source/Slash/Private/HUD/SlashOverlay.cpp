// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"//����������Է���HealthProgressBar
#include "Components/TextBlock.h"
void USlashOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void USlashOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void USlashOverlay::SetGold(int32 Gold)
{
	if (GlodText)
	{
		//FString::Printf(TEXT("%d"), Gold);//�᷵��һ��FString���� 
		// FText::FromString();//����FString��������ͬ�ı�ֵ��FText
		const FString String= FString::Printf(TEXT("%d"), Gold);
		const FText Text= FText::FromString(String);
		GlodText->SetText(Text);
	}

}

void USlashOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		//FString::Printf(TEXT("%d"), Gold);//�᷵��һ��FString���� 
		// FText::FromString();//����FString��������ͬ�ı�ֵ��FText
		const FString String = FString::Printf(TEXT("%d"), Souls);
		const FText Text = FText::FromString(String);
		SoulsText->SetText(Text);
	}
}
