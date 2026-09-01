# test-repo — Git 工作流演示项目

这是一个用于演示 Git 开发流程的纯 Markdown 演示项目（无实际业务用途）。

## 项目内容

| 文件 | 说明 |
| --- | --- |
| `inventory-2026-08-30.md` | 商品库存记录：商品、数量、单价、小计，标注了记录日期 |
| `clothing-inventory-2026-08-30.md` | 衣物类库存记录：芭蕾舞衣、纱裙、软底芭蕾鞋等 |
| `.gitignore` | 忽略日志、临时文件等不需要纳入版本控制的内容 |

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
