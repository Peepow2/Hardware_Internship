def map_unit(value, A, B, C, D):
    return ((D-C)/(B-A)) * value + C 

print(T, '-->', map_unit(T, 0, 100, 32, 212))
