def input_points(filename):
    file = open (filename, 'r')
    points = []
    for line in file:
        x,y = map(float, line.split())
        points.append({'x': x, 'y': y})
    file.close()
    return points

def f(x, a0, a1, a2):
    return (a0*x)/(a1+a2*x)

def f_(x, a0, a1, a2):
    return (a0*a1)/(a1+a2*x)**2

def generate_func(fromx, tox, step, a0, a1, a2):
    points = []
    while fromx <= tox:
        if a1+a2*fromx != 0:
            points.append({'x': fromx, 'y': f(fromx, a0, a1, a2)})
        fromx += step
    return points

def leftside(points, n, h):
    p1 = []
    p1.append('-')
    for i in range(1,n):
        p1.append((points[i]['y'] - points[i-1]['y'])/h)
    return p1

def higher(points, n, h):
    p2 = ['-' for i in range(n)]
    p2[0] = (-3*points[0]['y'] + 4*points[1]['y'] - points[2]['y'])/(2*h)
    p2[n-1] = (3*points[n-1]['y'] - 4*points[n-2]['y'] + points[n-3]['y'])/(2*h)
    return p2

def centerpoints(points, n, h):
    p3 = []
    p3.append('-')
    for i in range(1,n-1):
        p3.append((points[i+1]['y'] - points[i-1]['y'])/(2*h))
    p3.append('-')
    return p3
    
def runge(points, n, h):
    p4 = []
    p = 1
    for i in range(n-2):
        ksi = (points[i+1]['y'] - points[i]['y'])/h
        ksi2 = (points[i+2]['y'] - points[i]['y'])/(2*h)
        p4.append(ksi + (ksi - ksi2)/(2**p - 1))
    for i in range(n-2,n):
        ksi = (points[i]['y'] - points[i-1]['y'])/h
        ksi2 = (points[i]['y'] - points[i-2]['y'])/(2*h)
        p4.append(ksi + (ksi - ksi2)/(2**p - 1))
    return p4

##def ksif(x):
##    return 1/x
##def etaf(y):
##    return 1/y
def ksif_x(x):
    return -1/x**2
def etaf_y(y):
    return -1/y**2
def etaf_ksi(a0,a1):
    return a1/a0

def alignvars(points, n, a0, a1, a2):
    p5 = []
    for i in range(n):
        if points[i]['x'] != 0:
            p5.append(ksif_x(points[i]['x'])*etaf_ksi(a0,a1)/etaf_y(points[i]['y']))
        else:
            p5.append('-')
    return p5

def exact(points, n, a0, a1, a2):
    p6 = []
    for i in range(n):
        p6.append(f_(points[i]['x'], a0, a1, a2))
    return p6

def print_table(points, n, table):
    print('\n\n')
    print("{:10}|{:10}|{:13}|{:26}|{:11}|{:15}|{:24}|{:15}".format('x','y','левосторонняя','повышение порядка точности',\
                                                                 'центральная','2-ая ф-ла Рунге','выравнивающие переменные',\
                                                                 'точное значение'))
    for i in range(n):
        print('-'*131)
        print("{:10}|{:10.4f}".format(points[i]['x'],points[i]['y']),end = '|')
        if table[0][i] == '-':
            print("{:13}".format('-'),end = '|')
        else:
            print("{:13.4f}".format(table[0][i]),end = '|')

        if table[1][i] == '-':
            print("{:26}".format('-'),end = '|')
        else:
            print("{:26.4f}".format(table[1][i]),end = '|')

        if table[2][i] == '-':
            print("{:11}".format('-'),end = '|')
        else:
            print("{:11.4f}".format(table[2][i]),end = '|')
              
        print("{:15.4f}".format(table[3][i]),end = '|')

        if table[4][i] == '-':
            print("{:24}".format('-'),end = '|')
        else:
            print("{:24.4f}".format(table[4][i]),end = '|')

        print("{:15.4f}".format(table[5][i]))
            

a0,a1,a2 = map(float, input('Введите а0, а1, а2: ').split())
fromx = -5
tox = 5
step = 1
points = generate_func(fromx, tox, step, a0, a1, a2)
n = len(points)
table = []
table.append(leftside(points, n, step))
table.append(higher(points, n, step))
table.append(centerpoints(points, n, step))
table.append(runge(points, n, step))
table.append(alignvars(points, n, a0, a1, a2))
table.append(exact(points, n, a0, a1, a2))
##print(table)
print_table(points, n, table)
