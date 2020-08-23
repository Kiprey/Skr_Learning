int loop(int a, int b, int c)
{
        int i = 0, ret = 0;

        do {
            while(i)
            {
                int j = a + c;
                if(i == 2)
                    i = 5;
                else
                    i = 3;
                
            }
            if(ret)
                i = 3;
            c = b + 2;
            i++;
        } while (i < a);

        return ret + c;
}
