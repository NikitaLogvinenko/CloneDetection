void foo(float, float);

void sumProd(int n)
{
	float sum = 0.0;//C1
	float prod = 1.0;
	for (int i = 1; i <= n; i++)
	{
		if (n % 2 == 0)
		{
			sum += i;
		}
		prod = prod * i;
		foo(sum, prod);
	}
}