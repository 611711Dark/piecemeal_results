import numpy as np
import random
import os
import pickle
from collections import defaultdict, deque
import datetime
from typing import List, Dict, Tuple, Deque
import jieba  # 导入中文分词库

# 初始化jieba分词器
jieba.initialize()

class PureSelfOrganizingBrain:
    def __init__(self, model_path="model.pkl"):
        # 基础神经参数
        self.global_time = 0
        self.time_step = 1.0  # 毫秒
        self.model_path = model_path

        # 神经元系统
        self.neuron_id_counter = 0
        self.neurons: Dict[int, dict] = {}
        self.neuron_activity: Dict[int, Deque[int]] = defaultdict(lambda: deque(maxlen=100))

        # 连接系统
        self.synapses: Dict[int, Dict[int, float]] = defaultdict(dict)
        self.synapse_history: Dict[Tuple[int, int], Deque[int]] = defaultdict(lambda: deque(maxlen=100))

        # 学习参数
        self.stdp_params = {"a_plus": 0.15, "a_minus": 0.12, "tau_plus": 20.0, "tau_minus": 20.0}
        self.plasticity_params = {"growth_threshold": 0.25, "prune_threshold": 0.04}
        self.neuromodulators = {"dopamine": 0.0, "acetylcholine": 0.5}

        # 语言系统（词语级）
        self.word_neurons: Dict[str, int] = {}  # 所有词语到神经元的映射
        self.neuron_words: Dict[int, str] = {}  # 神经元到词语的映射
        self.context: Deque[int] = deque()  # 无长度限制的上下文
        self.conversation_history: List[Tuple[str, str, datetime.datetime]] = []
        self.word_sources: Dict[str, str] = {}  # 标记每个词语的来源（user或ai）
        self.neuron_sources: Dict[int, str] = {}  # 神经元ID到来源的映射

        # 响应生成状态
        self.response_pathway = []

        # 特殊控制神经元
        self.EOS_USER = '\x01'  # 用户输入结束标识符 (ASCII SOH)
        self.EOS_AI = '\x02'  # AI输出结束标识符 (ASCII STX)
        self.eos_user_neuron = None
        self.eos_ai_neuron = None

        # 初始化
        self._initialize_core_network()
        self._initialize_special_neurons()
        self.load_model()

    def _initialize_core_network(self):
        """创建基础网络结构"""
        # 创建语言处理核心区域
        for _ in range(100):
            self._create_neuron(neuron_type="core", source="system")

        # 创建输入/输出接口神经元
        self.input_interface = self._create_neuron(neuron_type="interface", source="system")
        self.output_interface = self._create_neuron(neuron_type="interface", source="system")

    def _initialize_special_neurons(self):
        """创建特殊控制神经元"""
        # 创建用户输入结束神经元
        self.eos_user_neuron = self._create_neuron(neuron_type="special", source="system")
        self.word_neurons[self.EOS_USER] = self.eos_user_neuron
        self.neuron_words[self.eos_user_neuron] = self.EOS_USER
        self.word_sources[self.EOS_USER] = "system"
        self.neuron_sources[self.eos_user_neuron] = "system"

        # 创建AI输出结束神经元
        self.eos_ai_neuron = self._create_neuron(neuron_type="special", source="system")
        self.word_neurons[self.EOS_AI] = self.eos_ai_neuron
        self.neuron_words[self.eos_ai_neuron] = self.EOS_AI
        self.word_sources[self.EOS_AI] = "system"
        self.neuron_sources[self.eos_ai_neuron] = "system"

        # 加强结束神经元的连接
        self._create_synapse(self.input_interface, self.eos_user_neuron, initial_weight=0.8)
        self._create_synapse(self.output_interface, self.eos_ai_neuron, initial_weight=0.8)

    def _create_neuron(self, neuron_type="standard", initial_potential=0.0, source: str = "unknown") -> int:
        """创建一个新神经元"""
        neuron_id = self.neuron_id_counter
        self.neuron_id_counter += 1

        self.neurons[neuron_id] = {
            "membrane_potential": initial_potential,
            "threshold": 1.0,
            "tau": 20.0,
            "reset_potential": 0.0,
            "refractory_period": 5,
            "refractory_counter": 0,
            "last_spike_time": -1000,
            "type": neuron_type,
            "base_activity": 0.1,
            "created_time": self.global_time,
            "source": source
        }

        return neuron_id

    def _create_synapse(self, pre_id: int, post_id: int, initial_weight: float = 0.4):
        """创建新突触连接"""
        if pre_id not in self.synapses:
            self.synapses[pre_id] = {}

        if post_id not in self.synapses[pre_id]:
            self.synapses[pre_id][post_id] = initial_weight
            self.synapse_history[(pre_id, post_id)] = deque(maxlen=100)

    def _get_neuron_activity(self, neuron_id: int) -> float:
        """获取神经元近期活动水平"""
        activity_deque = self.neuron_activity[neuron_id]
        if len(activity_deque) == 0:
            return self.neurons[neuron_id]["base_activity"]
        return sum(activity_deque) / len(activity_deque)

    def _update_neuron(self, neuron_id: int, current_input: float) -> bool:
        """更新神经元状态"""
        neuron = self.neurons[neuron_id]

        # 处理不应期
        if neuron["refractory_counter"] > 0:
            neuron["refractory_counter"] -= 1
            neuron["membrane_potential"] = neuron["reset_potential"]
            self.neuron_activity[neuron_id].append(0)
            return False

        # 更新膜电位
        decay = -neuron["membrane_potential"] / neuron["tau"]
        neuron["membrane_potential"] += (decay + current_input) * self.time_step

        # 检查是否发放脉冲
        if neuron["membrane_potential"] >= neuron["threshold"]:
            neuron["membrane_potential"] = neuron["reset_potential"]
            neuron["refractory_counter"] = neuron["refractory_period"]
            neuron["last_spike_time"] = self.global_time
            self.neuron_activity[neuron_id].append(1)
            return True

        self.neuron_activity[neuron_id].append(0)
        return False

    def _update_stdp(self, pre_id: int, post_id: int):
        """应用STDP学习规则"""
        pre_neuron = self.neurons[pre_id]
        post_neuron = self.neurons[post_id]

        # 计算时间差
        delta_t = post_neuron["last_spike_time"] - pre_neuron["last_spike_time"]

        # 更新权重
        if delta_t > 0:
            # LTP (Long-Term Potentiation)
            dw = self.stdp_params["a_plus"] * np.exp(-delta_t / self.stdp_params["tau_plus"])
            dw *= (1.0 + self.neuromodulators["dopamine"])
        else:
            # LTD (Long-Term Depression)
            dw = -self.stdp_params["a_minus"] * np.exp(delta_t / self.stdp_params["tau_minus"])

        new_weight = self.synapses[pre_id][post_id] + dw
        self.synapses[pre_id][post_id] = max(0.01, min(1.0, new_weight))

    def _update_structural_plasticity(self):
        """动态调整网络结构"""
        # 突触生长
        active_neurons = [nid for nid in self.neurons
                          if self._get_neuron_activity(nid) > self.plasticity_params["growth_threshold"]]

        if active_neurons:
            for _ in range(min(5, len(active_neurons) // 2)):
                pre_id, post_id = random.sample(active_neurons, 2)
                if pre_id != post_id:
                    self._create_synapse(pre_id, post_id, initial_weight=0.3)

        # 突触修剪
        to_prune = []
        for pre_id, posts in list(self.synapses.items()):
            for post_id in list(posts.keys()):
                synapse_activity = sum(self.synapse_history.get((pre_id, post_id), deque())) / 100.0
                if synapse_activity < self.plasticity_params["prune_threshold"]:
                    to_prune.append((pre_id, post_id))

    def _update_neuromodulators(self, novelty: float = 0.0, reward: float = 0.0):
        """更新神经调节物质水平"""
        self.neuromodulators["dopamine"] = min(1.0, novelty * 0.8 + reward * 0.7)
        self.neuromodulators["dopamine"] *= 0.95

        self.neuromodulators["acetylcholine"] = 0.5

    def _tokenize_text(self, text: str) -> List[str]:
        """使用jieba进行中文分词，处理英文单词"""
        # 如果是英文，按空格分词；如果是中文，使用jieba分词
        if any('\u4e00' <= char <= '\u9fff' for char in text):
            # 中文文本，使用jieba分词
            words = list(jieba.cut(text))
        else:
            # 英文文本，按空格分词
            words = text.split()
        
        # 过滤空字符串
        return [word for word in words if word.strip()]

    def _encode_text(self, text: str, source: str) -> List[str]:
        """处理输入文本，创建所有词语的神经元，并标记来源"""
        # 添加结束标识符
        if source == "user":
            text += self.EOS_USER  # 用户输入结束
        elif source == "ai":
            text += self.EOS_AI  # AI输出结束

        # 词语级处理
        words = self._tokenize_text(text)

        for word in words:
            if word not in self.word_neurons:
                # 创建神经元时标记来源
                neuron_id = self._create_neuron(neuron_type="word", source=source)
                self.word_neurons[word] = neuron_id
                self.neuron_words[neuron_id] = word
                self.word_sources[word] = source
                self.neuron_sources[neuron_id] = source
                self._update_neuromodulators(novelty=0.7)
            else:
                # 更新现有词语的来源信息
                neuron_id = self.word_neurons[word]
                self.word_sources[word] = source
                self.neuron_sources[neuron_id] = source
        return words

    def _stimulate_word(self, word: str, strength: float = 0.8) -> int:
        """刺激词语神经元"""
        if word in self.word_neurons:
            neuron_id = self.word_neurons[word]
            self.neurons[neuron_id]["membrane_potential"] = min(
                self.neurons[neuron_id]["threshold"] * 1.8,
                self.neurons[neuron_id]["membrane_potential"] + strength
            )
            return neuron_id
        return -1

    def _create_word_connections(self, words: List[str]):
        """创建词语之间的连接"""
        if len(words) < 2:
            return

        # 创建序列关联
        for i in range(len(words) - 1):
            pre_word = words[i]
            post_word = words[i + 1]

            if pre_word in self.word_neurons and post_word in self.word_neurons:
                pre_id = self.word_neurons[pre_word]
                post_id = self.word_neurons[post_word]

                # 创建或加强连接
                if post_id in self.synapses[pre_id]:
                    self.synapses[pre_id][post_id] = min(1.0, self.synapses[pre_id][post_id] + 0.1)
                else:
                    self._create_synapse(pre_id, post_id, initial_weight=0.5)
        
        # 创建长距离连接（跨词连接）
        if len(words) > 3:
            for i in range(len(words) - 2):
                pre_word = words[i]
                post_word = words[i + 2]
                
                if pre_word in self.word_neurons and post_word in self.word_neurons:
                    pre_id = self.word_neurons[pre_word]
                    post_id = self.word_neurons[post_word]
                    
                    if post_id not in self.synapses[pre_id]:
                        self._create_synapse(pre_id, post_id, initial_weight=0.3)

    def _process_input(self, text: str, source: str):
        """处理输入文本，指定来源"""
        words = self._encode_text(text, source)
        self._update_neuromodulators(novelty=0.4)

        for word in words:
            neuron_id = self._stimulate_word(word)
            if neuron_id != -1:
                self.context.append(neuron_id)

        self._stimulate_neuron(self.input_interface, strength=1.0)
        self._run_network(iterations=15)
        self._create_word_connections(words)

    def _stimulate_neuron(self, neuron_id: int, strength: float = 1.0):
        """直接刺激神经元"""
        self.neurons[neuron_id]["membrane_potential"] = min(
            self.neurons[neuron_id]["threshold"] * 1.5,
            self.neurons[neuron_id]["membrane_potential"] + strength
        )

    def _run_network(self, iterations: int = 15):
        """运行网络多个时间步"""
        for _ in range(iterations):
            self.global_time += self.time_step
            spikes = []

            # 更新所有神经元
            for neuron_id in list(self.neurons.keys()):
                current_input = 0.0

                # 来自突触前神经元的输入
                for pre_id, synapses in self.synapses.items():
                    if neuron_id in synapses and self.neurons[pre_id][
                        "last_spike_time"] == self.global_time - self.time_step:
                        current_input += synapses[neuron_id] * (1.0 + self.neuromodulators["acetylcholine"])

                # 更新神经元
                if self._update_neuron(neuron_id, current_input):
                    spikes.append(neuron_id)

            # 应用STDP学习
            for spike_id in spikes:
                for pre_id in list(self.synapses.keys()):
                    if spike_id in self.synapses[pre_id]:
                        self._update_stdp(pre_id, spike_id)
                        self.synapse_history[(pre_id, spike_id)] = deque(maxlen=100)

            # 更新结构可塑性
            if self.global_time % 50 == 0:
                self._update_structural_plasticity()

    def _activate_response_pathway(self):
        """激活完全自主的响应通路"""
        self.response_pathway = []

        # 如果没有上下文，从所有词语中随机选择起点
        if not self.context:
            if self.word_neurons:
                random_word = random.choice(list(self.word_neurons.keys()))
                start_neuron = self.word_neurons[random_word]
                self._stimulate_neuron(start_neuron, strength=1.5)
                self.context.append(start_neuron)
            else:
                return

        # 从上下文中选择起始神经元
        start_neuron = random.choice(list(self.context))

        # 激活响应通路
        current_neuron = start_neuron
        self.response_pathway.append(current_neuron)

        # 设置一个非常大的步数上限以防无限循环
        max_steps = 100000
        step_count = 0

        while True:
            # 获取当前神经元的连接
            if current_neuron not in self.synapses:
                break

            connections = self.synapses[current_neuron]
            total_weight = sum(connections.values())
            if total_weight == 0:
                break

            # 基于权重概率选择
            rand_val = random.uniform(0, total_weight)
            cumulative = 0
            next_neuron = None

            for post_id, weight in connections.items():
                cumulative += weight
                if rand_val <= cumulative:
                    next_neuron = post_id
                    break

            if next_neuron is None:
                break

            # 刺激下一个神经元
            self._stimulate_neuron(next_neuron, strength=0.8)
            current_neuron = next_neuron
            self.response_pathway.append(current_neuron)

            # 如果到达AI结束神经元，则停止
            if current_neuron == self.eos_ai_neuron:
                break

            # 防止无限循环
            step_count += 1
            if step_count >= max_steps:
                break

        # 强化响应通路连接
        for i in range(len(self.response_pathway) - 1):
            pre_id = self.response_pathway[i]
            post_id = self.response_pathway[i + 1]
            if post_id not in self.synapses[pre_id]:
                self._create_synapse(pre_id, post_id, initial_weight=0.6)
            else:
                self.synapses[pre_id][post_id] = min(1.0, self.synapses[pre_id][post_id] + 0.1)

    def _generate_response(self) -> str:
        """生成完全自主的响应"""
        # 激活响应通路
        self._activate_response_pathway()

        # 收集响应通路中的词语
        response_words = []
        for neuron_id in self.response_pathway:
            if neuron_id in self.neuron_words:
                word = self.neuron_words[neuron_id]
                # 如果遇到AI结束符，停止收集
                if word == self.EOS_AI:
                    break
                response_words.append(word)

        # 如果没有词语，使用基础响应
        if not response_words:
            return ""

        # 组合成响应字符串
        return "".join(response_words) if any('\u4e00' <= char <= '\u9fff' for char in "".join(response_words)) else " ".join(response_words)

    def learn_from_conversation(self, user_input: str) -> str:
        """从对话中学习并生成响应"""
        # 处理用户输入，标记为user
        self._process_input(user_input, source="user")
        self.conversation_history.append(("用户", user_input, datetime.datetime.now()))

        # 生成响应
        response = self._generate_response()

        # 将自身输出作为学习材料，标记为ai
        if response:  # 确保有响应才学习
            self._process_input(response, source="ai")
            self.conversation_history.append(("星忆", response, datetime.datetime.now()))

        self.save_model()
        return response

    def save_model(self):
        """保存当前模型状态"""
        model_data = {
            "global_time": self.global_time,
            "neurons": self.neurons,
            "synapses": dict(self.synapses),
            "word_neurons": self.word_neurons,
            "neuron_words": self.neuron_words,
            "conversation_history": self.conversation_history,
            "neuron_id_counter": self.neuron_id_counter,
            "word_sources": self.word_sources,
            "neuron_sources": self.neuron_sources,
            "EOS_USER": self.EOS_USER,
            "EOS_AI": self.EOS_AI,
        }

        with open(self.model_path, "wb") as f:
            pickle.dump(model_data, f)

    def load_model(self):
        """加载保存的模型"""
        if not os.path.exists(self.model_path):
            print("未找到模型文件，创建新模型")
            return False

        try:
            with open(self.model_path, "rb") as f:
                model_data = pickle.load(f)

            self.global_time = model_data["global_time"]
            self.neurons = model_data["neurons"]

            # 恢复突触连接
            self.synapses = defaultdict(dict)
            for pre_id, connections in model_data["synapses"].items():
                self.synapses[pre_id] = connections

            self.word_neurons = model_data["word_neurons"]
            self.neuron_words = model_data["neuron_words"]
            self.conversation_history = model_data["conversation_history"]
            self.neuron_id_counter = model_data["neuron_id_counter"]

            # 加载来源信息
            self.word_sources = model_data.get("word_sources", {})
            self.neuron_sources = model_data.get("neuron_sources", {})

            # 加载特殊控制符
            self.EOS_USER = model_data.get("EOS_USER", '\x01')
            self.EOS_AI = model_data.get("EOS_AI", '\x02')

            # 获取特殊神经元ID
            self.eos_user_neuron = self.word_neurons.get(self.EOS_USER)
            self.eos_ai_neuron = self.word_neurons.get(self.EOS_AI)

            # 如果特殊神经元不存在，重新创建
            if self.eos_user_neuron is None:
                self.eos_user_neuron = self._create_neuron(neuron_type="special", source="system")
                self.word_neurons[self.EOS_USER] = self.eos_user_neuron
                self.neuron_words[self.eos_user_neuron] = self.EOS_USER

            if self.eos_ai_neuron is None:
                self.eos_ai_neuron = self._create_neuron(neuron_type="special", source="system")
                self.word_neurons[self.EOS_AI] = self.eos_ai_neuron
                self.neuron_words[self.eos_ai_neuron] = self.EOS_AI

            print(f"模型加载成功，包含 {len(self.word_neurons)} 个词语")
            return True
        except Exception as e:
            print(f"模型加载失败: {str(e)}")
            return False

    def get_model_info(self):
        """获取模型信息"""
        synapse_count = sum(len(connections) for connections in self.synapses.values())
        return {
            "word_count": len(self.word_neurons),
            "neuron_count": len(self.neurons),
            "synapse_count": synapse_count,
            "conversation_count": len(self.conversation_history) // 2,
            "global_time": self.global_time
        }

    def interactive_dialog(self):
        """启动交互式对话"""
        print("XINGYI - Made by GMUnitX.")
        print("输入 'exit' 退出对话")
        print("----------------------------")
        info = self.get_model_info()
        print(f"当前模型: {info['word_count']}词语/{info['synapse_count']}连接")

        while True:
            try:
                user_input = input(">")
                if user_input.lower() in ['exit', '退出']:
                    break

                response = self.learn_from_conversation(user_input)
                print(f"XINGYI：{response}")

                # 显示模型状态
                info = self.get_model_info()
                print(f"[系统] 神经元: {info['neuron_count']} | 连接: {info['synapse_count']}")
                print()
            except KeyboardInterrupt:
                print("\n对话已中断")
                break
            except Exception as e:
                print(f"处理错误: {str(e)}")
                continue


# 启动交互式对话
if __name__ == "__main__":
    # 创建并运行对话系统
    brain_net = PureSelfOrganizingBrain()
    brain_net.interactive_dialog()
