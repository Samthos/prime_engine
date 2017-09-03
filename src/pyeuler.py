import prime_engine


class PrimeEngine(prime_engine.PrimeEngine):
    
	def factor_integer(self, n):
		"""Return a dict of {val : occurances} in prime factorization of integer n"""
		return dict(self.FactorInteger_tuples(n))
			
