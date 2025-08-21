// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponents.h"

// Sets default values for this component's properties
UAttributeComponents::UAttributeComponents()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponents::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponents::ReciveDamage(float Damage)
{
	//ȷ������ֵ�������0
	Health = FMath::Clamp(Health-Damage,0.f,MaxHealth);//�������������Health<0
}

float UAttributeComponents::GetHealthPercent()
{
	return Health / MaxHealth;
}

bool UAttributeComponents::IsAlive()//0816
{
	return Health > 0.f;
}


// Called every frame
void UAttributeComponents::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

