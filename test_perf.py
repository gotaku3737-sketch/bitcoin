import timeit

setup = """
CHARSET = "qpzry9x8gf2tvdw0s3jn54khce6mua7l"
CHARSET_DICT = {c: i for i, c in enumerate(CHARSET)}

def bech32_decode_old(bech):
    bech = bech.lower()
    pos = bech.rfind('1')
    if not all(x in CHARSET for x in bech[pos+1:]):
        return (None, None, None)
    data = [CHARSET.find(x) for x in bech[pos+1:]]
    return data

def bech32_decode_new(bech):
    bech = bech.lower()
    pos = bech.rfind('1')
    if not all(x in CHARSET_DICT for x in bech[pos+1:]):
        return (None, None, None)
    data = [CHARSET_DICT[x] for x in bech[pos+1:]]
    return data

bech = 'bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4'
"""

print("Old:", timeit.timeit("bech32_decode_old(bech)", setup=setup, number=100000))
print("New:", timeit.timeit("bech32_decode_new(bech)", setup=setup, number=100000))
