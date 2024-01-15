import asyncio
import sys
import os
import random
import time
import itertools as it

max_await_time = 3


async def makeitem(size: int = 5) -> str:
    return os.urandom(size).hex()


async def randomsleep(caller=None) -> None:
    i = random.randint(0, max_await_time)
    if caller:
        print(f"{caller} sleeping for {i} seconds.")
    await asyncio.sleep(i)


async def produce(name: int, q: asyncio.Queue) -> None:
    n = random.randint(0, max_await_time)
    for _ in it.repeat(None, n):
        await randomsleep(f"Producer {name}")
        item = await makeitem()
        t = time.perf_counter()
        await q.put((item, t))
        print(f"Producer {name} added {item} to queue.")


async def consume(name: int, q: asyncio.Queue) -> None:
    while True:
        await randomsleep(caller=f"Consumer {name}")
        item, t = await q.get()
        now = time.perf_counter()
        print(f"Consumer {name} got {item} from queue in {now - t:0.5f} seconds.")
        q.task_done()


async def AsyncQueue(nprod: int = 3, ncons: int = 5) -> None:
    q = asyncio.Queue()
    producers = [asyncio.create_task(produce(i, q)) for i in range(nprod)]
    consumers = [asyncio.create_task(consume(i, q)) for i in range(ncons)]
    await asyncio.gather(*producers)
    await q.join()
    for c in consumers:
        c.cancel()


def TestAsyncQueue():
    random.seed(int(time.time()))
    start = time.perf_counter()
    asyncio.run(AsyncQueue())
    elapsed = time.perf_counter() - start
    print(f"{__file__} executed in {elapsed:0.2f} seconds.")


from itertools import cycle


def endless():
    yield from cycle((9, 8, 7, 6, 5, 4, 3, 2, 1))
def TestEndless():
    e = endless()
    print(next(e))
    print(next(e))
    print(next(e))

async def gen(u : int = 10):
    i = 0
    while i < u:
        yield i
        i += 1
        await asyncio.sleep(0.1)

async def AsyncGen():
    g = [i async for i in gen()]
    return g
def TestAsyncGen():
    asyncio.run(AsyncGen())


if __name__ == "__main__":
    TestAsyncQueue()

    TestEndless()

    TestAsyncGen()
