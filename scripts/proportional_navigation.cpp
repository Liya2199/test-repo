// 比例导引（Proportional Navigation）二维演示
// 与战雷档案的"导弹视角"趣味联动；恒速导弹拦截匀速直线逃跑的目标。
//
// 编译（可选）：g++ -std=c++17 -O2 proportional_navigation.cpp -o pn && ./pn
//
// 核心思想：视线转率（LOS rate）越大，就施加越大的横向过载把它压回零——
// 视线不再旋转，就是纯追踪碰撞 course。导航比 N 经典取值 3~5。

#include <cmath>
#include <cstdio>

struct Vec2 {
    double x, y;
    Vec2  operator+(Vec2 o) const { return {x + o.x, y + o.y}; }
    Vec2  operator-(Vec2 o) const { return {x - o.x, y - o.y}; }
    Vec2  operator*(double s) const { return {x * s, y * s}; }
    double norm() const { return std::hypot(x, y); }
    Vec2  normalized() const { return *this * (1.0 / norm()); }
};

static double cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }

int main() {
    Vec2 missile{0.0, 0.0};                        // 导弹初始位置（米）
    Vec2 target{800.0, 600.0};                     // 目标初始位置（米）
    Vec2 missileVel{120.0, 40.0};                  // 导弹速度（米/秒）
    const Vec2 targetVel{-30.0, 5.0};              // 目标匀速直线逃跑
    const double speed = missileVel.norm();        // 导弹保持恒速，只改方向
    const double N = 4.0;                          // 导航比（Proportional constant）
    const double dt = 0.01;                        // 积分步长（秒）

    for (int step = 0; step < 100000; ++step) {
        Vec2 rel = target - missile;
        if (rel.norm() < 5.0) {                    // 命中判定：近距 5 米
            std::printf("命中！飞行 %.2f 秒，剩余偏差 %.2f 米\n",
                        step * dt, rel.norm());
            return 0;
        }

        Vec2 los        = rel.normalized();                            // 视线单位向量
        double losRate  = cross(rel, targetVel - missileVel)           // 视线转率
                          / (rel.norm() * rel.norm());                 // dλ/dt = (R × V) / |R|²
        Vec2 accel      = los * (N * losRate * speed);                 // PN 横向制导指令

        missileVel = (missileVel + accel * dt).normalized() * speed;   // 恒速：只转向不加速
        missile    = missile + missileVel * dt;
        target     = target + targetVel * dt;
    }
    std::printf("未能在限定步数内命中。\n");
    return 1;
}
