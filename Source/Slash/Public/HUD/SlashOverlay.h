// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthBarPercent(float Percent);//��ʾHealthProgressBar������ ��Ѫ��;
	void SetStaminaBarPercent(float Percent);
	void SetGold(int32 Gold);
	void SetSouls(int32 Souls);
private:

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthProgressBar;//���ֱ�����WBP_SlashOverlay�е�һ��

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaProgressBar;//���ֱ�����WBP_SlashOverlay�е�һ��

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GlodText;//���ֱ�����WBP_SlashOverlay�е�һ��

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulsText;//���ֱ�����WBP_SlashOverlay�е�һ��
};
