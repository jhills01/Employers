#!/usr/bin/env python

from pwn import *
import binascii
from lenstra import lenstra

MMI = lambda A, n,s=1,t=0,N=0: (n < 2 and t%N or MMI(n, A%n, t, s-A//n*t, N or n),-1)[n<1]

# From http://rosettacode.org/wiki/Miller%E2%80%93Rabin_primality_test#Python:_Probably_correct_answers
import random

_mrpt_num_trials = 5 # number of bases to test
 
def is_probable_prime(n):
    """
    Miller-Rabin primality test.
 
    A return value of False means n is certainly not prime. A return value of
    True means n is very likely a prime.
 
    >>> is_probable_prime(1)
    Traceback (most recent call last):
        ...
    AssertionError
    >>> is_probable_prime(2)
    True
    >>> is_probable_prime(3)
    True
    >>> is_probable_prime(4)
    False
    >>> is_probable_prime(5)
    True
    >>> is_probable_prime(123456789)
    False
 
    >>> primes_under_1000 = [i for i in range(2, 1000) if is_probable_prime(i)]
    >>> len(primes_under_1000)
    168
    >>> primes_under_1000[-10:]
    [937, 941, 947, 953, 967, 971, 977, 983, 991, 997]
 
    >>> is_probable_prime(6438080068035544392301298549614926991513861075340134\
3291807343952413826484237063006136971539473913409092293733259038472039\
7133335969549256322620979036686633213903952966175107096769180017646161\
851573147596390153)
    True
 
    >>> is_probable_prime(7438080068035544392301298549614926991513861075340134\
3291807343952413826484237063006136971539473913409092293733259038472039\
7133335969549256322620979036686633213903952966175107096769180017646161\
851573147596390153)
    False
    """
    assert n >= 2
    # special case 2
    if n == 2:
        return True
    # ensure n is odd
    if n % 2 == 0:
        return False
    # write n-1 as 2**s * d
    # repeatedly try to divide n-1 by 2
    s = 0
    d = n-1
    while True:
        quotient, remainder = divmod(d, 2)
        if remainder == 1:
            break
        s += 1
        d = quotient
    assert(2**s * d == n-1)
 
    # test the base a to see whether it is a witness for the compositeness of n
    def try_composite(a):
        if pow(a, d, n) == 1:
            return False
        for i in range(s):
            if pow(a, 2**i * d, n) == n-1:
                return False
        return True # n is definitely composite
 
    for i in range(_mrpt_num_trials):
        a = random.randrange(2, n)
        if try_composite(a):
            return False
 
    return True # no base tested showed n as composite

# r = remote('jupiter.challenges.picoctf.org', 4557)

# lines = r.recv().split('\n')

c = 63809823008305287279852679000171959409161765461943540940549544702097770018983001189759663009145234376061141977472363834837675171489606544065184420989309447069297207108656945452574611550561115483781098456643803439428413544346115258021974375630863224881645243443743491263260486693115657371377273549392248622596245893060541478436189514528110001291
n = 81717885743899308005098324498665758421697089725771822730845540299414221099333989405903618760894139448040400067717589153564123693404264546824464474508519233608696466819638322076179675762340053621667055618918421520434239814989858287591893654245602769792709482374352610615810429934583966642615539457109023573542354506083087256828624009350500364981
e = 65537

log.info('c: {}'.format(c))
log.info('n: {}'.format(n))
log.info('e: {}'.format(e))

log.info('Factorizing n (ECM)')

factors = [n]
primes = []

while len(factors) > 0:
    if not is_probable_prime(factors[0]):
        f = factors.pop(0)

        ff = False
        work = 1000

        while not ff:
            ff = lenstra(f, work)
            work *= 10

        assert(f % ff == 0)

        # factors.extend([ff, f / ff]) # TOM: This was causing "OverflowError: integer division result too large for a float" to occur. Changed '/' to '//' and it worked!
        factors.extend([ff, f // ff])
    else:
        primes.append(factors.pop(0))

        log.info('Found prime factor #{} ({})'.format(len(primes), primes[-1]))

phi = 1
for p in primes:
    phi *= (p - 1)

log.info('Calculated phi: {}'.format(phi))

d = MMI(e, phi)
log.info('Calculated d: {}'.format(d))

assert((e * d) % phi == 1)

plain = pow(c, d, n)
log.info('Calculated plaintext: {}'.format(plain))
log.info('In hex: {}'.format(hex(plain)))

log.info('Unhexlified: {}'.format(binascii.unhexlify(hex(plain)[2:]))) # python 2