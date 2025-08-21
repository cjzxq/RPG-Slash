// Fill out your copyright notice in the Description page of Project Settings.
//UserWidget显示--WidgetComponnent实现功能--

#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"
void UHealthBarComponent::SetHealthPercent(float Percent)
{
	//从UUserWidget指针变为UHealthBarWidget指针
	//避免再频繁调用的函数中多次转换，可以直接创建一个这个类型的成员变量，并将其储存在HealthBarComponent中
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)//如果转换成功
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
