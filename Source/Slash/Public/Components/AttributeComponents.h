// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponents.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponents : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponents();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegeneStamina(float DeltaTime);//

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float Health;
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	int32 Gold;
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	int32 Souls;//BP_Paladin中Actor Attribute下会有Souls选项

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float Stamina;
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float DodgeCost=14.f;//消耗体力值
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float StaminaRegenRate=8.f;//恢复体力值
public:
	void ReciveDamage(float Damage);
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();//0816
	void AddGold(int32 AmountOfGold);
	void AddSouls(int32 NumberOfSouls);
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return  Souls; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
};

