// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterfaces.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)//  UInterface 允许我们的接口参与反射系统 IHitInterfaces是多重继承时使用的接口类
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
	//常量引用传递，引用时不需要复制数据，避免不必要的性能开销 不想改变值和不想复制数据时使用常量引用
	UFUNCTION(BluePrintNativeEvent)//既可以在蓝图中使用，又可以在c++中使用 蓝图原生事件 这样设计就不需要virtual，也不需要=0了 有两个版本，c++版本是虚拟的
	void GetHit(const FVector& ImpactPoint);//加了上面一行，本来是纯虚函数，必须在实现接口的类中重写  修改了 
};
