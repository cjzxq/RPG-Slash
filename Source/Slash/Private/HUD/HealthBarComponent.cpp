// Fill out your copyright notice in the Description page of Project Settings.
//UserWidget��ʾ--WidgetComponnentʵ�ֹ���--

#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"
void UHealthBarComponent::SetHealthPercent(float Percent)
{
	//��UUserWidgetָ���ΪUHealthBarWidgetָ��
	//������Ƶ�����õĺ����ж��ת��������ֱ�Ӵ���һ��������͵ĳ�Ա�����������䴢����HealthBarComponent��
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)//���ת���ɹ�
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
