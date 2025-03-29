import pandas as pd
import matplotlib.pyplot as plt
import csv  # For saving results to csv files
import imageio # For generating animated GIFs
import os

# Average Plots

# Load summary data containing averages and standard deviations for each beta
df = pd.read_csv("Results_100x100/ising_averaged_results_100x100.csv")

# Extract columns
betas = df["Beta"]
avg_E = df["AvgEnergy"]
std_E = df["StdEnergy"]
avg_M = df["AvgMagnetization"]
std_M = df["StdMagnetization"]
plt.figure(figsize=(14, 6))  # Create side by side figure

# Plot average Energy with beta
plt.subplot(1, 2, 1)
plt.errorbar(betas, avg_E, yerr=std_E, fmt='o-', color='blue', ecolor='skyblue', elinewidth=2, capsize=5, markerfacecolor='white')
plt.xlabel("Beta (β)", fontsize=12)
plt.ylabel("Average Energy", fontsize=12)
plt.title("Average Energy vs Beta", fontsize=14)
plt.grid(True, alpha=0.3)
plt.xticks(betas, fontsize=10)
plt.yticks(fontsize=10)

# Plot average Magnetization with beta
plt.subplot(1, 2, 2)
plt.errorbar(betas, avg_M, yerr=std_M, fmt='o-', color='orange', ecolor='navajowhite', elinewidth=2, capsize=5, markerfacecolor='white')
plt.axhline(0, color='gray', linestyle='--', linewidth=1, alpha=0.5)
plt.xlabel("Beta (β)", fontsize=12)
plt.ylabel("Average Magnetization", fontsize=12)
plt.title("Average Magnetization vs Beta", fontsize=14)
plt.grid(True, alpha=0.3)
plt.xticks(betas, fontsize=10)
plt.yticks(fontsize=10)

# Display both subplots
plt.tight_layout()
plt.show()

# Distributions histogram 
dist_df = pd.read_csv("Results_100x100/ising_all_runs_100x100.csv")
unique_betas = sorted(dist_df["Beta"].unique())  # Get all unique beta values to loop through

# Open CSV writers for bin summaries for each beta
energy_csv = open("Results_100x100/energy_distribution_summary_100x100.csv", "w", newline='')
mag_csv = open("Results_100x100/magnetization_distribution_summary_100x100.csv", "w", newline='')
energy_writer = csv.writer(energy_csv)
mag_writer = csv.writer(mag_csv)

# Write header row
energy_writer.writerow(["Beta", "Bin", "Start", "End", "Count"])
mag_writer.writerow(["Beta", "Bin", "Start", "End", "Count"])

# Energy Distributions
for beta in unique_betas:
    subset = dist_df[dist_df["Beta"] == beta]

    plt.figure()
    counts, bin_edges, patches = plt.hist(subset["Energy"], bins=30, edgecolor='black')
    plt.title(f"Energy Distribution at β = {beta}")
    plt.xlabel("Energy")
    plt.ylabel("Frequency")
    plt.grid(True, alpha=0.3)
    plt.show()

    print(f"\n--- Energy Distribution at β = {beta} ---")
    for i in range(len(counts)):
        print(f"Bin {i+1}: range = [{bin_edges[i]:.2f}, {bin_edges[i+1]:.2f}), count = {int(counts[i])}")
        # Save to CSV
        energy_writer.writerow([beta, i+1, bin_edges[i], bin_edges[i+1], int(counts[i])])

# Magnetization Distributions
for beta in unique_betas:
    subset = dist_df[dist_df["Beta"] == beta]

    plt.figure()
    counts, bin_edges, patches = plt.hist(subset["Magnetization"], bins=30, edgecolor='black')
    plt.title(f"Magnetization Distribution at β = {beta}")
    plt.xlabel("Magnetization")
    plt.ylabel("Frequency")
    plt.grid(True, alpha=0.3)
    plt.show()

    print(f"\n--- Magnetization Distribution at β = {beta} ---")
    for i in range(len(counts)):
        print(f"Bin {i+1}: range = [{bin_edges[i]:.2f}, {bin_edges[i+1]:.2f}), count = {int(counts[i])}")
        # Save to CSV
        mag_writer.writerow([beta, i+1, bin_edges[i], bin_edges[i+1], int(counts[i])])

# Close files
energy_csv.close()
mag_csv.close()

print("\nSaved histogram bin data to:")
print("  - Results_100x100/energy_distribution_summary_100x100.csv")
print("  - Results_100x100/magnetization_distribution_summary_100x100.csv")

# Read all saved frame CSVs and create PNGs
frames = []
file_list = sorted(os.listdir("Results_100x100/frames_100x100"), key=lambda x: int(x.split("_")[1].split(".")[0]))

for file in file_list:
    data = pd.read_csv(f"Results_100x100/frames_100x100/{file}", header=None)
    fig, ax = plt.subplots(figsize=(6, 6))
    ax.imshow(data, cmap='coolwarm', vmin=-1, vmax=1)
    ax.axis('off')
    step_num = file.split("_")[1].split(".")[0]
    
    # Each frame represents 1,000,000 / subframes steps
    step_size = 1000000 // 5  # Update this if subframe count changes
    frame_path = f"Results_100x100/frames_png_100x100/{file.replace('.csv', '.png')}"
    plt.savefig(frame_path)
    plt.close()
    frames.append(imageio.imread(frame_path))

# Create animated GIF
imageio.mimsave("Results_100x100/ising_simulation_100x100.gif", frames, duration=0.9)
print("GIF saved as Results_100x100/ising_simulation_100x100.gif")
