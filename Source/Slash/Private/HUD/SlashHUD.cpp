// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"//Ϊ�˿���ʹ��USlashOverlay
void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller= World->GetFirstPlayerController();
		if (Controller && SlashOverlayClass)
		{
			//����һ��С������
			SlashOverlay = CreateWidget<USlashOverlay>(Controller,SlashOverlayClass);
			SlashOverlay->AddToViewport();//��ӵ��ӿ� ��ɫѪ��֮�����ӵ��ӿ�
		}
	}

}
