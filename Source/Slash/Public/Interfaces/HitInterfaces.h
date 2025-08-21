// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterfaces.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)//  UInterface �������ǵĽӿڲ��뷴��ϵͳ IHitInterfaces�Ƕ��ؼ̳�ʱʹ�õĽӿ���
class UHitInterfaces : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IHitInterfaces
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//�������ô��ݣ�����ʱ����Ҫ�������ݣ����ⲻ��Ҫ�����ܿ��� ����ı�ֵ�Ͳ��븴������ʱʹ�ó�������
	UFUNCTION(BluePrintNativeEvent)//�ȿ�������ͼ��ʹ�ã��ֿ�����c++��ʹ�� ��ͼԭ���¼� ������ƾͲ���Ҫvirtual��Ҳ����Ҫ=0�� �������汾��c++�汾�������
	void GetHit(const FVector& ImpactPoint);//��������һ�У������Ǵ��麯����������ʵ�ֽӿڵ�������д  �޸��� 
};
