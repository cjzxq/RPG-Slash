// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterfaces.h" //包含接口头文件
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent; 

UCLASS()
class SLASH_API ABreakableActor : public AActor,public IHitInterfaces//继承接口
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;//在人物没有打碎物体前，阻挡人物


private:	


	//TSubclassOf<class ATreasure>TreasureClass;//用了TSubclassOf这个包装包，在蓝图中Treasure class显示的类型是基于Treasure的类型

	//创建一个随机的宝藏
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>>TreasureClasses;//宝藏类数组

	bool BeBroken=false; //一旦调用被击中，变为真

};
