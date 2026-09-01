# Git 全流程实战教程

> 本教程由 Odysseus 依据 test-repo 仓库的真实开发历史整理，
> 所有示例均来自本仓库的真实提交（可对照 `git log` 逐条验证）。
> 记录日期：2026-09-01

## 目录

1. [初始化仓库](#1-初始化仓库)
2. [首次提交与提交信息规范](#2-首次提交与提交信息规范)
3. [关联远程仓库并推送](#3-关联远程仓库并推送)
4. [日常循环：修改 → diff → 提交](#4-日常循环修改--diff--提交)
5. [撤销的四种姿势](#5-撤销的四种姿势)
6. [分支开发全流程](#6-分支开发全流程)
7. [远程分支与 Pull Request](#7-远程分支与-pull-request)
8. [冲突的产生与解决](#8-冲突的产生与解决)
9. [git stash：临时口袋](#9-git-stash临时口袋)
10. [tag 与 Release](#10-tag-与-release)
11. [.gitignore 详解](#11-gitignore-详解)
12. [多身份提交（模拟多人协作）](#12-多身份提交模拟多人协作)
13. [常用命令速查表](#13-常用命令速查表)
14. [本仓库真实提交历史（案例）](#14-本仓库真实提交历史案例)

---

## 1. 初始化仓库

```bash
mkdir test-repo && cd test-repo   # 新建文件夹
git init                          # 初始化，生成 .git/ 目录
```

要点：

- 仓库的全部数据（历史、分支、暂存区）都在 `.git/` 里，内部使用相对路径，
  因此**整个文件夹可以随意移动到任何路径**，不影响任何功能。
- 与远程的关联只是 `.git/config` 里的一条 URL，与本地路径无关。

## 2. 首次提交与提交信息规范

本仓库第一条提交：

```
d589e56 docs: 初始化仓库，添加 README 与 2026-08-30 商品库存记录
```

提交信息遵循 **Conventional Commits** 规范：

```
<类型>(<可选范围>): <简短描述>
例：docs(inventory): 库存表新增商品分类列
```

| 类型 | 用途 | 本仓库实例 |
| --- | --- | --- |
| `feat` | 新功能 / 新增内容 | `4a5f396` feat(snacks): 新增零食类库存记录 |
| `fix` | 修复错误 | `5558149` fix(inventory): 修正香蕉单价录入错误 |
| `docs` | 纯文档 | `791c207` docs(inventory): 苹果进货 30 箱 |
| `chore` | 杂项配置 | `5b0b8fd` chore: 扩充 .gitignore |

基本循环：

```bash
git status        # 查看工作区状态（红=未暂存，绿=已暂存）
git add <文件>    # 暂存改动
git commit -m ""  # 提交到当前分支
```

## 3. 关联远程仓库并推送

```bash
git remote add origin https://github.com/Liya2199/test-repo.git  # 关联（仅一次）
git remote -v                                                     # 验证 fetch/push 地址
git push -u origin master                                         # 首次推送
```

- `-u` 建立跟踪关系，之后本地与远程同分支名自动关联，直接 `git push` 即可。
- 推送成功后 `git status` 显示 `Your branch is up to date with 'origin/master'`。

## 4. 日常循环：修改 → diff → 提交

以本仓库 `791c207`（苹果进货）为例：

```bash
# 修改 inventory-2026-08-30.md：苹果 120 → 150，小计、合计同步更新
git diff                                   # 查看未暂存的改动
git add inventory-2026-08-30.md            # 暂存
git commit -m "docs(inventory): 苹果进货 30 箱（120 -> 150），更新合计"
git push
```

`git diff` 输出中 `-` 开头是旧行、`+` 开头是新行，精确到"哪一行改了什么"。

## 5. 撤销的四种姿势

按"错误在哪个阶段被发现"选择药方：

| 阶段 | 命令 | 效果 | 本仓库实例 |
| --- | --- | --- | --- |
| 还没提交 | `git restore <文件>` | 丢弃工作区改动 | 把芭蕾舞衣 300 手滑改回 30 |
| 已 commit 未 push | `git commit --amend` | 修改最后一次提交 | — |
| 已 commit 未 push | `git reset --soft HEAD~1` | 撤销提交、保留改动（`--hard` 连改动一起丢，慎用） | — |
| **已 push 到远程** | `git revert <提交>` | 生成反向提交抵消，**不改写历史** | `9ca0695` Revert `a125166` |

铁律：**已推送的提交永远用 revert，不要 reset**——别人可能已基于它工作，改写历史会让所有人的本地仓库陷入混乱。

## 6. 分支开发全流程

### 6.1 命名规范

| 前缀 | 用途 | 本仓库实例 |
| --- | --- | --- |
| `feature/<名称>` | 新功能 | `feature/snacks-category`、`feature/gift-box` |
| `fix/<名称>` | 修复 | `fix/banana-price`、`fix/milk-stock-count` |
| `docs/<名称>` | 纯文档 | `docs/readme-branches` |
| `hotfix/<名称>` | 线上紧急修复 | （未用到） |

### 6.2 标准循环

```bash
git switch master && git pull        # 起手式：确保基于最新主干
git switch -c feature/xxx            # 创建并切换到新分支
# ……开发、git add、git commit……
git switch master                    # 切回主干
git merge feature/xxx                # 合并（或 --no-ff，见下）
git branch -d feature/xxx            # 删除已合并分支（-d 只肯删已合并的）
git push                             # 推送主干
```

### 6.3 Fast-forward 与 --no-ff

```bash
git merge fix/cola-price-audit               # 快进：直线历史，无合并提交
git merge --no-ff feature/snacks-category    # 强制造合并提交，保留分支形状
```

本仓库历史中两种形状并存（见第 14 节图）：

- `a86532a`、`2bba157` 是 `--no-ff` 产生的菱形分叉；
- `5558149`（fix/banana-price）则是快进合并，一条直线，看不出曾有分支。

团队协作常**强制 --no-ff**，让每个功能在历史上留下清晰足迹。

### 6.4 远程分支的删除与影子清理

```bash
git branch -d <分支>              # 只删本地（-d 校验已合并；-D 强制，慎用）
git push origin --delete <分支>   # 删远程分支（可一次写多个分支名）
git fetch --prune                 # 清理本地缓存的远程分支"影子"(remotes/origin/*)
```

- `git branch -d` **只删本地**，远程分支必须用 `git push origin --delete` 单独删；
- `git branch -a` 里 `remotes/origin/` 开头的条目只是本地缓存的快照，不是远程本身；
- 若是**别人**在 GitHub 网页上删了远程分支，你本地的影子不会自动消失，用 `--prune` 与服务器对账；
- 实战：本仓库三条 WIP 分支收尾合并后，就是本地、远程一并删除的（见第 14 节）。

## 7. 远程分支与 Pull Request

**核心事实：`git push` 只上传数据，永远不会自动创建 PR。**
PR 是 GitHub 平台上的"合并申请单"，必须有人手动创建：

```
1. push 分支：  git push -u origin feature/xxx     ← 纯数据上传
2. 网页点 "Compare & pull request"（或 Pull requests → New）
   选 base: master ← compare: feature/xxx，确认 diff
3. Create pull request                              ← PR 此刻诞生
4. Reviewer 审查：评论 / Request changes / Approve
5. 有权限者点 Merge pull request                     ← 闭环
```

权限模型：

| 角色 | push 分支 | 创建 PR | merge 进 master |
| --- | --- | --- | --- |
| 所有者 | ✅ | ✅ | ✅ |
| 协作成员 | ✅ | ✅ | 由分支保护规则决定 |
| 陌生路人 | ❌ 须先 Fork | 只能发跨仓库 PR | ❌ |

正式团队会给 master 开 **Branch protection**：要求 N 个 Approve、CI 通过才能合并——
这就是"合并需要首肯"的制度化形式。个人仓库自己点合并即可。

PR 里写 `WIP:` 或在 GitHub 上标记 **Draft** 表示"未完成，先别合"。

## 8. 冲突的产生与解决

**触发条件**：两个分支改了同一个文件的同一处，git 无法裁决。
本仓库真实案例：Hermes 的促销分支（可乐 2.9）与 Hera 的审计分支（可乐 3.6）先后合并。

```bash
git merge feature/cola-promo    # 顺利
git merge fix/cola-price-audit  # CONFLICT (content): Merge conflict in ...
```

文件中会出现标记：

```
<<<<<<< HEAD               ← 当前分支（master）一侧
| 可乐 … 2.9 | 348.00 |
=======                    ← 分隔线
| 可乐 … 3.6 | 432.00 |
>>>>>>> fix/cola-price-audit   ← 正在合入的分支一侧
```

裁决三选一（真实案例采用了后两种）：

1. 取我方（HEAD 侧）
2. 取对方（合入侧）——最终可乐采用审计价 3.6、合计 2446.00
3. 两边融合——促销与审计两条备注都保留

```bash
git add <文件>            # 冲突场景下 add = "本文件已解决"
git commit --no-edit      # 接受默认合并信息，闭环
git merge --abort         # 任何时刻反悔：整个合并作废，回到合并前
```

**案例二（分支活得太久导致的冲突）**：Odysseus 基于旧版 master 完成了饮料调价分支
（可乐 3.5），而 master 随后被 Hera 的审计价 3.6 更新——收尾合并时，可乐行、
合计行、备注区三处同时冲突。裁决：可乐取审计价 3.6（分支上的 3.5 已过时），
矿泉水取分支上的 1.8（只有分支改了它，是有效新工作），合计融合为 2506.00，
促销/审计/调价三条备注全部保留。

教训：**分支活得越久，与主干的偏差越大，冲突越猛**——这又给"小步快合"添了一条理由。

防冲突四诀：任务分工不交叉；分支小步快合；勤 `git pull`；大改先沟通。

## 9. git stash：临时口袋

场景：改到一半要切去干别的，改动还没法提交。

```bash
git stash        # 半成品塞进口袋，工作区瞬间干净
git stash list   # 查看存货（stash@{0} 最新，像叠盘子）
git stash pop    # 取出最新的，口袋自动清空
git stash apply  # 取出但保留副本
git stash drop   # 扔掉某份存货
```

注意：stash 是**临时寄存**，长期工作请正常提交。

## 10. tag 与 Release

```bash
git tag -a v1.0.0 -m "v1.0.0 首个演示版本"   # 附注标签（推荐：含作者/日期/说明）
git tag -n                                    # 查看标签与说明
git push origin v1.0.0                        # ⚠️ 标签不会随 push 自动上传，必须显式推
```

- 分支是**会移动**的指针，tag 是**钉死**在某提交上的路牌；
  本仓库 `v1.0.0` 钉在 `d2f46b5`，`v1.1.0` 钉在 `6f2dfc4`，永不漂移。
- **Release** = tag + 发布说明 + 自动源码包，在 GitHub 网页 Releases → Draft a new release 创建；
  命令行方式：`gh release create v1.0.0 --notes "..."`（需先 `gh auth login`）。
- 版本命名遵循语义化：修 bug 升 `v1.0.1`，加功能升 `v1.1.0`。

## 11. .gitignore 详解

```gitignore
*.log                    # 通配符：所有 .log
node_modules/            # 结尾 / = 整个目录
.env                     # 精确文件名（密钥绝不入库！）
!important.log           # ! 否定：从忽略名单里捞回例外
```

要点：

- `git status` 默认看不到被忽略文件，加 `--ignored` 参数可让它们以 `!!` 现形；
- **.gitignore 只对未跟踪文件生效**，已跟踪文件要先 `git rm --cached <文件>` 才能移出。

## 12. 多身份提交（模拟多人协作）

```bash
git -c user.name="Odysseus" -c user.email="odysseus@ithaca.gr" \
    commit -m "feat(snacks): 矿泉水调价 1.5 -> 1.8，重算合计"
```

- `-c` 只对这一条命令生效，可临时假扮另一身份；
- **commit 的作者**是提交时写死的元数据；**push 的凭据**才是 GitHub 账号——两者独立；
- 真实协作中每人用自己的电脑与账号，作者自然正确；
- 邮箱若未绑定 GitHub 账号，网页上只显示名字不显示头像。

## 13. 常用命令速查表

```bash
# 状态与历史
git status / git status --short      # 工作区状态
git log --oneline --graph --all      # 全分支图形化历史
git diff                             # 未暂存的改动
git diff --staged                    # 已暂存的改动
git ls-remote --heads origin         # 问服务器要分支列表

# 基础循环
git add <文件> / git commit -m ""    # 暂存 + 提交
git push / git pull                  # 推 / 拉

# 分支
git switch -c <新分支>               # 建分支并切换
git merge [--no-ff] <分支>           # 合并
git branch -d <分支>                 # 删已合并分支（仅本地）
git push origin --delete <分支>      # 删远程分支

# 撤销
git restore <文件>                   # 丢弃工作区改动
git commit --amend                   # 补改最后一次提交（未 push）
git revert <提交>                    # 反向提交抵消（已 push 安全撤销）

# 临时与标记
git stash / git stash pop            # 口袋收 / 取
git tag -a v1.0.0 -m ""              # 打附注标签
git push origin v1.0.0               # 推送标签
```

## 14. 本仓库真实提交历史（案例）

```
6088da7 (HEAD -> master) Merge branch 'feature/beverages-price-update'  ← 冲突合并（案例二）
|\
| * b995473 feat(snacks): 矿泉水调价 1.5 -> 1.8             ← Odysseus 收尾
| * 2d24040 feat(snacks): 可乐调价 3.0 -> 3.5（WIP 开工）
* |   552c076 Merge branch 'feature/gift-box'                ← 干净合并
|\ \
| * | 2f29115 docs(giftbox): 完成对账，勾选全部待办          ← Heracles 收尾
| * | 49b6f74 feat(giftbox): 录入礼盒数据                    ← Athena
| * | 057306b feat(giftbox): 礼盒骨架（WIP 开工）
| |/
* |   d2dc83d Merge branch 'fix/milk-stock-count'            ← 干净合并
|\ \
| * | 14ce7ff fix(inventory): 牛奶盘点重算合计               ← Achilles 收尾
| * | 090ddcd fix(inventory): 牛奶盘点 45 -> 48（WIP 开工）
| |/
* | 2c984e1 docs(tutorial): 新增 Git 全流程实战教程          ← 本教程首次入库
* |   2bba157 Merge branch 'fix/cola-price-audit'            ← 冲突合并（案例一）
|\ \
| * | b86db95 fix(snacks): 审计更正可乐单价 3.0 -> 3.6       ← Hera
* | | 422c7d4 feat(snacks): 可乐促销价 3.0 -> 2.9            ← Hermes
|/ /
864d44d Merge branch 'docs/readme-branches'                  ← --no-ff 合并
|\
| * 9dc6d3a docs(readme): 补充分支策略说明
|/
5558149 fix(inventory): 修正香蕉单价录入错误 3.2 -> 2.8      ← 快进合并（直线）
a86532a Merge branch 'feature/snacks-category'               ← --no-ff 合并
|\
| * 4a5f396 feat(snacks): 新增零食类库存记录
|/
6f2dfc4 (tag: v1.1.0) docs(clothing): 纱裙调价、发髻网套售出
7ca3628 docs(inventory): 鸡蛋售出 50、面包调价
d2f46b5 (tag: v1.0.0) docs(clothing): 练功连体服进货 15 件
5b0b8fd chore: 扩充 .gitignore 忽略规则
9ca0695 Revert "docs(clothing): 更新衣物库存合计金额"        ← revert 撤销示例
a125166 docs(clothing): 更新衣物库存合计金额                 ← 被撤销的错误提交
034ec78 docs(clothing): 新增衣物商品库存记录
745317c docs(inventory): 库存表应用牛奶调价
7437be0 Merge pull request #1                                ← GitHub PR 合并
|\
| * 6c6be23 feat(inventory): 库存表新增商品分类列
|/
37e1585 docs(inventory): 牛奶调价 12.8 -> 14.5，记录人 Odysseus
791c207 docs(inventory): 苹果进货 30 箱
d589e56 docs: 初始化仓库，添加 README 与 2026-08-30 商品库存记录   ← 起点
```

曾经的"三条未合并 WIP 分支"已全部收尾合并（多身份接力：Liya2199 开工 →
Odysseus / Achilles / Athena / Heracles 分别收尾），随后本地与远程分支一并删除，
仓库回归单主干——这就是分支"用完即弃"的完整生命周期。

---

> 学完本教程，建议对照 `git log` 把每条提交的来龙去脉走一遍——
> 版本管理的功夫不在背命令，而在理解"每一步之后仓库处于什么状态"。
