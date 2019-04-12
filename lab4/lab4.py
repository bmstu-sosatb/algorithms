import matplotlib.pyplot as plt
import numpy as np

def phi_k(x, k):
    return x ** k

def input_points(filename):
    file = open (filename, 'r')
    table = []
    for line in file:
        x,y,p = map(float, line.split())
        table.append({'x': x, 'y': y, 'p': p})
    file.close()
    return table

def form_matrix(table, n):
    matrix = np.zeros((n + 1, n + 1))
    right_part = np.zeros(n + 1)
    N = len(table)
    for k in range(n + 1):
        for m in range(n + 1):            
            for i in range(N):
                matrix[k][m] += table[i]['p'] * phi_k(table[i]['x'], k + m)
                if m == 0:
                    right_part[k] += table[i]['p'] * table[i]['y'] * phi_k(table[i]['x'], k)
    return matrix, right_part

def solve(matrix, right_part):
    n = len(matrix)
    a = np.zeros(n)
    for i in range(n - 1):
        for icur in range(i + 1, n):
            if matrix[i][i] != 0:
                d = - matrix[icur][i] / matrix[i][i]
                for j in range(i, n):
                    matrix[icur][j] += d * matrix[i][j]
                right_part[icur] += d * right_part[i]
                
    flag = 0
    for i in range(n):
        if matrix[i][i] != 0:
            right_part[i] /= matrix[i][i]
            for j in range(n - 1, i - 1, -1):
                matrix[i][j] /= matrix[i][i]
        else:
            flag = 1
    if flag:
        print('Zero division error')

    for i in range(n - 1, -1, -1):
        a[i] += right_part[i]
        for j in range(n - 1, i, -1):
            a[i] -= a[j] * matrix[i][j]

    return a

def draw(table, a, n):
    N = len(table)
    x = np.linspace(table[0]['x'], table[N - 1]['x'], 100)
    y = []
    for xcur in x:
        elem = 0
        for i in range(n + 1):
            elem += phi_k(xcur, i) * a[i]
        y.append(elem)
            
    plt.plot(x, y, color = 'red', label = 'aprox')

    for i in range(N):
        plt.scatter(table[i]['x'], table[i]['y'], color = 'black')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.show()

def print_table(table):
    print('    x    |    y    |    p    ')
    print('-----------------------------')
    for row in table:
        print('%9.3f|%9.3f|%9.3f' % (row['x'], row['y'], row['p']))

filename = 'in.txt'
table = input_points(filename)
print_table(table)
n = int(input('Введите n: '))
matrix, right_part = form_matrix(table, n)
a = solve(matrix, right_part)
print(a)
draw(table, a, n)
        
        
