import sys
import os
import math

class key(object):
    @staticmethod
    def KEY():
        return None

BLOCK_SIZE = 16
UMAX = int(math.pow(256, BLOCK_SIZE))

def to_bytes(n):
    s = hex(n)
    s_n = s[2:]
    if 'L' in s_n:
        s_n = s_n.replace('L', '')
    if len(s_n) % 2 != 0:
        s_n = '0' + s_n
    
    # decoded = s_n.decode('hex')
    decoded = bytes.fromhex(s_n)

    pad = (len(decoded) % BLOCK_SIZE)
    if pad != 0: 
        decoded = b"\0" * (BLOCK_SIZE - pad) + decoded
    return decoded

def remove_line(s):
    # returns the header line, and the rest of the file
    # return s[:s.index('\n') + 1], s[s.index('\n')+1:]

    header = s[:s.index(b'\n') + 1]
    data = s[s.index(b'\n')+1:]
    return header, data

def parse_header_ppm(f):
    data = f.read()
    header = b""

    for i in range(3):
        header_i, data = remove_line(data)
        header += header_i

    return header, data


if __name__ == "__main__":
    sys.modules['key'] = key
    # from aes_abc import parse_header_ppm, to_bytes, BLOCK_SIZE, UMAX

    with open('body.enc.ppm', 'rb') as f:
        header, data = parse_header_ppm(f)
        blocks = [data[i * BLOCK_SIZE:(i+1) * BLOCK_SIZE] for i in range(len(data) // BLOCK_SIZE)]
        new_blocks = []

        for i in range(len(blocks) - 1):
            # prev_blk = int(blocks[i].encode('hex'), 16)
            # curr_blk = int(blocks[i+1].encode('hex'), 16)

            prev_blk = int.from_bytes(blocks[i], 'big')
            curr_blk = int.from_bytes(blocks[i+1], 'big')

            n_curr_blk = (curr_blk - prev_blk)
            if n_curr_blk < 0:
                n_curr_blk += UMAX
            # new_blocks.append(to_bytes(n_curr_blk))
            new_blocks.append(bytes.hex(to_bytes(n_curr_blk)))

        joined = "".join(new_blocks)
        joined  = str.encode('utf-16')

        with open('ecb.ppm', 'wb') as fw:
            fw.write(header)
            fw.write(joined)