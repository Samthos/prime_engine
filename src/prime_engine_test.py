import prime_engine


p = prime_engine.PrimeEngine(1000)
for i in range(100):
    print(i, p.is_prime(i))
