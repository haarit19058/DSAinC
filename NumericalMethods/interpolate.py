import matplotlib.pyplot as plt
import numpy as np

# Given points
points = [
    (3.2, 7.5),
    (1.1, 4.8),
    (6.3, 2.2),
    (0.9, 5.4),
    (8.6, 3.3),
    (4.4, 6.6),
    (7.1, 0.5),
    (2.8, 9.0),
    (5.5, 1.7),
    (9.9, 8.8)
]

# Sort by x for interpolation stability
points.sort()

def newton_coefficients(x, y):
    n = len(x)
    a = y.copy()
    for j in range(1, n):
        for i in range(n - 1, j - 1, -1):
            a[i] = (a[i] - a[i - 1]) / (x[i] - x[i - j])
    return a

def newton_eval(x, coeffs, base_x):
    n = len(coeffs)
    result = coeffs[-1]
    for i in range(n - 2, -1, -1):
        result = result * (x - base_x[i]) + coeffs[i]
    return result

# Choose n points (e.g., 4 for 3rd-order polynomial)
n = 10
selected_points = points[:n]
x_vals, y_vals = zip(*selected_points)

# Compute coefficients
coeffs = newton_coefficients(list(x_vals), list(y_vals))

# Evaluate at a specific x
x_target = 1
y_target = newton_eval(x_target, coeffs, x_vals)
print(f"Interpolated value at x = {x_target} is {y_target}")

# Generate points for the polynomial curve
x_plot = np.linspace(min(x_vals), max(x_vals), 200)
y_plot = [newton_eval(xi, coeffs, x_vals) for xi in x_plot]

# Plotting
plt.figure(figsize=(8, 6))
plt.scatter(*zip(*points), color='blue', label='Original Points')
plt.plot(x_plot, y_plot, color='red', label=f'{n-1}th Order Interpolation')
plt.scatter([x_target], [y_target], color='green', label='Interpolated Point', zorder=5)
plt.title("Polynomial Interpolation")
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.legend()
plt.grid(True)
plt.show()
