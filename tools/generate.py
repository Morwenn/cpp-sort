import sys


def transform(line):
    line = line.strip('[],\n ')
    if line:
        x, y = map(int, line.split(', '))
        return 'detail::swap_if(first[{0}u], first[{1}u], compare);'.format(x, y)


if __name__ == '__main__':
    with open(sys.argv[1]) as source:
        for line in source:
            pairs = line.split('],[')
            for pair in pairs:
                res = transform(pair)
                if res:
                    print(res)
