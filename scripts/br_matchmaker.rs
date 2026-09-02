//! 战雷分房模拟器（WIP）：把排队的玩家按 BR 贪心均衡分进两队。
//! 与战雷档案的"分房视角"趣味联动。
//! 可选运行：cargo run；不跑也很好读。

/// 一名排队玩家的载具与分房权重
struct Player {
    name: &'static str,
    vehicle: &'static str,
    br: f64,
}

fn main() {
    let queue = [
        Player { name: "Icarus",   vehicle: "F-16A ADF",    br: 13.3 },
        Player { name: "Odysseus", vehicle: "Su-27",        br: 13.7 },
        Player { name: "Athena",   vehicle: "歼-10A",       br: 14.0 },
        Player { name: "Heracles", vehicle: "台风 EF-2000", br: 14.7 },
    ];

    // 贪心策略：BR 高的先选队，轮流塞进当前总队值较小的那一队。
    let mut sorted: Vec<&Player> = queue.iter().collect();
    sorted.sort_by(|a, b| b.br.partial_cmp(&a.br).unwrap());

    let (mut team_a, mut team_b): (Vec<&Player>, Vec<&Player>) = (Vec::new(), Vec::new());
    for p in sorted {
        let score = |team: &[&Player]| team.iter().map(|p| p.br).sum::<f64>();
        if score(&team_a) <= score(&team_b) {
            team_a.push(p);
        } else {
            team_b.push(p);
        }
    }

    let fmt = |team: &[&Player]| {
        team.iter()
            .map(|p| p.vehicle)
            .collect::<Vec<_>>()
            .join(" / ")
    };
    let diff = (score_fn(&team_a) - score_fn(&team_b)).abs();

    println!("A 队：{}", fmt(&team_a));
    println!("B 队：{}", fmt(&team_b));
    println!("两队 BR 差：{:.1}", diff);
}

/// 独立的求和函数，闭包借用起来更清爽
fn score_fn(team: &[&Player]) -> f64 {
    team.iter().map(|p| p.br).sum()
}
