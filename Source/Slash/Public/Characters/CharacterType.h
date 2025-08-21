#pragma once

UENUM(BluePrintType)
enum class ECharacterState : uint8//�޷���ö�����ͣ�û�и�����uint8��ʾʹ��8λ�޷��������洢ö��ֵ
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"), //��UMETA����ͼ����ʾ������Unequipped��������ECS_Unequipped
	ECS_EquippedOneHandWeapon UMETA(DisplayName = "Equipped One Hand Weapon"),
	ECS_EquippedTwoHandWeapon UMETA(DisplayName = "Equipped Two Hand Weapon"),

};

//׷�ٽ�ɫ��״̬�������Ƿ�װ�����������Ƿ���ս��״̬��
UENUM(BluePrintType)//û������꣬��ͼ�о��޷�ʹ�����ö������
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),//����״̬������ִ�ж���
	EAS_Attacking UMETA(DisplayName = "Attacking"),//����״̬�����ڹ���������ִ����������
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")//װ��״̬������װ������
};
//��������,Ҫʹ������ö�٣�����Enmy.h�м������ͷ�ļ�0816
UENUM(BluePrintType)//û������꣬��ͼ�о��޷�ʹ�����ö������
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),//����״̬������ִ�ж���
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6")
};

//����״̬0821  �����˳����������ԭ��ģ�ÿ��ö�ٳ�������һ������ֵ��Ĭ����0 1 2
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),//Ѳ��
	EES_Chasing UMETA(DisplayName = "Chasing"),//׷��
	EES_Attacking UMETA(DisplayName = "Attacking")//����״̬�����ڹ���������ִ����������

};
