void foo(float, float);

void sumProd(int n)
{
	int s = 0;//C1
	int p = 1;
	for (int i = 1; i <= n; i++)
	{
		s = s + i;
		p = p * i;
		foo(s, p);
	}
}