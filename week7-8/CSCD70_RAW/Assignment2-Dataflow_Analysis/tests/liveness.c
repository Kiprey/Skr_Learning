int sum(int a, int b)
{
        int res = 1;
        for (int i = a; i < b; i++)
        {
                res *= i;
        }
        return res;
}
void test(int u,int v)
{
    int i = 1;
    int j, k;
    do{
        if(u < v)
        {
            k = i;
            u = u + 1;
        }
        i = 2;
        v = v - 1;
    }while(v <= 20);
    j = i;
}