def find_all_path(V, S, T, Edge_list):
    adj_list = [[] for e in range(V)]
    
    for u, v in Edge_list:
        adj_list[u].append(v)
        adj_list[v].append(u)
        
    stack = list()
    stack.append((str(S), S))
    while(len(stack) > 0):
        Path, u = stack[-1]
        stack.pop(-1)
        
        if u == T:
            print(Path)
            continue
        
        for v in adj_list[u]:
            if str(v) not in Path:
                stack.append((Path+str(v), v))
    return
# ---------------------------------------------- #     
Edge_list = [(0,1),(0,3),(1,2),(1,4),\
             (2,5),(3,4),(3,6),(4,5),\
             (4,7),(5,8),(6,7),(7,8)] 
find_all_path(9, 0, 8, Edge_list)

