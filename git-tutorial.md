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
15. [常见误解澄清（FAQ）](#15-常见误解澄清faq)

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
- 版本命名遵循语义化：修 bug 升 `v1.0.1`，加功能升 `v1.1.0`。

### 发布 Release 的两条路径（gh 与 git 的联动点）

**Release 必须锚定 tag**——tag 是路牌，Release 是路牌旁的公告栏，先立牌、再贴公告。

**路径 A（推荐：git 管打标签，gh 管发公告）**

```bash
git tag -a v1.3.0 -m "v1.3.0 说明文字"   # ① git：本地打附注标签
git push origin v1.3.0                    # ② 标签推上远程（⚠️ 不会随 git push 自动上传）
gh release create v1.3.0 --notes "..."    # ③ gh：往已存在的 tag 上发 Release（需 gh auth login）
```

第 ③ 步不想用命令行，就去网页 Releases → Draft a new release → 选择已推上去的 tag。

**路径 B（gh 一条龙：tag 不存在时它替你建）**

```bash
gh release create v1.3.0 --target master --generate-notes
# 本地/远程都没有 v1.3.0 时，gh 通过 API 在远程自动创建 tag，指向 --target 分支
# ⚠️ 陷阱：这样建的 tag 只存在于远程，本地毫无感知，记得补一刀：
git fetch --tags                          # 把远程新建的 tag 同步回本地
```

> 两条路径的更多细节（附件、--generate-notes、删除 Release 等）见 **gh-tutorial.md 第 5 节**。

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
git fetch origin                     # 只下载远程更新，不合并、不动工作区
git log HEAD..origin/master --oneline  # 比对：远程有而本地没有的提交
git push / git pull                  # 推 / 拉（pull = fetch + merge）

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
6088da7 Merge branch 'feature/beverages-price-update'  ← 冲突合并（案例二）
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

注：上图截取至 `6088da7` 前后，此后又叠加了 gh-tutorial 入库与多次教程迭代提交，最新历史以 `git log` 为准。

## 15. 常见误解澄清（FAQ）

**Q1：分支"旧了"就一定会冲突吗？**

不会。冲突的判定只看**同一文件的同一处区域是否被双方都改过**，与分支新旧无关。
实战工具箱：

```bash
git merge-base master <分支>                  # 找两分支的共同祖先
git merge-tree --write-tree master <分支>     # 合并预演：exit 0 干净 / exit 1 有冲突，不改动任何文件
```

本仓库实例：`feature/war-thunder-jets` 只新增了战雷档案文件，master 侧同期只改了
`gh-tutorial.md`——文件集合零交集，无论隔多久合并都干净。

**Q2：分支落后主干很多，该怎么办？**

给分支"保鲜"：站在分支上把主干反向合并进来（back-merge）。

```bash
git switch feature/xxx
git merge master        # 把主干最新成果同步进分支
```

分支拖得越久偏差越大：要么将来冲突更猛，要么合并时"惊喜"越多。勤保鲜，或小步快合。

**Q3：删了分支，提交会丢吗？**

不会。**分支只是一个指向提交的指针**（所以建删都是瞬间完成）。只要提交仍被某条
分支、tag 或远程引用可达，删指针不丢数据；只有彻底无人引用的提交才最终被回收。
这也是 `git branch -d` 只肯删"已合并分支"的原因——它防止的正是指针删了、提交悬空。

**Q4：`git branch -a` 里的 origin/master 就是远程分支吗？**

准确说是**远程分支的本地影子**（remote-tracking ref）：上次与服务器同步时的快照。
它不会自动更新——别人在网页上删了远程分支，你本地的影子还留着，用
`git fetch --prune` 对账清理（见 6.4）。

**Q5：`git pull` 随手用安全吗？**

`git pull` = `git fetch` + `git merge`，直接并进当前分支。单人开发无所谓；
在多人协作的分支上，更稳妥的习惯是先 `git fetch`，看一眼
`git log HEAD..origin/master` 确认对方的改动，再手动 merge——
把"自动合并"升级为"知情合并"。

**Q6：`git push` 被拒（fetch first）怎么办？——真实事故完整还原**

场景：本地刚提交教程更新，push 时被拒。原因：另一个"自己"正在网页上合并
PR #5，远程 master 已经前进——**两个交汇点同时推进，git 拒绝悄悄覆盖**。

```bash
git push
```
```
 ! [rejected]        master -> master (fetch first)
error: failed to push some refs to 'https://github.com/Liya2199/test-repo.git'
hint: Updates were rejected because the remote contains work that you do not
hint: have locally. ...
```

被拒是 git 在保护你，**正确反应只有一条路：fetch → 查看 → 合并 → 再推**，
绝不 `git push --force`（那会用本地历史覆盖远程，把别人——这里是你自己——
在网页上合并的 PR 直接从历史里抹掉）。

第 1 步：`git fetch origin` —— 只下载、不合并

```bash
git fetch origin
```
```
From https://github.com/Liya2199/test-repo
   f544007..86286a2  master     -> origin/master
```

fetch 把远程新提交下载到本地，更新影子引用 origin/master，
但**完全不碰你的工作区和本地分支**——这是它与 pull 的本质区别。

第 2 步：双向 diff —— 搞清两边各有什么

```bash
git log HEAD..origin/master --oneline          # 远程有、本地没有
```
```
86286a2 Merge pull request #5 from Liya2199/feature/war-thunder-jets
f615a21 Merge branch 'master' into feature/war-thunder-jets
b6c04b2 feat(warthunder): 新增现代喷气战斗机档案（WIP）
```
```bash
git log origin/master..HEAD --oneline          # 本地有、远程没有
```
```
2016b19 docs(tutorial): 新增第 15 节常见误解澄清（FAQ）
```

`A..B` 读作"在 B 里、但不在 A 里"。两条一对比：双方各自前进、互不包含
——标准的多方并行局面，需要一次合并来汇合。

第 3 步：`git pull --no-edit` —— 知情合并

```bash
git pull --no-edit
```
```
Merge made by the 'ort' strategy.
 war-thunder-modern-jets.md | 36 ++++++++++++++++++++++++++++++++++++
```

远程动的是战雷新文件，本地动的是 git-tutorial.md，零交集，干净合并。

第 4 步：重新 `git push` —— 一路绿灯

```bash
git push
```
```
To https://github.com/Liya2199/test-repo.git
   86286a2..69ec9ea  master -> master
```

---

> 学完本教程，建议对照 `git log` 把每条提交的来龙去脉走一遍——
> 版本管理的功夫不在背命令，而在理解"每一步之后仓库处于什么状态"。
