// Fill out your copyright notice in the Description page of Project Settings.
//创建小部件添加到视口
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

class USlashOverlay;
UCLASS()
class SLASH_API ASlashHUD : public AHUD
{

	GENERATED_BODY()

protected:
	virtual void BeginPlay()override;
private:
	UPROPERTY(EditDefaultsOnly,Category=Slash)
	TSubclassOf<USlashOverlay>SlashOverlayClass;//
	UPROPERTY()
	USlashOverlay* SlashOverlay;

public:
	//这样就可以在别的地方（SlashCharacter.cpp）访问SlashOverlay了,通过GetSlashOverlay
	FORCEINLINE USlashOverlay* GetSlashOverlay()const { return SlashOverlay; }
	
};
