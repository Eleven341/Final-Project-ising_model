import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("ising_single_run.csv")

plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(df["Step"], df["Energy"], label="Energy", color="blue")
plt.xlabel("Step")
plt.ylabel("Energy")
plt.title("Energy vs Step")
plt.grid(True)

plt.subplot(1, 2, 2)
plt.plot(df["Step"], df["Magnetization"], label="Magnetization", color="orange")
plt.xlabel("Step")
plt.ylabel("Magnetization")
plt.title("Magnetization vs Step")
plt.grid(True)

plt.tight_layout()
plt.show()
