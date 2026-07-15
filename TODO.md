# TODO

## Follower System

- 使用 `source/characters/dwarf/` 的 idle、run、attack 帧创建 `Follower`。
- 为每个随从分配玩家后方的不同槽位，避免重叠。
- 复用当前导航网格与局部分离规则，敌人在攻击范围内时优先攻击最近目标。
- 将随从加入 `GameScene` 的更新、战斗和 Y 轴渲染排序。

当前版本暂不实现该系统，避免保留未接入工程的半成品类。
