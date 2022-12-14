import matplotlib.pyplot as plt

compliance = []
mass = []

with open('q2_result.txt') as result:
    for line in result:
        tokens = line.split()
        if len(tokens) == 2:
            compliance.append(float(tokens[1]))
        elif len(tokens) == 3:
            mass.append(float(tokens[2]))

idx = 0
x = []
y = []

with open('q2_pareto_front_result.txt') as pareto:
    for line in pareto:
        tokens = line.split()
        if len(tokens) == 1 and ":" not in tokens[0]:
            if idx % 2 == 0:
                y.append(float(tokens[0]))
            else:
                x.append(float(tokens[0]))
            idx += 1

plt.plot(mass, compliance, 'bo')
plt.plot(x, y, 'r')
plt.ylabel("Compliance")
plt.xlabel("Total mass")
plt.show()

