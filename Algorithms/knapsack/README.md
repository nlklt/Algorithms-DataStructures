# Knapsack Problem Solutions

Implementation of the classic Knapsack Problem using three different approaches in C++.

## Problem Description
Given a set of items, each with a weight and a value, determine the number of each item to include in a collection so that the total weight is less than or equal to a given limit `N` and the total value is as large as possible.

## Implementations

### 1. Dynamic Programming (Optimal)
Builds a table `dp[n][w]` to store optimal values for subproblems. Backtracking is used to find which items were selected.
- **Time Complexity:** $O(N \cdot W)$ where $N$ is items count, $W$ is capacity.
- **Space Complexity:** $O(N \cdot W)$.

### 2. Brute Force (Recursion)
Recursively checks every combination of items.
- **Time Complexity:** $O(2^N)$.
- **Note:** Extremely slow for large inputs.

### 3. Greedy Algorithm (Heuristic)
Sorts items by value-to-weight ratio ($v/w$) and picks them until the bag is full.
- **Time Complexity:** $O(N \log N)$ (due to sorting).
- **Note:** Does **NOT** guarantee an optimal solution for the 0/1 Knapsack problem (unlike Fractional Knapsack), but is very fast.
