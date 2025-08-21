// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthBar;//这样做之后，c++的健康条变量就会和蓝图健康条变量连接，这个变量再控件蓝图中称为WBP_Health_Bar 名字必须一致，都是HealthBar

	
};
