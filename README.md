# test-repo — Git 工作流演示项目

这是一个用于演示 Git 开发流程的纯 Markdown 演示项目（无实际业务用途）。

## 项目内容

| 文件 | 说明 |
| --- | --- |
| `inventory-2026-08-30.md` | 商品库存记录：商品、数量、单价、小计，标注了记录日期 |
| `clothing-inventory-2026-08-30.md` | 衣物类库存记录：芭蕾舞衣、纱裙、软底芭蕾鞋等 |
| `snacks-inventory-2026-09-01.md` | 零食饮料类库存记录：薯片、可乐、巧克力等 |
| `.gitignore` | 忽略日志、临时文件等不需要纳入版本控制的内容 |
| `git-tutorial.md` | **Git 全流程实战教程**：init → 提交规范 → 分支 → PR → 冲突 → 撤销 → stash → tag |

## 演示流程

1. `git status` — 查看工作区状态
2. `git add <文件>` — 暂存改动
3. `git commit` — 提交（提交信息遵循 Conventional Commits 规范）
4. 修改库存记录（如进货、调价）→ 重复 1–3，用 `git diff` / `git log` 回溯变化

## 提交信息规范（Conventional Commits）

```
<类型>(<可选范围>): <简短描述>

例：docs(inventory): 更新 2026-08-30 库存记录，苹果进货 30 箱
```

常用类型：`feat` 新功能 / `fix` 修复 / `docs` 文档 / `chore` 杂项

## 分支策略

| 分支 | 用途 | 生命周期 |
| --- | --- | --- |
| `master` | 主干，保持随时可发布 | 永久 |
| `feature/<名称>` | 新功能开发 | 合并回 master 后删除 |
| `fix/<名称>` | 缺陷修复 / 数据修正 | 合并回 master 后删除 |
| `docs/<名称>` | 纯文档改动 | 合并回 master 后删除 |
