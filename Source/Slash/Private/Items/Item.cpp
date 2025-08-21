// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacors.h"
#include "Components/SphereComponent.h"//包含球形碰撞组件的头文件
#include "Characters/SlashCharacter.h"//包含角色类的头文件
#include "NiagaraComponent.h"

 
// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;//启用或禁用角色的Tick功能

	//创建一个新的静态网格组件子对象，这个函数会返回一个指向新创建的组件的指针，并将其赋值给ItemMesh成员变量
	ItemMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));//创建一个静态网格组件子对象，名称为ItemMeshComponent,

	RootComponent = ItemMesh;//将ItemMesh组件设置为根组件，这样它就会成为这个Actor的根组件

	//创建一个新的球形碰撞组件子对象，这个函数会返回一个指向新创建的组件的指针，并将其赋值给Sphere成员变量
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));//创建一个球形碰撞组件子对象，名称为Sphere
	Sphere->SetupAttachment(GetRootComponent());//将Sphere组件附加到根组件上，这样它就会成为ItemMesh的子组件，在蓝图中可以看到Sphere组件是ItemMesh的子组件

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	EmbersEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();//正在调用父类的BeginPlay函数
	//在BeginPlay绑定委托，因为构造函数执行的太早，那时候组件还没有创建完成，所以不能在构造函数中绑定委托。只能在BeginPlay中绑定委托。
	//游戏开始时，回调绑定到Sphere组件的OnComponentBeginOverlap事件，然后就对Sphere组件的重叠事件进行处理。执行AItem::OnSphereOverlap函数，
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);//绑定Sphere组件的OnComponentBeginOverlap事件到OnSphereOverlap函数，这样当Sphere组件与其他物体发生重叠时，就会调用OnSphereOverlap函数
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);//绑定Sphere组件的OnComponentEndOverlap事件到OnSphereEndOverlap函数，这样当Sphere组件与其他物体结束重叠时，就会调用OnSphereEndOverlap函数
	int Avgint = Avg<int32>(1, 3);//调用Avg函数，计算两个整数的平均值
	UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3 is: %d"), Avgint);//输出日志，显示平均值
	//向输出打印日志，会用到UE_LOG宏
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay called!"));//LogTemp是一个临时日志类别，Warning是日志级别，TEXT宏用于处理字符串
	//判断空指针 添加到屏幕的调试信息函数 
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("Item OnScreen Message!"));
	//}

	//UWorld* World = GetWorld();//获取当前世界
	//UE中蓝图会发生在c++代码之前
	//SetActorLocation(FVector(0.f,0.f,50.f));
	//SetActorRotation(FRotator(0.f, 90.f, 0.f));
	//FVector Location = GetActorLocation();//获取当前物体的位置
	//FVector Forward = GetActorForwardVector();//获取当前物体的前向向量
	//DRAW_SPHERE(Location);//使用宏绘制一个球体
	//下面两个宏是用来绘制线和点的，直接被DRAW_VECTOR合为了一个宏
	//DRAW_LINE(Location, Location + Forward * 100.f);//使用宏绘制一条线，从当前位置到前向向量的100单位处
	//DRAW_POINT(Location + Forward * 100.f);//使用宏绘制一个点，位置为当前位置加上前向向量的100单位处
	//DRAW_VECTOR(Location, Location + Forward * 100.f);//使用宏绘制一个向量

	//if (World)
	//{
	//	FVector Location=GetActorLocation();//创建一个FVector对象，表示位置
	//	//在世界中绘制一个红色的球体，位置为(0,0,0)，半径为25，持续时间为30秒 为了不用写这么多参数，可以使用一个宏  
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

