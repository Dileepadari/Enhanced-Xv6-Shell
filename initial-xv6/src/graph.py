import matplotlib.pyplot as plt
from matplotlib.cm import get_cmap

data_file = 'plots.txt'

queue_data = {}

with open(data_file, 'r') as f:
    for line in f:
        if line.startswith('GRAPH'):
            parts = line.strip().split()
            pid = int(parts[3])
            ticks = int(parts[2])
            queue = int(parts[1])
            
            if queue not in queue_data:
                queue_data[queue] = {'x': [], 'y': [], 'label': f'P{queue-3}'}
            
            queue_data[queue]['x'].append(pid)
            queue_data[queue]['y'].append(ticks)


cmap = get_cmap('tab10', len(queue_data))



for queue, data in queue_data.items():
    plt.plot(data['y'], data['x'], label=data['label'], marker='_', markersize=5, color=cmap(queue-8), linestyle='-')


plt.xlabel('Number of Ticks')
plt.ylabel('Queue Number')
plt.title('MLFQ Scheduler Timeline')
plt.legend()

plt.grid(True)
plt.show()
