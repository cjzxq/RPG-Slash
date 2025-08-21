#pragma once

UENUM(BluePrintType)
enum class ECharacterState : uint8//无符号枚举类型，没有负数，uint8表示使用8位无符号整数存储枚举值
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"), //用UMETA在蓝图中显示的名称Unequipped，而不是ECS_Unequipped
	ECS_EquippedOneHandWeapon UMETA(DisplayName = "Equipped One Hand Weapon"),
	ECS_EquippedTwoHandWeapon UMETA(DisplayName = "Equipped Two Hand Weapon"),

};

//追踪角色的状态，比如是否装备了武器，是否在战斗状态等
UENUM(BluePrintType)//没有这个宏，蓝图中就无法使用这个枚举类型
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),//空闲状态，可以执行动作
	EAS_Attacking UMETA(DisplayName = "Attacking"),//攻击状态，正在攻击，不能执行其他动作
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")//装备状态，正在装备武器
};
//死亡姿势,要使用以下枚举，先在Enmy.h中加上这个头文件0816
UENUM(BluePrintType)//没有这个宏，蓝图中就无法使用这个枚举类型
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),//空闲状态，可以执行动作
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6")
};

//敌人状态0821  按这个顺序排列是有原因的，每个枚举常量都有一个整数值，默认是0 1 2
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),//巡逻
	EES_Chasing UMETA(DisplayName = "Chasing"),//追逐
	EES_Attacking UMETA(DisplayName = "Attacking")//攻击状态，正在攻击，不能执行其他动作

};
