// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	TArray<AActor*>IgnoreActors;//0808 记录所有被击中的角色
	void Equip(USceneComponent* InParent,FName InSocketName,AActor*NewOwner,APawn*NewInstiGator);//为了伤害敌人增加了输入0812

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

protected:

	    virtual void BeginPlay() override;
	   //因为是重写的，继承来的，所以不需要	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

		UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		UFUNCTION(BlueprintImplementableEvent)
		void CreatFields(const FVector& FieldLoocation);//创建临时场，会从c++调用，传入我们想要创建场的位置，其余部分在蓝图处理，所以不需要在这里实现0809 weapon.cpp中调用
private:

	UPROPERTY(EditAnywhere,Category="Weapon Properties")
    USoundBase* EquipSound; // 装备武器时播放的声音

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties");
	UBoxComponent* WeaponBox; // 武器碰撞体积

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart; //组件通常用VisibleAnywhere，而基础类型变量通常用EditAnywhere
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;//

	UPROPERTY(EditAnywhere, Category = "Weapon Properties");
	float Damage = 20.f;//武器伤害变量0812


public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; } //获取武器碰撞体积


};
