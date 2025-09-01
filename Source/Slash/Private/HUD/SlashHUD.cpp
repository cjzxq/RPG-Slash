// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"//为了可以使用USlashOverlay
void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller= World->GetFirstPlayerController();
		if (Controller && SlashOverlayClass)
		{
			//创建一个小部件，
			SlashOverlay = CreateWidget<USlashOverlay>(Controller,SlashOverlayClass);
			SlashOverlay->AddToViewport();//添加到视口 角色血量之类的添加到视口
		}
	}

}
