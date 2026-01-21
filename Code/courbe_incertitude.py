import numpy as np
import matplotlib.pyplot as plt
from math import *

temp_reel=[53,53,51,49,47,45,43,42,41,40,38,37,36,35,34,33,32,31,30,28,28,27,26,25,23,19]
temp_affichee=[58,55,53,50,49,46,44,43,42,41,40,38,36,35,35,33,32,31,30,29,28,27,26,25,21,19]

def ecart_type(liste):
    n = len(liste)
    mean = sum(liste) / n
    variance = sum((x - mean) ** 2 for x in liste) / n
    return sqrt(variance)

def plot_with_uncertainty(x, y, uncertainty, title, xlabel, ylabel):
    fig, ax1 = plt.subplots(figsize=(10, 6))
    ax1.plot(x, y, label='Température affichée', color='blue')
    ax1.fill_between(x, np.array(y) - uncertainty, np.array(y) + uncertainty, color='blue', alpha=0.2, label='Incertitude')
    ax1.plot(x, temp_reel, label='Température mesurée', color='red')
    ax1.set_xlabel(xlabel)
    ax1.set_ylabel(ylabel)
    ax1.grid()

    # Axe secondaire pour la différence
    ax2 = ax1.twinx()
    diff = np.abs(np.array(temp_reel) - np.array(y))
    ax2.plot(x, diff, label='|Différence| (mesurée - affichée)', color='green', linestyle=':')
    ax2.set_ylabel('Différence absolue (°C)')
    ax2.set_ylim(0, 10)

    # Gestion des légendes combinées
    lines_1, labels_1 = ax1.get_legend_handles_labels()
    lines_2, labels_2 = ax2.get_legend_handles_labels()
    ax1.legend(lines_1 + lines_2, labels_1 + labels_2, loc='upper right')

    plt.title(title)
    plt.show()
incertitude = ecart_type([abs(a - b) for a, b in zip(temp_reel, temp_affichee)])
x_values = list(range(1, len(temp_reel) + 1))
plot_with_uncertainty(x_values, temp_affichee, incertitude, 'Température Affichée avec Incertitude', 'Index de Mesure', 'Température (°C)')