//只要于球体组件发生重叠时调用的函数，委托会遍历其委托列表，并调用所有绑定在其上的回调函数
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//以下几行是调试信息，重叠时，可以在屏幕上显示其他物体的名称和输出日志
	//const FString OtherActorName = OtherActor->GetName();//获取当前物体的名称
	//if (GEngine)
	//{
	//	//发生重叠时，打印另一个角色的名字，将其他物体的名称添加到屏幕上，持续60秒，颜色为青色
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, OtherActorName);
	//	//输出日志，显示其他物体的名称
	//	UE_LOG(LogTemp, Warning, TEXT("Overlapped with: %s"), *OtherActorName);
	//}

	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);//将OtherActor转换为ASlashCharacter类型 人物靠近武器， 人物就是OtherActor
	if (SlashCharacter)
	{
		//如果转换成功，说明OtherActor是ASlashCharacter类型的角色，那么就调用角色的SetOverlappingItem函数，将当前物体（this）传递给角色
		SlashCharacter->SetOverlappingItem(this);//需要传入一个物品指针，重叠的物品就是当前物体（this），this指向当前AItem对象
	}

}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//以下几行是调试信息，不重叠时，可以在屏幕上显示其他物体的名称和输出日志
	//const FString OtherActorName = FString("Ending Overlap With:") + OtherActor->GetName();//获取当前物体的名称
	//if (GEngine)
	//{
	//	//发生重叠结束时，打印另一个角色的名字，将其他物体的名称添加到屏幕上，持续60秒，颜色为青色
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue, OtherActorName);
	//	//输出日志，显示其他物体的名称
	//	UE_LOG(LogTemp, Warning, TEXT("End Overlap with: %s"), *OtherActorName);
	//}

	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);//将OtherActor转换为ASlashCharacter类型 人物靠近武器， 人物就是OtherActor
	if (SlashCharacter)
	{
		//人没有到武器范围时，传入一个空指针
		SlashCharacter->SetOverlappingItem(nullptr);//
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Tick called! DeltaTime: %f"), DeltaTime);//每帧调用Tick函数，DeltaTime是上一帧到当前帧的时间差
	//if (GEngine)
	//{
	//	//字符串格式化函数FString::Printf用于格式化字符串
	//	FString Name = GetName();//获取当前对象的名称
	//	//格式化字符串，将对象名称插入到固定文本中。%s：字符串占位符，会被*Name的值替换.*Name：FString重载了*运算符，返回底层TCHAR*（C风格字符串指针），这是Printf需要的类型。
	//	FString Message = FString::Printf(TEXT("Item Name: %s"), *Name);
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);
	//	UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);
	//}
	 


	// 下面两个函数可以让角色每秒移动50厘米，每秒旋转45度 用移动速度和旋转速度来控制角色的移动和旋转
	//AddActorWorldOffset(FVector(1.f, 0.f, 0.f));//每帧向x移动1单位，，但是帧率的变化会影响角色速度的变化，大量计算时，帧率会下降，所以最好使用Tick函数的DeltaTime参数来计算移动距离
	//float MoveMentRate = 50.f;//移动速度 单位是50cm/s,而DeltaTime的单位是每帧秒速 ，所以MoveMentRate * DeltaTime是（cm/s）*(s/frame)= cm/frame DeltaTime 
	//float RotationRate = 45.f;//旋转速度 单位是45度/s,而DeltaTime的单位是每帧秒速 ，所以RotationRate * DeltaTime是（度/s）*(s/frame)=度/frame
	//是当前帧和上一帧之间的时间间隔。60 FPS	0.01667 秒（即 1/60）这样实现无论帧率多少，每秒移动50厘米。  
	//AddActorWorldOffset(FVector(MoveMentRate * DeltaTime, 0.f, 0.f));//每帧向x移动1单位，DeltaTime是上一帧到当前帧的时间差，这样可以保证角色速度的稳定
	//AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));//每帧向y旋转1单位，DeltaTime是上一帧到当前帧的时间差，这样可以保证角色速度的稳定


	RunningTime += DeltaTime;//每帧增加运行时间
	//float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);//意思是每秒钟增加0.25f*Sin(5*RunningTime)的高度变化，其中。*5f是为了让Sin函数的周期更短，period = 2π/5 = 1.2566秒
	//AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));//每帧向z轴移动DeltaZ单位，这样可以实现上下浮动的效果  
	//DRAW_SPHERE_SingleFrame(GetActorLocation());//绘制一个球体在当前物体的位置上
	//DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);//绘制一个向量从当前位置到前向向量的100单位处

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));//每帧向z轴移动Z单位，这样可以实现上下浮动的效果
	}

}

