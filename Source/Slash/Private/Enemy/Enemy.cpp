// Fill out your copyright notice in the Description page of Project Settings.

//敌人类自带一个Mesh骨骼网格体组件， 在WeaponBox中，我们忽略了Pawn的碰撞对象检测，所以敌人不该把它的Mesh组件设为Pawn的对象类型， 设为WorldDynamic对象类型就可以了。
//想造成伤害时，可以使用U Gameplay Static的应用功能并传入相关信息
#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacors.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponents.h"//自己创建的，与.h中的前向声明对应
#include "Components/WidgetComponent.h"//UE自带的类。查文档知道还需要包含UMG才能使用
#include "Perception/PawnSensingComponent.h"//0821
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h" //
#include "Items/Weapons/Weapon.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);//设置敌人的Mesh组件的类型为WorldDynamic Object Type 是一个“标签”，告诉引擎“我是什么”
	//Box追踪是针对可见性通道的,下面这行代码可以实现网格体能被盒子追踪到
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);//当有可见性通道（ECC_Visibility） 的检测命中我时，我会阻挡（Block）这个检测
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); //相机碰到敌人时忽略敌人，
	//让网格体能产生重叠事件
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	//在c++中设置，需要#include "GameFramework/CharacterMovementComponent.h"0819
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

}

void AEnemy::PatrolTimerFinished()//设置了定时器，时间一到，就调用这个函数，移动到目标
{
	MoveToTarget(PatrolTarget);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	//不是一直都需要看见血条0817
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	//if (EnemyContorller && PatrolTarget)
	//{
	//	FAIMoveRequest MoveRequest;
	//	MoveRequest.GetGoalActor(PatrolTarget);
	//	MoveRequest.SetAcceptanceRadius(15.F);
	//	FNavPathSharedPtr NavPath;
	//	EnemyContorller->MoveTo(MoveRequest, &NavPath);
	//	//创建一个FNavPathPoint的数组
	//	TArray <FNavPathPoint>&PathPoints = NavPath->GetPathPoints();//描述导航路径中的节点
	//	for (auto& point : PathPoints)
	//	{
	//		const FVector = &Location = Point.Location;
	//		DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
	//	}
	//}
			//需包含AIModule和一个头文件AIController.h
	    EnemyContorller = Cast<AAIController>(GetController());
		MoveToTarget(PatrolTarget);
		//绑定回调函数
		if (PawnSensing)
		{
			PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);//
		}
	    //0823 生成武器
		UWorld* World = GetWorld();
		if (World&& WeaponClass)
		{
			AWeapon*DefalultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
			DefalultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);//装备武器到右手插槽
			EquippedWeapon = DefalultWeapon;
		}
}


void AEnemy::Die()
{
	//播放死亡蒙太奇
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);//播放装备蒙太奇
		//播放蒙太奇哪一个部分 随机选一个0816和slashcharacter.cpp中类似
		//但是血条消失完蒙太奇又站起来了，需要在ABP_Paladin中设置
		const int32 Section = FMath::RandRange(0, 5);//生成0到5之间的随机整数 因为有6个动画
		FName SectionName = FName();//生成攻击动画的名称,
		switch (Section)
		{
		case 0:
			SectionName = FName("Death1"); // 死亡动画1 把SectionName设为一个FName类型的变量 其中Attack1名字一定要与AK_AttackMontage的节名一致
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death1"); // 
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3"); //
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4"); //
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5"); // 
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5:
			SectionName = FName("Death6"); //
			DeathPose = EDeathPose::EDP_Death6;
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);//DeathMontage要在蓝图中设置，
	}
	//敌人死后，血条消失0817
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	//敌人死后还有碰撞 先获取敌人胶囊组件0817
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//敌人死后没有消失
	SetLifeSpan(3.f);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();//到目标的距离，0819
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	return DistanceToTarget<= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyContorller == nullptr || Target == nullptr)return;
	FAIMoveRequest MoveRequest;//创建一个移动请求
	//设置移动请求参数
	MoveRequest.SetGoalActor(Target);   //
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyContorller->MoveTo(MoveRequest);

}

