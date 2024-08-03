#if !defined(MYSTOI_H)
#define MYSTOI_H

// String to unsigned int. Can choose your own end of string char, by default eos = '\0'
inline int myStoui(char *s, char eos = '\0')
{
    int result = 0;
    for (int i = 0; s[i] != eos; i++)
    {
        // Char check
        if (s[i] < '0' || s[i] > '9')
        {
            return -1;
        }
        result = result * 10 + (s[i] - 48);
    }
    return result;
}
// Character integer to int
inline int myCtoui(char c)
{
    if (!(c < '0' || c > '9'))
    {
        return c - (int)'0';
    }
    else
    {
        return -1;
    }
}
#endif // MYSTOI_H
