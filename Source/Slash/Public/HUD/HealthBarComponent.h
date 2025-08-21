// Fill out your copyright notice in the Description page of Project Settings.
//��ʾ�ؼ������0812
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
	void SetHealthPercent(float Percent);//������������������ʹ�õ��û��ؼ�����������ٷֱ�
private:
	UPROPERTY()
	class UHealthBar* HealthBarWidget;

	
};
