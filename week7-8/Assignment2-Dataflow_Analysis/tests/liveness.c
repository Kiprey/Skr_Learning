int sum(int a, int b)
{
        int res = 1;
        for (int i = a; i < b; i++)
        {
                res *= i;
        }
        return res;
}
