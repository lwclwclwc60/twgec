import random
import math

def generate_map(size=20):
    # Initialize all cells as open ground
    grid = [[' . ' for _ in range(size)] for _ in range(size)]

    # ---- 1. Generate grass patches using clusters to form continuous areas ----
    # Randomly pick 2~7 cluster centers, each grows into a circular/elliptical grass block
    num_clusters = random.randint(2, 7)
    for _ in range(num_clusters):
        # Random center
        cr = random.randint(0, size - 1)
        cc = random.randint(0, size - 1)
        # Radius 1~2 to make patches of varying sizes
        radius = random.randint(1, 2)

        for r in range(max(0, cr - radius), min(size - 1, cr + radius) + 1):
            for c in range(max(0, cc - radius), min(size - 1, cc + radius) + 1):
                # Circular range with 70% fill probability to create uneven, natural edges
                dist = math.hypot(r - cr, c - cc)
                if dist <= radius and random.random() < 0.7:
                    grid[r][c] = '***'

    # ---- 2. Place trees with very low probability, scattered randomly ----
    for r in range(size):
        for c in range(size):
            if random.random() < 0.01:  # 1% probability, about 4 trees on average for 20x20
                grid[r][c] = 't00'

    # Convert to string rows (each cell already has spaces, join with empty string)
    return [''.join(row) for row in grid]


if __name__ == "__main__":
    # Uncomment the next line to fix the seed for reproducible results
    # random.seed(42)

    map_data = generate_map()
    # Output in the same string-array format as your example
    print("[\n " + ",\n ".join(f'"{row}"' for row in map_data) + "\n]")