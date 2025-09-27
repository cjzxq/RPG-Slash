AI 架构设计

分层次 AI（感知层 → 决策层 → 行为层），支持灵活扩展新行为

使用 UAIController 管理 NPC 的感知与状态切换

组件化架构

战斗逻辑独立在 CombatComponent 中，便于角色与 NPC 共享

技能与 Buff 系统模块化，支持叠加与移除

动画驱动战斗

AnimMontage 与 AnimNotify 同步技能释放与命中判定

BlendSpace 控制移动/攻击过渡，保证流畅衔接

数据驱动

使用 DataTable 管理 NPC 属性与技能参数

策划可直接在表格中配置数值，无需修改代码
