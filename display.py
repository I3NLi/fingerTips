import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
import numpy as np
import requests
import threading

# 定义初始值
defaultCenters = [(0.5, 0.5), (0.5, 0.5), (0.5, 0.5), (0.5, 0.5), (0.5, 0.5)]
defaultRadii = [0.0, 0.0, 0.0, 0.0, 0.0]

range=255

centers = [(0.5, 0.5), (0.5, 0.5), (0.5, 0.5), (0.5, 0.5), (0.5, 0.5)]
radii = [0.1, 0.1, 0.1, 0.1, 0.1]
temperature = [0, 0, 0, 0, 0]

# 创建多个图形和轴，每个传感器一个图形
fig, axes = plt.subplots(nrows=1, ncols=5, figsize=(25, 5))
plt.subplots_adjust(left=0.05, bottom=0.1, top=0.9, wspace=0.3)

# 定义渐变颜色映射
cmap = LinearSegmentedColormap.from_list("heatmap_gradient", ["blue", "yellow", "red"])

# 初始化每个子图的背景和文本
background_gradients = []
texts = []

for i, ax in enumerate(axes):
    # 创建网格数据，覆盖整个画布
    x = np.linspace(0, 1, 500)
    y = np.linspace(0, 1, 500)
    X, Y = np.meshgrid(x, y)
    Z = np.exp(-((X - centers[i][0]) ** 2 + (Y - centers[i][1]) ** 2) / (2 * (radii[i] ** 2)))
    
    # 使用颜色映射创建热力图背景
    background_gradient = ax.pcolormesh(X, Y, Z, shading='auto', cmap=cmap)
    background_gradients.append(background_gradient)
    
    # 添加文本显示
    text = ax.text(0.05, 0.9, '', transform=ax.transAxes, fontsize=12, color='white', bbox=dict(facecolor='black', alpha=0.5))
    texts.append(text)
    
    # 设置坐标范围和比例
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)
    ax.set_aspect('equal')
    ax.set_title(f'Sensor {i+1}')

# 更新函数
def update():
    for i, ax in enumerate(axes):
        Z = np.exp(-((X - centers[i][0]) ** 2 + (Y - centers[i][1]) ** 2) / (2 * (radii[i] ** 2)))
        background_gradients[i].set_array(Z.ravel())
        texts[i].set_position((0.05, 0.9))
        texts[i].set_text(f'T: {temperature[i]}')
    fig.canvas.draw_idle()

# 函数：从URL获取数据并更新图表
def fetch_data_and_update(url):
    global axes
    response = requests.get(url)
    print(response)
    if response.status_code == 200:
        data = response.json()
        print(data)
        sensors = data

        # 压缩数据并根据centers进行偏移
        for i, (key, sensor) in enumerate(sensors.items()):
            if key == "timestamp":
                continue
            centers[i] = (defaultCenters[i][0] + abs(sensor['x']) / range * 0.2 - 0.1, defaultCenters[i][1] + abs(sensor['y']) / range * 0.2 - 0.1)
            radii[i] = abs(sensor['z']) / range * 0.1 - 0.05
            temperature[i] = sensor['t']
        update()
    else:
        print(f"Failed to fetch data: {response.status_code}")


# 示例URL
# url = "http://127.0.0.1:5000/api/heatmap_data"
url = "http://192.168.4.1/data"


# 定时器函数，每秒请求一次数据
def periodic_fetch():
    fetch_data_and_update(url)
    threading.Timer(0.1, periodic_fetch).start()

# 启动定时器
periodic_fetch()

# 显示图形
plt.show()
