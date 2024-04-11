import math
import heapq

ROW = 9
COL = 10

# Define directions (up, down, left, right)
directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]

def is_valid(row, col):
    return 0 <= row < ROW and 0 <= col < COL

def is_unblocked(grid, row, col):
    return grid[row][col] == 1

def is_destination(row, col, dest):
    return row == dest[0] and col == dest[1]

def calculate_h_value(row, col, dest):
    return math.sqrt((row - dest[0]) ** 2 + (col - dest[1]) ** 2)

def trace_path(cell_details, dest):
    print("\nThe Path is ", end="")
    row, col = dest
    path = []
    while (row, col) != (cell_details[row][col]['parent_i'], cell_details[row][col]['parent_j']):
        path.append((row, col))
        row, col = cell_details[row][col]['parent_i'], cell_details[row][col]['parent_j']
    path.append((row, col))
    for p in reversed(path):
        print("->", p, end=" ")
    print()

def a_star_search(grid, src, dest):
    if not is_valid(src[0], src[1]) or not is_valid(dest[0], dest[1]):
        print("Source or destination is invalid")
        return

    if not is_unblocked(grid, src[0], src[1]) or not is_unblocked(grid, dest[0], dest[1]):
        print("Source or the destination is blocked")
        return

    if is_destination(src[0], src[1], dest):
        print("We are already at the destination")
        return

    closed_list = [[False] * COL for _ in range(ROW)]
    cell_details = [[{'parent_i': -1, 'parent_j': -1, 'f': float('inf'), 'g': float('inf'), 'h': float('inf')} for _ in range(COL)] for _ in range(ROW)]

    i, j = src
    cell_details[i][j]['f'] = 0.0
    cell_details[i][j]['g'] = 0.0
    cell_details[i][j]['h'] = 0.0
    cell_details[i][j]['parent_i'] = i
    cell_details[i][j]['parent_j'] = j

    open_list = [(0.0, (i, j))]
    found_dest = False

    while open_list:
        f, (i, j) = heapq.heappop(open_list)
        closed_list[i][j] = True

        if is_destination(i, j, dest):
            print("The destination cell is found")
            trace_path(cell_details, dest)
            found_dest = True
            return

        for d_row, d_col in directions:
            new_row, new_col = i + d_row, j + d_col
            if is_valid(new_row, new_col) and is_unblocked(grid, new_row, new_col) and not closed_list[new_row][new_col]:
                g_new = cell_details[i][j]['g'] + 1.0
                h_new = calculate_h_value(new_row, new_col, dest)
                f_new = g_new + h_new

                if cell_details[new_row][new_col]['f'] == float('inf') or cell_details[new_row][new_col]['f'] > f_new:
                    heapq.heappush(open_list, (f_new, (new_row, new_col)))
                    cell_details[new_row][new_col]['f'] = f_new
                    cell_details[new_row][new_col]['g'] = g_new
                    cell_details[new_row][new_col]['h'] = h_new
                    cell_details[new_row][new_col]['parent_i'] = i
                    cell_details[new_row][new_col]['parent_j'] = j

    if not found_dest:
        print("Failed to find the destination cell")

def read_input_from_file(filename):
    grid = []
    with open(filename, 'r') as file:
        for line in file:
            grid.append(list(map(int, line.strip().split())))
    src = tuple(map(int, input("Enter source coordinates (row col): ").split()))
    dest = tuple(map(int, input("Enter destination coordinates (row col): ").split()))
    return grid, src, dest

def main():
    filename = input("Enter the filename containing grid data: ")
    grid, src, dest = read_input_from_file(filename)

    print("Source:", src)
    print("Destination:", dest)

    a_star_search(grid, src, dest)

if __name__ == "__main__":
    main()
