// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterType.h"//自己创建的头文件，包含了角色类型枚举，比放一个巨大的头文件要好很多
#include "SlashCharacter.generated.h"

class USpringArmComponent;//前向声明，弹簧臂组件，用于摄像机跟随物体
class UCameraComponent;//前向声明，摄像机组件，用于显示的视角
class UGroomComponent;//前向声明，角色的毛发
class AItem;//前向声明，物品类
class UAnimMontage;//前向声明，动画蒙太奇类
class AWeapon;//前向声明，武器类

//以下是c++的枚举类型定义
//enum CharacterState
//{
//	Unequipped,
//	EquippedOneHandWeapon,
//	EquippedTwoHandWeapon,
//};
//在私有的枚举类型中定义角色状态，表示角色当前的装备状态 例如CharacterState State= Unequipped;
//必须有E，作用域枚举，跟踪状态，一般用枚举类型来表示状态，比如是否装备了武器，是否在战斗状态等 ECS_命名规范表示CharacterState的缩写


UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled);//启用武器碰撞
	//UFUNCTION(BlueprintCallable)
	//void DisableWeaponCollision();//禁用武器碰撞

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 输入的回调函数
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void MoveRightAndLeft(float Value);//向左和向右移动 轴映射，需要浮点型输入
	void EkeyPressed();//E键按下事件，动作映射，不需要浮点型输入，函数是一次性的，按下E键就会调用一次这个函数
	void Attack();//攻击函数，绑定到鼠标左键按下事件

	// 播放蒙太奇动画
	void PlayAttackMontage();
	UFUNCTION(BlueprintCallable)//蓝图可调用
	void AttackEnd();//攻击结束函数
	bool CanAttack();//是否可以攻击的函数，判断角色状态和动作状态

	void PlayEquipMontage(const FName& SectionName);//播放装备武器的蒙太奇动画
	bool CanDisarm();//是否可以卸下武器的函数，把武器放到背上
	bool CanArm();//把武器从背上拿到手里

	UFUNCTION(BlueprintCallable)//蓝图可调用
	void Disarm();	//解除武器，把武器从手上卸下，放到背上

	UFUNCTION(BlueprintCallable)
	void Arm();	//装备武器，把武器从背上拿到手里

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();//完成装备武器的函数，设置动作状态为空闲

private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;//角色状态，初始状态为未装备 
	UPROPERTY(BlueprintReadWrite,Meta=(AllowPrivateAccess="true")) //无法直接将私有变量设为蓝图读写，除非给他加个Meta指定符，允许蓝图读写这个变量
	EActionState ActionState = EActionState::EAS_Unoccupied;//动作状态，初始状态为空闲 ,然后去cpp中判断

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;//弹簧臂组件，用于摄像机跟随物体

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;//摄像机组件，用于显示echo的视角

	UPROPERTY(VisibleAnywhere,Category=Hair)
	UGroomComponent* Hair;//毛发组件，用于显示角色的头发

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;//眉毛组件，用于显示角色的胡须

	UPROPERTY(VisibleInstanceOnly)//在游戏世界看见这个值，在蓝图中看不见这个值
	AItem* OverlappingItem;//如果这个变量不为nullptr，表示角色正在与一个物品重叠，可以尝试捡起来

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;//当前装备的武器

	//蒙太奇动画，变量
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;//攻击动作蒙太奇

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;//装备武器蒙太奇

public:
	//设置重叠的物品 内敛函数效率更高，每次调用函数，程序执行都要跳转到函数代码定义的地方，但只用于简单小的函数，通常是获取函数和设置函数 FORCEINLINE强制内敛
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; } 
	//
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; } //获取角色状态

};
