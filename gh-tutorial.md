# GitHub CLI (gh) 实战教程

> 由 Odysseus 整理，基于本仓库（Liya2199/test-repo）的真实输出编写。
> 前置条件：已安装 gh 并完成 `gh auth login` 授权。
> 记录日期：2026-09-01

## 目录

1. [gh 是什么](#1-gh-是什么)
2. [认证管理](#2-认证管理)
3. [仓库操作](#3-仓库操作)
4. [Pull Request 操作](#4-pull-request-操作)
5. [Release 操作](#5-release-操作)
6. [Issue 操作](#6-issue-操作)
7. [gh api：万能后门](#7-gh-api万能后门)
8. [gh + git 组合工作流](#8-gh--git-组合工作流)
9. [速查表](#9-速查表)

---

## 1. gh 是什么

`gh` 是 GitHub 官方命令行工具。**git 管版本，gh 管 GitHub 平台**：
git 只负责 push/pull 数据，而 PR、Issue、Release、Actions 这些"平台功能"
过去必须开网页操作，现在用 gh 在终端里一条命令搞定。

## 2. 认证管理

```bash
gh auth login     # 交互式登录（选 GitHub.com → HTTPS → 浏览器授权）
gh auth status    # 查看登录状态
```

本仓库的真实输出：

```
github.com
  ✓ Logged in to github.com account Liya2199 (keyring)
  - Active account: true
  - Git operations protocol: https
  - Token: gho_************************************
  - Token scopes: 'gist', 'read:org', 'repo', 'workflow'
```

要点：

- token 存在系统钥匙串（keyring）里，不会明文落盘；
- scopes 是授权范围：`repo`（读写仓库）、`workflow`（改 Actions）、`gist` 等；
- 支持多账号：`gh auth switch -u <用户名>` 切换。

## 3. 仓库操作

```bash
gh repo view                    # 终端里看当前仓库 README
gh repo view --web              # 直接打开浏览器（= gh browse）
gh repo view --json name,owner,defaultBranchRef \
  --jq "{name: .name, owner: .owner.login, default: .defaultBranchRef.name}"
# 本仓库真实输出：{"default":"master","name":"test-repo","owner":"Liya2199"}

gh repo fork                    # fork 别人的仓库（开源贡献第一步）
gh repo create <名字> --source=. --push   # 把当前本地仓库一键建到 GitHub 并推送
gh repo clone <owner>/<repo>    # 克隆（等价 git clone，但自动配置 upstream）
```

`--jq` 参数用 jq 语法过滤 JSON，只提取关心的字段。

## 4. Pull Request 操作

上一节教程说过：PR 要去网页点按钮创建。有了 gh，全程留在终端：

```bash
gh pr create --base master --head feature/xxx \
  --title "feat: xxx" --body "做了什么、为什么"

gh pr list          # 列出所有 PR（本仓库当前输出为空——还没有发过 PR）
gh pr view 1        # 看 1 号 PR 详情
gh pr checkout 1    # 把别人的 PR 拉到本地审查（超实用！）
gh pr checks        # 看 CI 检查状态
gh pr review 1 --approve          # 批准
gh pr review 1 --request-changes  # 要求修改
gh pr merge 1 --merge             # 合并（或 --squash / --rebase）
gh pr merge 1 --squash --delete-branch   # 压扁合并并顺手删分支
gh pr close 1
```

注意：`gh pr merge` 一样受分支保护规则约束，该有的 Approve 一个都不能少。

## 5. Release 操作

本仓库当前的真实 Release 列表：

```
v1.2.0	Latest	v1.2.0	2026-09-01T12:32:18Z
v1.1.0		v1.1.0	2026-09-01T06:25:27Z
```

(v1.0.0 只有 tag，还没发过 Release——正好留给你练手：`gh release create v1.0.0 ...`)

```bash
gh release list                              # 列出 Release
gh release view v1.2.0                       # 查看某个 Release
gh release create v1.3.0 --title "v1.3.0" \
  --notes "- 更新内容1
- 更新内容2"                                # 基于已存在的 tag 创建 Release
gh release create v1.4.0 --generate-notes    # 自动根据提交记录生成发布说明
gh release create v1.5.0 dist.zip            # 顺带附上构建产物附件
gh release download v1.2.0                   # 下载某个 Release 的附件/源码包
gh release delete v1.0.0 --yes               # 删除 Release（不影响 tag 本身）
```

`--generate-notes` 会自动汇总"新提交 + 贡献者 + 完整变更链接"，懒人福音。

## 6. Issue 操作

```bash
gh issue create --title "牛奶价格显示错误" --body "复现步骤：……"
gh issue list                    # 列出 Issue
gh issue view 1                  # 看详情与评论
gh issue close 1                 # 关闭
gh issue list --state all        # 含已关闭的
```

团队惯例：分支名 / PR 里写 `#1` 这样的 Issue 编号，合并时 GitHub 自动关联；
提交信息写 `fix: xxx (fixes #1)`，PR 合入时自动关闭对应 Issue。

## 7. gh api：万能后门

GitHub 网页和 gh 子命令没有覆盖的功能，都能用原始 API 敲开：

```bash
gh api repos/Liya2199/test-repo/tags              # 列出所有 tag
gh api user --jq .login                            # 我是谁
gh api repos/Liya2199/test-repo/commits --jq '.[0].commit.message'
```

返回纯 JSON，配 `--jq` 随意裁剪，可无缝接进脚本。

## 8. gh + git 组合工作流

以本仓库真实流程为例——"给未合并的 WIP 分支发 PR 并合并"：

```bash
git push -u origin feature/gift-box              # git：上传分支
gh pr create --base master --head feature/gift-box \
  --title "feat(giftbox): 礼盒库存档案" --body "数据已录入完毕"   # gh：建 PR
gh pr merge --squash --delete-branch             # gh：压扁合并 + 双端删分支
```

`gh pr merge --delete-branch` 会把**本地和远程的分支一起删掉**并切回主干，
等于替你完成了 git 侧的 `branch -d` + `push origin --delete` 两步。

## 9. 速查表

```bash
# 认证
gh auth login / gh auth status / gh auth switch -u <用户名>

# 仓库
gh repo view [--web] / gh repo fork / gh repo create --source=. --push

# PR
gh pr create --base <目标> --head <分支> --title "" --body ""
gh pr list / view / checkout / checks
gh pr review --approve / --request-changes
gh pr merge [--merge|--squash|--rebase] [--delete-branch]

# Release
gh release list / view <tag> / create <tag> --notes "" / --generate-notes
gh release download <tag> / delete <tag> --yes

# Issue
gh issue create / list / view / close

# 万能
gh api <端点> --jq <过滤表达式>
gh browse        # 当前仓库/文件/Issue 直接在浏览器打开
```

---

> 一句话记住分工：**git 管提交，gh 管平台**。
> 能 push 只说明数据上去了；PR、Release、Issue 这些"手续"，现在也都能在终端办了。
