// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Interfaces/HitInterfaces.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox")); //����һ������������ײ���0806
	WeaponBox->SetupAttachment(GetRootComponent()); //���ӵ��������
	//������ײԤ��0806
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);//��ײ��������Ϊ����ײ���ڹ�����ʱ��Ż�������ײ��⣬�����������ٽ�����ײ���
	//������ײ��Ӧ0806
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); //����ͨ������ײ��Ӧ����Ϊ�ص�
	//���õ���ͨ������ײ��Ӧ0806
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); //Pawnͨ������ײ��Ӧ����Ϊ����
	//��������Ҫ�����ص��¼�
	// 
	//�ȴ������������յ㣬Ȼ������ͼ���������ǵ�λ�ã���Ӧ���������յ�
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start")); //����һ������������
	BoxTraceStart->SetupAttachment(GetRootComponent()); //���ӵ��������

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End")); ///����һ�������յ����
	BoxTraceEnd->SetupAttachment(GetRootComponent()); //���ӵ��������

	//׼�����к���׷��

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();//��Ϊ��дBeginPlay()������������Ҫ���ø����BeginPlay()����0806
	//�ں���������ص�ʱ����OnBoxOverlap����
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap); //���ص��¼���OnBoxOverlap����
}


void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstiGator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstiGator);//0812

	//���������ӵ�ָ���ĸ�����Ͳ����
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped; //������Ʒ״̬Ϊ��װ��  ���������Ͳ�����Ϊ����״̬��
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation()); //����װ������ʱ������
		//��һ��װ������ʱ�������������ٺ��水E��ʱ���ǻ�����������Ϊ��ɫ�ֺ�������⵽���ص�����slashcharacter.cpp�е�:EkeyPressed()������
		// OverlappingWeapon��ֵ�ˣ����Ի����Equip���� һ����װ������������Ӧ�ý���������ײ���
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); //������ײ���
		//Sphere->SetGenerateOverlapEvents(false); //�����ص��¼�
	}
	if (EmbersEffect)
	{
		EmbersEffect->Deactivate();//ͣ�û������Ч
	}

}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)//���������嵽�����
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true); //����һ�����ӱ任����ʹ��SnapToTarget���򣬱�������λ�ú���ת������ģ������
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName); //���������ӵ�ָ���ĸ�����Ͳ����
}

//�ڸ����У������������������ص�ʱ���ӡ�������֣����Ҫ���ø��ຯ��������super:::OnSphereOverlap����
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//�����ǵ������ص����Զ���������
	//ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);//��OtherActorת��ΪASlashCharacter����
	//if (SlashCharacter)
	//{
	//	// �������ϵ���Ʒ���񣬸��ӵ�SlashCharacter�Ĺ��������� SnapToTarget������Ŀ����
	//	//FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true); //����һ�����ӱ任����ʹ��SnapToTarget���򣬱�������λ�ú���ת������ģ������
	//	//��������������������ӵ�SlashCharacter��ɫ��RightHnadSocket�����
	//	ItemMesh->AttachToComponent(SlashCharacter->GetMesh(), TransformRules, FName("RightHandSocket")); //���������ӵ���ɫ�����������
	//}

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

//
void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation(); //��ȡ��������λ��,�õ���������ռ��ȫ����ת�����Ǳ��ؿռ䣬���ؿռ���GetRelativeLocation ����ͼ�У�����getworldlocation,
	const FVector End = BoxTraceEnd->GetComponentLocation(); //��ȡ�����յ��λ��

	//��һ�������еĽ�ɫ�б���ֹ�ٴλ������ǣ�
	TArray<AActor*>ActorsToIgnore; //����һ�����Ե�Actor����  TArrayһ��������
	ActorsToIgnore.Add(this); //����ǰ������ӵ����Ե�Actor�����У������Ͳ����⵽�Լ� ���������������

	for (AActor* Actor : IgnoreActors)//���Ա������ԵĽ�ɫ�� 
	{
		ActorsToIgnore.AddUnique(Actor);//û����һ����ɫ���Ͳ�����Ǹ���ɫ����box trace��0808
	}

	FHitResult BoxHit; //����һ����ײ������� Ҳ�������н����
	//����׷�ٺ���������T�����ų�һЩ��ɫ0807 ���к���׷�٣�����Ƿ��������������ײ
	UKismetSystemLibrary::BoxTraceSingle(
		this, //�����Ķ���
		Start, //���
		End, //�յ�
		FVector(5.f, 5.f, 5.f), //���Ӵ�С
		BoxTraceStart->GetComponentRotation(), //������ת
		ETraceTypeQuery::TraceTypeQuery1, //׷�����Ͳ�ѯ������ʹ��TraceTypeQuery1
		false, //�����и���׷��
		ActorsToIgnore,
		EDrawDebugTrace::None, //ForDuratio ��ʾ����һ��ʱ��
		BoxHit, //��ײ���
		true //�Ƿ񷵻���ײ���
	);
	if (BoxHit.GetActor()) //�����ײ�����Actor  һ������ĳ�����壬�Ͱѻ��еĶ���תΪ�ӿڣ����˵ʵ��������ӿڻ���˵������ӿڼ̳й������࣬�Ϳ��Ե��ýӿں���
	{
		//�˺����ˡ�Ȼ��ȥEnemy.cpp�е���  ���˺����ˣ�������HitInterface->Execute_GetHit(BoxHit.GetActor(),BoxHit.ImpactPoint);������ȷ�ж���û�н���ֵ
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		//����ڱ����еĽ�ɫ�ϵ��ýӿں���   HitInterface��һ��ָ�� IHitInterfaces �ӿڵ�ָ�����  
		//���ת���ɹ����ͱ�ʾ�����е���ʵ���˽ӿڵ�ACTOR�����Ե��ýӿں���  ������ô��������Ϊ���˱������һ�� IHitInterfaces�Ķ���  ������һ��Actor������һ��IHitInterfaces
		IHitInterfaces* HitInterface = Cast<IHitInterfaces>(BoxHit.GetActor()); //�������е�Actorת��ΪIHitInterfaces�ӿ�����
		if (HitInterface)
		{
			//HitInterface->GetHit(BoxHit.ImpactPoint);����ԭ���¼����� ��Ҫ��õķ���ϵͳ����������ӿ�
			HitInterface->Execute_GetHit(BoxHit.GetActor(),BoxHit.ImpactPoint);//������ͼԭ���¼�ʱ��Execute_GetHit������ͼԭ���¼�ʱ���Զ�����0810
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());//����֪����������Щ��ɫ0808

		CreatFields(BoxHit.ImpactPoint);
	}
}

