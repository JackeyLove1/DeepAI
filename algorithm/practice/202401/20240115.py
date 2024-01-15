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
import asyncio
import random
import time
from typing import List


async def count():
    print("one")
    await asyncio.sleep(1)
    print("two")


async def main():
    await asyncio.gather(count(), count(), count())


def TestAsyncCount():
    import time
    s = time.perf_counter()
    asyncio.run(main())
    elapsed = time.perf_counter() - s
    print(f"{__file__} executed in {elapsed:0.2f} seconds.")


max_await_time = 3
async def part1(n: int) -> str:
    i = random.randint(0, max_await_time)
    print(f"part1 {n} sleeping for {i} seconds")
    await asyncio.sleep(i)
    result = f"result{n}-1"
    print(f"Returning part1({n}) == {result}.")
    return result


async def part2(n: int, arg: str) -> str:
    i = random.randint(0, max_await_time)
    print(f"part2{n, arg} sleeping for {i} seconds.")
    await asyncio.sleep(i)
    result = f"result{n}-2 derived from {arg}"
    print(f"Returning part2{n, arg} == {result}.")
    return result


async def chain(n: int) -> None:
    start = time.perf_counter()
    p1 = await part1(n)
    p2 = await part2(n, p1)
    end = time.perf_counter() - start
    print(f"-->Chained result{n} => {p2} (took {end:0.2f} seconds).")


async def AsyncChainMain(args: List[int]):
    await asyncio.gather(*(chain(n) for n in args))


def TestAsyncChain():
    import sys
    random.seed(123)
    args = [1, 2, 3]
    start = time.perf_counter()
    asyncio.run(AsyncChainMain(args))
    end = time.perf_counter() - start
    print(f"Program finished in {end:0.2f} seconds.")


if __name__ == "__main__":
    TestAsyncCount()

    TestAsyncChain()
