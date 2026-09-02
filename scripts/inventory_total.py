"""校验食品库存表：从 markdown 表格解析并重算小计与合计。

纯标准库实现；直接读取仓库根目录的 inventory-2026-08-30.md，
是本目录里唯一与仓库文件实时联动的脚本。
"""
from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TABLE = ROOT / "inventory-2026-08-30.md"

# 匹配形如 "| 苹果 | 水果 | 150 | 5.5 | 825.00 |" 的数据行；
# 行首第二个单元格用 [^|*] 排除掉 **合计** 行。
ROW = re.compile(
    r"^\|\s*([^|*]+?)\s*\|\s*[^|]+\|\s*(\d+)\s*\|\s*([\d.]+)\s*\|\s*([\d.]+)\s*\|"
)


def parse_rows(text: str):
    """从 markdown 文本中提取 (商品, 数量, 单价, 表内小计) 四元组。"""
    for line in text.splitlines():
        if m := ROW.match(line):
            name, qty, price, subtotal = m.groups()
            yield name, int(qty), float(price), float(subtotal)


def main() -> None:
    rows = list(parse_rows(TABLE.read_text(encoding="utf-8")))
    if not rows:
        raise SystemExit(f"未在 {TABLE.name} 中解析到数据行，请检查表格格式")

    print(f"{'商品':<6}{'数量':>5}{'单价':>7}{'表内小计':>9}{'重算':>9}  核对")
    all_ok = True
    for name, qty, price, subtotal in rows:
        recalculated = qty * price
        ok = abs(recalculated - subtotal) < 1e-6
        all_ok &= ok
        print(f"{name:<6}{qty:>5}{price:>7.2f}{subtotal:>9.2f}{recalculated:>9.2f}  {'✓' if ok else '✗'}")

    grand = sum(qty * price for _, qty, price, _ in rows)
    print(f"\n共 {len(rows)} 种商品，重算合计 {grand:.2f} 元 —— 账实{'相符 ✓' if all_ok else '不符 ✗'}")


if __name__ == "__main__":
    main()
