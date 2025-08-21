// Fill out your copyright notice in the Description page of Project Settings.
//��������õ���Unicode����
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"//����ʹ��������뷴��ϵͳ����Ҫ�Ĵ���

class USphereComponent; //ǰ��������

enum class EItemState : uint8//ö�����ͣ���ʾ��Ʒ״̬ ���������ò���������
{
	EIS_Hovering, //����״̬
	EIS_Equipped //װ��״̬
};

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()//ÿ�α���ʱ�������ɵ�.h�����滻���Ӷ���ǿAActor�Ĺ���
	
public:	
	// Sets default values for this actor's properties
	AItem();
	// Called every frame ����ÿ֡�ĺ���
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//˽�б�����������ͼ��д������ö�����޶���Ҳ����
	//UPROPERTY(VisibleAnywhere)��˼����������ڱ༭���пɼ��������ܱ༭ UPROPERTYֻ��¶��ϸ����� UPROPERTY(EditAnywhere��BlueprintReadWrite)��������ͼ�¼��ж�ȡ��д���������
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sine Parameters");
	float TimeConstant = 5.f;//ʱ�䳣��
	//����һ�д��� ���뷴��ϵͳ�� �Ѹ��������Ա�¶��ue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters");
	float Amplitude = 0.25f;//���
	//ת�����Һ���������ͼ��ֱ�ӵ���
	//�����һ��������Ҫ����ͼ����ֱ��ʹ�����������������Ҫ��������Ϊpublic������ı�����κ����ԣ�ʹ��BlueprintPure
	UFUNCTION(BlueprintPure)
	float TransformedSin();
	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template <typename T>
	T Avg(T First, T Second);

	UFUNCTION() //Ϊ��������������д��Щ��������void OnSphereOverlap����Ϊ�麯��
	//������԰󶨵��������OnComponentBeginOverlapί�еĻص������������һ����̬�ಥί��
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()//Ϊ��������������д��Щ��������void OnSphereOverlap����Ϊ�麯��
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//void on(FComponentEndOverlapSignature, UPrimitiveComponent, OnComponentEndOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex);)

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly);//��¶����ͼ��Ϊ�����¼�ͼ���п�����������BlueprintReadOnly
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering; //��Ʒ״̬����ʼ״̬Ϊ���� ��protect�У������ⲿ�Ƿ��ʲ����ģ������������������Է��ʵ��������

	UPROPERTY(VisibleAnywhere);
	USphereComponent* Sphere;//������ײ��������ڼ����Ʒ����ײ�ͽ��� 
	//��E���ᷢ���õ���ʱ����������Ҫ��װ��������ر�������ײ��⣬����Ҫ����� Sphere���Ҵ������и����������ԣ�������˽�еģ������Ƶ���������

	UPROPERTY(EditAnywhere);
	class UNiagaraComponent* EmbersEffect;//��cpp�й���

private:
	//�������������˽�г�Ա�����ͺ���
	//���磺UStaticMeshComponent* MeshComponent; //��̬�������
	//���磺UBoxComponent* CollisionComponent; //��ײ���
	//���磺UFUNCTION() void OnItemClicked(); //�����Ʒʱ���õĺ���


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowprivateAccess = "true"));
	float RunningTime;//����ʱ��

	//UPROPERTY(VisibleAnywhere);//��¶����ͼ����ͼ�ɼ��ľ�̬���������������ͼ�¼��п���������������¼�ͼ���п���������BlueprintReadOnly��
	//UStaticMeshComponent* ItemMesh;//��Ʒ�ľ�̬�������,������һ����ָ�룬ָ��һ����̬�������������������һ���µľ�̬��������Ӷ�����cpp�Ĺ��캯�������


};


//inline����˼���ڱ���ʱ�������Ĵ���ֱ�Ӳ��뵽���ô�������������һ���������á��������ĵ�������������������ý����滻Ϊ������Ĵ��룬�����һ��
template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First+ Second)/2;
}
