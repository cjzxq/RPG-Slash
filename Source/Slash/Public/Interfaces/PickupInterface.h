// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetOverlappingItem(class AItem* Item);//任何实现这个接口的类都可以随意处理这个Item 
	virtual void AddSouls(class ASoul* Soul);//这个和上面那个都在SlashCharacter中重写
	virtual void AddGold(class ATreasure* Treasure);//添加黄金功能，在SlashCharacter中重写
};
