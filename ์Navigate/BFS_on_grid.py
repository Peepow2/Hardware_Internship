maze = list()
MAP_maze = list()
fp = open("Demo-map.csv", 'r')
for line in fp:
    maze.append(line.strip().split(","))
    MAP_maze.append(line.strip().split(","))
fp.close()



visited = [[False for i in range(len(maze[0]))] for j in range(len(maze))]
# ------------------------------------------ #
def out_of_area(x, y):
    return x < 0 or x >= len(maze[0]) or y < 0 or y >= len(maze)

def wall(x, y):
    return maze[y][x] == '1'
# ------------------------------------------ #
'''
for line in maze:
    for c in line:
        print(c, end = " ")
    print()
print("---------------------------")
'''
Sx, Sy = 0, 13
Ex, Ey = 9, 1

Q = list()
Q.append(("", Sx, Sy, 0))
visited[Sy][Sx] = True
find_path = False

while len(Q) > 0:
    Path, x, y, dist = Q[0]
    Q.pop(0)
    if(x == Ex and y == Ey):
        find_path = True
        break
    
    if(not out_of_area(x+1, y) and not wall(x+1, y) and not visited[y][x+1]):
        Q.append((Path + 'R', x+1, y, dist+1))
        visited[y][x+1] = True

    if(not out_of_area(x-1, y) and not wall(x-1, y) and not visited[y][x-1]):
        Q.append((Path + 'L', x-1, y, dist+1))
        visited[y][x-1] = True
        
    if(not out_of_area(x, y+1) and not wall(x, y+1) and not visited[y+1][x]):
        Q.append((Path + 'D', x, y+1, dist+1))
        visited[y+1][x] = True

    if(not out_of_area(x, y-1) and not wall(x, y-1) and not visited[y-1][x]):
        Q.append((Path + 'U', x, y-1, dist+1))
        visited[y-1][x] = True

if(find_path):
    MAP_maze[Sy][Sx] = 'S'
    MAP_maze[Ey][Ex] = 'E'
    for i in range(len(Path)-1):
        if Path[i] == 'U': Sy -= 1
        if Path[i] == 'D': Sy += 1
        if Path[i] == 'R': Sx += 1
        if Path[i] == 'L': Sx -= 1
        MAP_maze[Sy][Sx] = '*'

    for line in MAP_maze:
        for c in line:
            print(c, end = " ")
        print()
    print(Path, dist)

else:
    print("No Way to go")

