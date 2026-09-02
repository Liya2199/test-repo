# scripts/ — 趣味脚本集（WIP）

> 状态：WIP 草稿分支，**不参与构建、不进 CI**，以可读性和趣味性优先。
> 每个脚本独立成篇，标准库/零依赖优先，"能跑当然好，读着有趣也行"。

| 文件 | 语言 | 联动对象 | 说明 |
| --- | --- | --- | --- |
| `inventory_total.py` | Python | `inventory-2026-08-30.md` | 直接解析食品库存 markdown 表格，重算小计/合计并核对账实 |
| `proportional_navigation.cpp` | C++17 | 战雷档案（导弹视角） | 比例导引（PN）二维仿真：视线转率 × 导航比，恒速拦截 |
| `Airframe.cs` | Unity C# | 战雷档案（战机视角） | 极简战机机体组件：推力/滚转/俯仰 + 受损坠毁判定 |
| `br_matchmaker.rs` | Rust | 战雷档案（分房视角） | 贪心法把排队玩家按 BR 均衡分进两队 |

## 约定

- 不追求工程完整度：无测试、无构建脚本、无错误处理全家桶；
- 想跑 Python 那个：`python scripts/inventory_total.py`（唯一与仓库文件实时联动的）；
- C++ / Rust 只配了可选的一行编译注释，不跑也很好读。
