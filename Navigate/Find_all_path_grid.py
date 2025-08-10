maze = list()
fp = open("Demo-map.csv", 'r')
for line in fp:
    maze.append(line.strip().split(","))
fp.close()
# --------------------------------------------- #
def out_of_area(x, y):
    return y < 0 or y >= len(maze) or x < 0 or x >= len(maze[0])

def wall(x, y):
    return maze[y][x] == '1'

def find_all_path(S, T):
    cnt = 0
    PATH = set()
    stack = list()
    stack.append(("", [], S, 0))
    while len(stack) > 0:
        Path, node_passed, u, dist = stack[-1]
        stack.pop(-1)
        x, y = u
        
        if u == T:
            PATH.add((dist, Path))
            continue

        if(not out_of_area(x+1, y) and (x+1, y) not in node_passed and not wall(x+1, y)):
            stack.append((Path+"R", node_passed + [(x, y)], (x+1, y), dist+1))
            
        if(not out_of_area(x-1, y) and (x-1, y) not in node_passed and not wall(x-1, y)):
            stack.append((Path+"L", node_passed + [(x, y)], (x-1, y), dist+1))       
        
        if(not out_of_area(x, y+1) and (x, y+1) not in node_passed and not wall(x, y+1)):
            stack.append((Path+"D", node_passed + [(x, y)], (x, y+1), dist+1))

        if(not out_of_area(x, y-1) and (x, y-1) not in node_passed and not wall(x, y-1)):
            stack.append((Path+"U", node_passed + [(x, y)], (x, y-1), dist+1))        
    return PATH
# ----------------------------------------------#
S = (0, 13)
T = (7, 2)
All_Path = find_all_path(S, T)

for p in sorted(All_Path):
    dist, Path = p 
    MAP = [list(m) for m in maze]
    Sx, Sy = S
    Tx, Ty = T
    MAP[Sy][Sx] = 'S'
    for w in Path:
        if w == 'L': Sx -= 1
        if w == 'R': Sx += 1
        if w == 'U': Sy -= 1
        if w == 'D': Sy += 1
        MAP[Sy][Sx] = '*'
    MAP[Ty][Tx] = 'T'

    for c in MAP:
        for r in c:
            print(r, end = " ")
        print()
    print("----------------------")

