int main(int argc, char * argv [])
{
        int a, b, c, d, e, f;

        a = 50;
        b = argc + a;
        c = 96;
        e = b + c;

        if (a < b)
        {
                f = b - a;
                e = c * b;
        }
        else
        {
                f = b + a;
                e = c * b;
        }
        b = a - c;
        d = b + f;

        return 0;
}
