import sys


def transform(line):
    line = line.strip('[],\nSWAP();')
    if line:
        x, y = map(int, line.split(','))
        return 'iter_swap_if(first + {0}u, first + {1}u, compare, projection);'.format(x, y)


if __name__ == '__main__':
    with open(sys.argv[1]) as source:
        for line in source:
            line = line.replace(' ', '')
            line = line.replace('(', '[')
            line = line.replace(')', ']')
            pairs = line.split('],[')
            for pair in pairs:
                res = transform(pair)
                if res:
                    print(res)
