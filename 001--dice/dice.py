import random
import matplotlib.pyplot as plt

# Create a list to store the results of the rolls
rolls = []

# Set the number of rolls and the frequency of histogram updates
num_rolls = 100000
hist_update_freq = 100

# Simulate rolling two dice num_rolls times
for i in range(num_rolls):
    dice1 = random.randint(1, 6)
    dice2 = random.randint(1, 6)
    rolls.append(dice1 + dice2)
    
    # Check if it's time to update the histogram
    if i % hist_update_freq == 0:
        # Plot a histogram of the results
        plt.hist(rolls, bins=range(2, 14), align='left', rwidth=0.8)
        plt.xlabel('Dice roll result')
        plt.ylabel('Frequency')
        plt.xticks(range(2, 14))
        plt.show()

