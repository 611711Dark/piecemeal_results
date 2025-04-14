import plotly.graph_objects as go

# 定义节点（优化标签显示）
nodes = dict(
    label=[
        "环境交互<br>收集(sₜ, aₜ, rₜ, sₜ₊₁)",
        "轨迹缓存<br>存储{(sₜ, aₜ, rₜ)}",
        "Critic网络<br>V_ϕ(sₜ)",
        "时序差分<br>δₜ = rₜ + γV(sₜ₊₁) - V(sₜ)",
        "GAE计算<br>Aₜ = Σ(γλ)ᵏ δₜ₊ₖ",
        "旧策略网络<br>π_old(aₜ|sₜ)",
        "新策略网络<br>π_θ(aₜ|sₜ)",
        "概率比<br>rₜ(θ) = π_θ/π_old",
        "Clipped损失<br>min(rA, clip(r,1±ε)A)",
        "价值损失<br>(V_ϕ - Rₜ)²",
        "折扣回报<br>Rₜ = Σγᵏrₜ₊ₖ",
        "总损失<br>L = Lᴬ - c₁Lⱽ + c₂H",
        "参数更新<br>θ ← θ - α∇L",
        "旧策略同步<br>θ_old ← θ"
    ],
    color=[
        "rgba(249, 231, 159, 0.8)",  # 环境交互
        "rgba(213, 219, 219, 0.8)",  # 轨迹缓存
        "rgba(174, 214, 241, 0.8)",  # Critic网络
        "rgba(133, 193, 233, 0.8)",  # 时序差分
        "rgba(52, 152, 219, 0.8)",   # GAE计算
        "rgba(250, 219, 216, 0.8)",  # 旧策略
        "rgba(245, 183, 177, 0.8)",  # 新策略
        "rgba(231, 76, 60, 0.8)",    # 概率比
        "rgba(31, 97, 141, 0.8)",    # Clipped损失
        "rgba(171, 235, 198, 0.8)",  # 价值损失
        "rgba(200, 200, 200, 0.8)",  # 折扣回报（新增节点）
        "rgba(46, 204, 113, 0.8)",   # 总损失
        "rgba(125, 60, 152, 0.8)",   # 参数更新
        "rgba(255, 165, 0, 0.8)"     # 旧策略同步（新增节点）
    ]
)

# 定义连接关系（修正后的逻辑流）
links = dict(
    # source和target索引对应节点列表顺序
    source = [0, 1, 1, 1, 2, 3, 4, 1, 5, 6, 7, 4, 7, 8, 10, 9, 11, 12],
    target = [1, 2, 5,10, 3, 4, 8, 9, 7, 7, 8, 7,11,11, 9,11,12,13],
    value = [3, 2, 2,2, 2, 2, 3, 2, 2, 2, 3, 2,3,3, 2,2,3,3]
)

fig = go.Figure(go.Sankey(
    node = dict(
        label = nodes["label"],
        color = nodes["color"],
        hovertemplate = "%{label}<extra></extra>",  # 简化悬停信息
        pad = 130  # 增加节点间距
    ),
    link = dict(
        source = links["source"],
        target = links["target"],
        value = links["value"],
        color = "rgba(100, 100, 100, 0.3)"  # 连接线颜色
    ),
    arrangement = "snap",
    textfont = dict(size=12)
))

# 添加动态注释（优化核心公式显示）
fig.add_annotation(
    x=0.6, y=0.98,
    text="<b>PPO核心逻辑：</b><br>"
         "1. 策略更新限制：│π_θ/π_old -1│≤ε<br>"
         "2. 优势估计：Aₜ = 𝔼[δₜ + γλδₜ₊₁ + (γλ)²δₜ₊₂ +...]<br>"
         "3. 双重优化：策略θ + 价值函数ϕ",
    showarrow=False,
    bordercolor="#2ECC71",
    borderwidth=2,
    bgcolor="white",
    font=dict(size=12, color="#2C3E50"),
    xanchor="right",
    yanchor="top"
)

fig.update_layout(
    title="<b>PPO算法完整计算流</b>",
    title_x=0.5,
    height=500,
    width=1200,
    margin=dict(l=30, r=30, b=50, t=50),
    hovermode='closest',
    plot_bgcolor='white'
)

fig.show()
