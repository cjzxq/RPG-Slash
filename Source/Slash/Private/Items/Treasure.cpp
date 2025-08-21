// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Characters/SlashCharacter.h"//������ɫ���ͷ�ļ�
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//����Ķ��������಻��Ҫʹ�ã�����super  Ϊʲô��������������ˣ����ߵ�������ʲô��0810
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);//��OtherActorת��ΪASlashCharacter���� ���￿�������� �������OtherActor
	if (SlashCharacter)
	{
		//����Ӳ�� ��������
		if (PickupSound)//HitSound��Ҫ����ͼ�����ã�����Ϊ��0809
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}

		Destroy();//����ʰȡ��Ʒ
	}
	//��һ�����������壬���ֱ���0810 
}
