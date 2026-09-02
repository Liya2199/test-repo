// Unity C#：极简战机机体组件（WIP 骨架）
// 与战雷档案的"战机视角"趣味联动。
// 使用：挂到带 Rigidbody 的飞机预制体上，在 Input 设置里
//      建 Throttle / Roll / Pitch 三个轴即可试飞。

using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class Airframe : MonoBehaviour
{
    [Header("推力与操控")]
    [SerializeField] private float maxThrust   = 40f;   // 最大推力（"玩家味单位"）
    [SerializeField] private float rollTorque  = 800f;  // 滚转力矩
    [SerializeField] private float pitchTorque = 600f;  // 俯仰力矩

    [Header("生存性")]
    [SerializeField] private float maxHp = 100f;
    private float hp;

    private Rigidbody rb;

    private void Awake()
    {
        rb = GetComponent<Rigidbody>();
        hp = maxHp;
    }

    private void FixedUpdate()
    {
        float throttle = Input.GetAxis("Throttle");    // W/S
        float roll     = Input.GetAxis("Roll");        // A/D
        float pitch    = Input.GetAxis("Pitch");       // Up/Down

        rb.AddRelativeForce(Vector3.forward * (throttle * maxThrust));
        rb.AddRelativeTorque(Vector3.forward * (-roll * rollTorque));   // 左压杆左滚
        rb.AddRelativeTorque(Vector3.right   * (pitch * pitchTorque));
    }

    /// <summary>受到伤害；返回是否坠毁，供炮弹回调决定要不要播爆炸特效。</summary>
    public bool ApplyDamage(float amount)
    {
        hp = Mathf.Max(0f, hp - amount);
        return hp <= 0f;
    }
}
