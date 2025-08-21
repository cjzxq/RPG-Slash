// Fill out your copyright notice in the Description page of Project Settings.
//虚幻引擎用的是Unicode编码
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"//包含使我们类参与反射系统所需要的代码

class USphereComponent; //前向声明，

enum class EItemState : uint8//枚举类型，表示物品状态 比如武器该不该悬浮等
{
	EIS_Hovering, //悬浮状态
	EIS_Equipped //装备状态
};

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()//每次编译时都会生成的.h代码替换，从而增强AActor的功能
	
public:	
	// Sets default values for this actor's properties
	AItem();
	// Called every frame 调用每帧的函数
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//私有变量不能在蓝图读写，如果用额外的限定符也可以
	//UPROPERTY(VisibleAnywhere)意思是这个属性在编辑器中可见，但不能编辑 UPROPERTY只暴露给细节面板 UPROPERTY(EditAnywhere，BlueprintReadWrite)可以在蓝图事件中读取和写入这个属性
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sine Parameters");
	float TimeConstant = 5.f;//时间常数
	//下面一行代码 参与反射系统， 把浮点数属性暴露给ue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters");
	float Amplitude = 0.25f;//振幅
	//转换正弦函数，在蓝图中直接调用
	//设计了一个函数，要让蓝图可以直接使用这个函数，所以需要将其声明为public，不会改变类的任何属性，使用BlueprintPure
	UFUNCTION(BlueprintPure)
	float TransformedSin();
	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template <typename T>
	T Avg(T First, T Second);

	UFUNCTION() //为了在武器类中重写这些函数，把void OnSphereOverlap设置为虚函数
	//这个可以绑定到基本组件OnComponentBeginOverlap委托的回调函数。打算绑定一个动态多播委托
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()//为了在武器类中重写这些函数，把void OnSphereOverlap设置为虚函数
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//void on(FComponentEndOverlapSignature, UPrimitiveComponent, OnComponentEndOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex);)

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly);//暴露给蓝图，为了在事件图表中看见他，加了BlueprintReadOnly
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering; //物品状态，初始状态为悬浮 在protect中，从类外部是访问不到的，但子类比如武器类可以访问到这个变量

	UPROPERTY(VisibleAnywhere);
	USphereComponent* Sphere;//球形碰撞组件，用于检测物品的碰撞和交互 
	//按E键会发出拿到剑时的声音，需要在装备武器后关闭它的碰撞检测，现在要想访问 Sphere并且从子类中更改它的属性，本来是私有的，将其移到保护部分

	UPROPERTY(EditAnywhere);
	class UNiagaraComponent* EmbersEffect;//在cpp中构建

private:
	//可以在这里添加私有成员变量和函数
	//例如：UStaticMeshComponent* MeshComponent; //静态网格组件
	//例如：UBoxComponent* CollisionComponent; //碰撞组件
	//例如：UFUNCTION() void OnItemClicked(); //点击物品时调用的函数


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowprivateAccess = "true"));
	float RunningTime;//运行时间

	//UPROPERTY(VisibleAnywhere);//暴露给蓝图，蓝图可见的静态网格组件，但在蓝图事件中看不见，如果想在事件图表中看见他，用BlueprintReadOnly，
	//UStaticMeshComponent* ItemMesh;//物品的静态网格组件,创建了一个空指针，指向一个静态网格组件，接下来创建一个新的静态网格组件子对象，在cpp的构造函数中完成


};


//inline的意思是在编译时将函数的代码直接插入到调用处，而不是生成一个函数调用。无论在哪调用这个函数，函数调用将被替换为函数体的代码，就像宏一样
template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First+ Second)/2;
}
