// Fill out your copyright notice in the Description page of Project Settings.
//显示控件的组件0812
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);//这个函数会访问这个组件使用的用户控件，并设置其百分比
private:
	UPROPERTY()
	class UHealthBar* HealthBarWidget;

	
};
