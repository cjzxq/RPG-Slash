// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacors.h"
#include "Components/SphereComponent.h"//����������ײ�����ͷ�ļ�
#include "Characters/SlashCharacter.h"//������ɫ���ͷ�ļ�
#include "NiagaraComponent.h"

 
// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;//���û���ý�ɫ��Tick����

	//����һ���µľ�̬��������Ӷ�����������᷵��һ��ָ���´����������ָ�룬�����丳ֵ��ItemMesh��Ա����
	ItemMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));//����һ����̬��������Ӷ�������ΪItemMeshComponent,

	RootComponent = ItemMesh;//��ItemMesh�������Ϊ��������������ͻ��Ϊ���Actor�ĸ����

	//����һ���µ�������ײ����Ӷ�����������᷵��һ��ָ���´����������ָ�룬�����丳ֵ��Sphere��Ա����
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));//����һ��������ײ����Ӷ�������ΪSphere
	Sphere->SetupAttachment(GetRootComponent());//��Sphere������ӵ�������ϣ��������ͻ��ΪItemMesh�������������ͼ�п��Կ���Sphere�����ItemMesh�������

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	EmbersEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();//���ڵ��ø����BeginPlay����
	//��BeginPlay��ί�У���Ϊ���캯��ִ�е�̫�磬��ʱ�������û�д�����ɣ����Բ����ڹ��캯���а�ί�С�ֻ����BeginPlay�а�ί�С�
	//��Ϸ��ʼʱ���ص��󶨵�Sphere�����OnComponentBeginOverlap�¼���Ȼ��Ͷ�Sphere������ص��¼����д���ִ��AItem::OnSphereOverlap������
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);//��Sphere�����OnComponentBeginOverlap�¼���OnSphereOverlap������������Sphere������������巢���ص�ʱ���ͻ����OnSphereOverlap����
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);//��Sphere�����OnComponentEndOverlap�¼���OnSphereEndOverlap������������Sphere�����������������ص�ʱ���ͻ����OnSphereEndOverlap����
	int Avgint = Avg<int32>(1, 3);//����Avg��������������������ƽ��ֵ
	UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3 is: %d"), Avgint);//�����־����ʾƽ��ֵ
	//�������ӡ��־�����õ�UE_LOG��
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay called!"));//LogTemp��һ����ʱ��־���Warning����־����TEXT�����ڴ����ַ���
	//�жϿ�ָ�� ��ӵ���Ļ�ĵ�����Ϣ���� 
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("Item OnScreen Message!"));
	//}

	//UWorld* World = GetWorld();//��ȡ��ǰ����
	//UE����ͼ�ᷢ����c++����֮ǰ
	//SetActorLocation(FVector(0.f,0.f,50.f));
	//SetActorRotation(FRotator(0.f, 90.f, 0.f));
	//FVector Location = GetActorLocation();//��ȡ��ǰ�����λ��
	//FVector Forward = GetActorForwardVector();//��ȡ��ǰ�����ǰ������
	//DRAW_SPHERE(Location);//ʹ�ú����һ������
	//���������������������ߺ͵�ģ�ֱ�ӱ�DRAW_VECTOR��Ϊ��һ����
	//DRAW_LINE(Location, Location + Forward * 100.f);//ʹ�ú����һ���ߣ��ӵ�ǰλ�õ�ǰ��������100��λ��
	//DRAW_POINT(Location + Forward * 100.f);//ʹ�ú����һ���㣬λ��Ϊ��ǰλ�ü���ǰ��������100��λ��
	//DRAW_VECTOR(Location, Location + Forward * 100.f);//ʹ�ú����һ������

	//if (World)
	//{
	//	FVector Location=GetActorLocation();//����һ��FVector���󣬱�ʾλ��
	//	//�������л���һ����ɫ�����壬λ��Ϊ(0,0,0)���뾶Ϊ25������ʱ��Ϊ30�� Ϊ�˲���д��ô�����������ʹ��һ����  
	//	DrawDebugSphere(World, Location, 25.f, 24, FColor::Red, false, 30.f);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("World is null!"));
	//}


}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

