# map
print(list(map(lambda x: x ** 2, [1, 2, 3])))
import math

print(list(map(math.factorial, range(10))))

def myStartMap(func, iterable):
    for args in iterable:
        yield func(*args)

from itertools import starmap
print(list(starmap(pow, [(2, 5), (3, 2), (10, 3)])))
print(list(myStartMap(pow, [(2, 5), (3, 2), (10, 3)])))

# pip install --upgrade pip aiohttp aiofiles
