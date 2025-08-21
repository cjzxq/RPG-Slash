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

	TArray<AActor*>IgnoreActors;//0808 ��¼���б����еĽ�ɫ
	void Equip(USceneComponent* InParent,FName InSocketName,AActor*NewOwner,APawn*NewInstiGator);//Ϊ���˺���������������0812

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

protected:

	    virtual void BeginPlay() override;
	   //��Ϊ����д�ģ��̳����ģ����Բ���Ҫ	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

		UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		UFUNCTION(BlueprintImplementableEvent)
		void CreatFields(const FVector& FieldLoocation);//������ʱ�������c++���ã�����������Ҫ��������λ�ã����ಿ������ͼ�������Բ���Ҫ������ʵ��0809 weapon.cpp�е���
private:

	UPROPERTY(EditAnywhere,Category="Weapon Properties")
    USoundBase* EquipSound; // װ������ʱ���ŵ�����

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties");
	UBoxComponent* WeaponBox; // ������ײ���

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart; //���ͨ����VisibleAnywhere�����������ͱ���ͨ����EditAnywhere
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;//

	UPROPERTY(EditAnywhere, Category = "Weapon Properties");
	float Damage = 20.f;//�����˺�����0812


public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; } //��ȡ������ײ���


};