//ֻҪ��������������ص�ʱ���õĺ�����ί�л������ί���б����������а������ϵĻص�����
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���¼����ǵ�����Ϣ���ص�ʱ����������Ļ����ʾ������������ƺ������־
	//const FString OtherActorName = OtherActor->GetName();//��ȡ��ǰ���������
	//if (GEngine)
	//{
	//	//�����ص�ʱ����ӡ��һ����ɫ�����֣������������������ӵ���Ļ�ϣ�����60�룬��ɫΪ��ɫ
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, OtherActorName);
	//	//�����־����ʾ�������������
	//	UE_LOG(LogTemp, Warning, TEXT("Overlapped with: %s"), *OtherActorName);
	//}

	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);//��OtherActorת��ΪASlashCharacter���� ���￿�������� �������OtherActor
	if (SlashCharacter)
	{
		//���ת���ɹ���˵��OtherActor��ASlashCharacter���͵Ľ�ɫ����ô�͵��ý�ɫ��SetOverlappingItem����������ǰ���壨this�����ݸ���ɫ
		SlashCharacter->SetOverlappingItem(this);//��Ҫ����һ����Ʒָ�룬�ص�����Ʒ���ǵ�ǰ���壨this����thisָ��ǰAItem����
	}

}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//���¼����ǵ�����Ϣ�����ص�ʱ����������Ļ����ʾ������������ƺ������־
	//const FString OtherActorName = FString("Ending Overlap With:") + OtherActor->GetName();//��ȡ��ǰ���������
	//if (GEngine)
	//{
	//	//�����ص�����ʱ����ӡ��һ����ɫ�����֣������������������ӵ���Ļ�ϣ�����60�룬��ɫΪ��ɫ
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue, OtherActorName);
	//	//�����־����ʾ�������������
	//	UE_LOG(LogTemp, Warning, TEXT("End Overlap with: %s"), *OtherActorName);
	//}

	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);//��OtherActorת��ΪASlashCharacter���� ���￿�������� �������OtherActor
	if (SlashCharacter)
	{
		//��û�е�������Χʱ������һ����ָ��
		SlashCharacter->SetOverlappingItem(nullptr);//
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Tick called! DeltaTime: %f"), DeltaTime);//ÿ֡����Tick������DeltaTime����һ֡����ǰ֡��ʱ���
	//if (GEngine)
	//{
	//	//�ַ�����ʽ������FString::Printf���ڸ�ʽ���ַ���
	//	FString Name = GetName();//��ȡ��ǰ���������
	//	//��ʽ���ַ��������������Ʋ��뵽�̶��ı��С�%s���ַ���ռλ�����ᱻ*Name��ֵ�滻.*Name��FString������*����������صײ�TCHAR*��C����ַ���ָ�룩������Printf��Ҫ�����͡�
	//	FString Message = FString::Printf(TEXT("Item Name: %s"), *Name);
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);
	//	UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);
	//}
	 


	// �����������������ý�ɫÿ���ƶ�50���ף�ÿ����ת45�� ���ƶ��ٶȺ���ת�ٶ������ƽ�ɫ���ƶ�����ת
	//AddActorWorldOffset(FVector(1.f, 0.f, 0.f));//ÿ֡��x�ƶ�1��λ��������֡�ʵı仯��Ӱ���ɫ�ٶȵı仯����������ʱ��֡�ʻ��½����������ʹ��Tick������DeltaTime�����������ƶ�����
	//float MoveMentRate = 50.f;//�ƶ��ٶ� ��λ��50cm/s,��DeltaTime�ĵ�λ��ÿ֡���� ������MoveMentRate * DeltaTime�ǣ�cm/s��*(s/frame)= cm/frame DeltaTime 
	//float RotationRate = 45.f;//��ת�ٶ� ��λ��45��/s,��DeltaTime�ĵ�λ��ÿ֡���� ������RotationRate * DeltaTime�ǣ���/s��*(s/frame)=��/frame
	//�ǵ�ǰ֡����һ֮֡���ʱ������60 FPS	0.01667 �루�� 1/60������ʵ������֡�ʶ��٣�ÿ���ƶ�50���ס�  
	//AddActorWorldOffset(FVector(MoveMentRate * DeltaTime, 0.f, 0.f));//ÿ֡��x�ƶ�1��λ��DeltaTime����һ֡����ǰ֡��ʱ���������Ա�֤��ɫ�ٶȵ��ȶ�
	//AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));//ÿ֡��y��ת1��λ��DeltaTime����һ֡����ǰ֡��ʱ���������Ա�֤��ɫ�ٶȵ��ȶ�


	RunningTime += DeltaTime;//ÿ֡��������ʱ��
	//float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);//��˼��ÿ��������0.25f*Sin(5*RunningTime)�ĸ߶ȱ仯�����С�*5f��Ϊ����Sin���������ڸ��̣�period = 2��/5 = 1.2566��
	//AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));//ÿ֡��z���ƶ�DeltaZ��λ����������ʵ�����¸�����Ч��  
	//DRAW_SPHERE_SingleFrame(GetActorLocation());//����һ�������ڵ�ǰ�����λ����
	//DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);//����һ�������ӵ�ǰλ�õ�ǰ��������100��λ��

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));//ÿ֡��z���ƶ�Z��λ����������ʵ�����¸�����Ч��
	}

}

