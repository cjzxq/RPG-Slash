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
	void SetHealthBarPercent(float Percent);//显示HealthProgressBar进度条 ，血量;
	void SetStaminaBarPercent(float Percent);
	void SetGold(int32 Gold);
	void SetSouls(int32 Souls);
private:

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthProgressBar;//名字必须与WBP_SlashOverlay中的一样

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaProgressBar;//名字必须与WBP_SlashOverlay中的一样

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GlodText;//名字必须与WBP_SlashOverlay中的一样

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulsText;//名字必须与WBP_SlashOverlay中的一样
};
