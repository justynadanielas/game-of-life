import random
from typing import List

def create_array(n: int) -> List[List[int]]:
    array = []
    array_rows = []
    for i in range(0, n):
        for j in range(0, n):
            array_rows.append(random.randint(0, 1))
        array.append(array_rows)
        array_rows = []
    return array