//为了让这个函数能被调用，需要把它绑定,会在BeginPlay中完成操作 0821
void AEnemy::PawnSeen(APawn* SeenPawn)//没0.5s调用一次
{
	//一旦看见pawn就采取行动,不要让敌人之间相互追逐攻击
	//UE_LOG(LogTemp, Warning, TEXT("PawnSeen!"));
	if (EnemyState == EEnemyState::EES_Chasing) return;//这样不会一直调用MoveToTarget(CombatTarget)函数
	if (SeenPawn->ActorHasTag(FName("SlashCharacter")))//看见一个pawn,先检查它的标签
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);//不希望计时器追逐时触发
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;//设置战斗目标
		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);//敌人看见我们，就会跑过来
			//UE_LOG(LogTemp, Warning, TEXT("Attack"));
		}
	}
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*>ValidTargets;//把没有选中的巡逻目标加入这个数组
	for (AActor* target : PatrolTargets)//只要不是当前的巡逻目标，就加入数组  知道目标是角色的指针，不用auto，用AActor
	{
		if (target != PatrolTarget)
		{
			ValidTargets.AddUnique(target);
		}
	}
	//要从巡逻目标数组中选一个目标，随机挑选一个
	const int32 NumPatrolTargets = ValidTargets.Num();//获取巡逻目标数量
	if (NumPatrolTargets > 0)
	{
		const int32 SelectionTarget = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[SelectionTarget];//设置当前巡逻目标
		//PatrolTarget = Target;  本来是和上面连在一起的，在这个函数中没有必要0821
		//FAIMoveRequest MoveRequest;//创建一个移动请求
		////设置移动请求参数
		//MoveRequest.SetGoalActor(PatrolTarget);//
		//MoveRequest.SetAcceptanceRadius(15.f);
		//EnemyContorller->MoveTo(MoveRequest);
	}
	return nullptr;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//敌人到战斗目标的距离，太远就隐藏血条0817 敌人到战斗目标的向量
	//const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
	//if (DistanceToTarget > CombatRadius)
	if (EnemyState > EEnemyState::EES_Patrolling)//如果敌人状态等级>巡逻，检查战斗目标
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();//检查巡逻目标
	}


	//下面的代码意思被简化为上面的if中
	/*
	if (PatrolTarget && EnemyContorller)//0819
	{
		if (InTargetRange(PatrolTarget, PatrolRadius))//如果再巡逻范围内，
		{
			TArray<AActor*>ValidTargets;//把没有选中的巡逻目标加入这个数组
			for (AActor* target : PatrolTargets)//只要不是当前的巡逻目标，就加入数组  知道目标是角色的指针，不用auto，用AActor
			{
				if (target != PatrolTarget)
				{
					ValidTargets.AddUnique(target);
				}
			}
			//要从巡逻目标数组中选一个目标，随机挑选一个
			const int32 NumPatrolTargets = ValidTargets.Num();//获取巡逻目标数量
			if (NumPatrolTargets > 0)
			{
				const int32 SelectionTarget = FMath::RandRange(0, NumPatrolTargets - 1);
				AActor* Target = ValidTargets[SelectionTarget];//设置当前巡逻目标
				PatrolTarget = Target;
				FAIMoveRequest MoveRequest;//创建一个移动请求
				//设置移动请求参数
				MoveRequest.SetGoalActor(PatrolTarget);//
				MoveRequest.SetAcceptanceRadius(15.f);
				EnemyContorller->MoveTo(MoveRequest);
			}

		}
	}
	*/
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);//GetWorldTimerManager()返回的是一个结构体，不是指针0821
	}
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))//如果我们有战斗目标不在范围内
	{
		//如果在战斗半径之外，敌人失去兴趣0821
		CombatTarget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
	}
	//攻击范围外追逐角色 如果已经在追逐中，就要检查确保我们不是已经在追逐中
	else if (!InTargetRange(CombatTarget, AttackRadius)&& EnemyState !=EEnemyState::EES_Chasing)
	{
		//攻击范围外追逐角色
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
	}
	//在攻击范围内
	else if (InTargetRange(CombatTarget, AttackRadius)&&EnemyState!=EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Attacking;
		//攻击蒙太奇

	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE(ImpactPoint); //在敌人被击中时绘制一个红色的球体，半径为25，持续30秒
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
	//是否还有健康值，
	if (Attributes && Attributes->IsAlive())//0816
	{
		DirectionalHitReact(ImpactPoint);//方向受击反应
	}
	else
	{
		//播放死亡蒙太奇,
		Die();
	}


	// 想要在挥剑击中角色时只调用一次GetHit，回到OnBoxOverlap,可能是盒子与一个角色反复重叠多次调用GetHit，可以使用里面的ActorsToIgnore.Add(this); 

	//要播放声音，用到GamePlayStatics,需要导入头文件 
	if (HitSound)//HitSound需要早蓝图中设置，否则为空0809
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
	//显示粒子 0809
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		HitParticles,
		ImpactPoint//显示血液的地方
		);
	}
}

//伤害敌人 weapon.cpp挥剑的时候
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//伤害敌人时，更新健康条控件
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReciveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());//设置血量
		
	}
	CombatTarget=EventInstigator->GetPawn();//0817 pawn是actor子类，可以将子类地址存储在父类指针中 后续在tick（）中
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MoveToTarget(CombatTarget);

	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy(); 
	}
}

