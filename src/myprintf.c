
#include "myprintf.h"

#define INT_BUF_SIZE 30

/**
 * Write an integer (and null-terminator) to a string buffer as a decimal
 * @param out The string buffer
 * @param size The length of the buffer
 * @param n The integer (cast to long to use for both int and unsigned)
 * @param n_pad The minimum number of pad characters
 * @param c_pad The character to pad with
 * @param force_sign Add this char if number is positive
 * @return The size of string written (not counting null-terminator)
 */
static size_t sprintInt( char out[], size_t size, long n, unsigned short n_pad, char c_pad, char pos_sign, char spec )
{
    char buf[INT_BUF_SIZE];
    int i = 0, j;
    char isNeg = 0;

    /* Write integer (least significant digit first) */
    if ('x' == spec || 'X' == spec)
    {
        /* Hexadecimal */
        if (0 == n) {
            buf[i] = '0';
            ++i;
        }
        else for (; i<INT_BUF_SIZE-1 && i<sizeof(int)*2 && 0 != n; ++i)
        {
            buf[i] = ((n&0xf) < 0xa) ? ((n&0xf)+'0') : ((n&0xf)+('x'==spec?'a':'A')-0xa);
            n >>= 4;
        }

        /* Disable '+' */
        pos_sign = 0;
    }
    else if ('o' == spec)
    {
        /* Octal */
        if (0 == n) {
            buf[i] = '0';
            ++i;
        }
        else for (; i<INT_BUF_SIZE-1 && i<sizeof(int)*8/3+1 && 0 != n; ++i)
        {
            buf[i] = (n&0x7)+'0';
            n >>= 3;
        }

        /* Disable '+' */
        pos_sign = 0;
    }
    else /* 'd' == spec */
    {
        /* Decimal */
        isNeg = (n < 0 ? 1 : 0);

        /* Make n positive; we'll print negative sign later */
        if (isNeg) n *= -1;

        if (0 == n) {
            buf[i] = '0';
            ++i;
        }
        else for (; i<INT_BUF_SIZE-1 && 0 != n; ++i)
        {
            buf[i] = n%10 + '0';
            n /= 10;
        }
    }

    /* Add padding and sign */
    if ('0' == c_pad)
    {
        /* Add padding first, then sign */
        for (; i < n_pad - (isNeg || pos_sign ? 1 : 0); ++i) {
            buf[i] = c_pad;
        }
        
        if (isNeg) 
        {
            buf[i] = '-';
            ++i;
        }
        else if (pos_sign)
        {
            buf[i] = pos_sign;
            ++i;
        }
    }
    else
    {
        /* Add sign first, then padding */
        if (isNeg) {
            buf[i] = '-';
            ++i;
        }
        else if (pos_sign)
        {
            buf[i] = pos_sign;
            ++i;
        }

        for (; i < n_pad; ++i)
        {
            buf[i] = c_pad;
        }
    }

    /* Write to buffer */
    for (j=0; j<i && j<size-1; ++j)
    {
        out[j] = buf[i-j-1];
    }
    out[j] = 0;

    return j;
}


void myvsnprintf( char out[], size_t size, const char *fmt, va_list ap )
{
    int i, j;

    unsigned short n_pad;
    short d_len;
    char c_pad;
    char pos_sign;

    for( i=0, j=0; fmt[i] && j<size-1; ++i )
    {
        if( '%' == fmt[i] )
        {
            n_pad = 0;
            d_len = 5;
            c_pad = ' ';
            pos_sign = 0;
            
            ++i;

            /* Options to ignore for compatability */
            if( '-' == fmt[i] )
            {
                ++i;
            }

            /* Parse options */
            if( '+' == fmt[i] || ' ' == fmt[i] )
            {
                pos_sign = fmt[i];
                ++i;
            }

            if( '0' == fmt[i] )
            {
                c_pad = fmt[i];
                ++i;
            }

            while( fmt[i] >= '1' && fmt[i] <= '9' )
            {
                n_pad = n_pad*10 + (fmt[i] - '0');
                ++i;
            }

            if ( '.' == fmt[i] )
            {
                d_len = 0;
                ++i;
                while( fmt[i] >= '1' && fmt[i] <= '9' )
                {
                    d_len = d_len*10 + (fmt[i] - '0');
                    ++i;
                }
            }

            switch( fmt[i] )
            {
                case '%':
                    out[j++] = '%';
                    break;
                case 'd':
                    j += sprintInt( &out[j], size-j, va_arg( ap, int ), n_pad, c_pad, pos_sign, fmt[i] );
                    break;
                case 'u': case 'x': case 'X': case 'o':
                    j += sprintInt( &out[j], size-j, va_arg( ap, unsigned ), n_pad, c_pad, pos_sign, fmt[i] );
                    break;
                case 'f': case 'F':
#ifdef myprintfDOUBLE
                    /* TODO: Figure out why putting this in a functions causes crash */
//                    j += sprintDouble( &out[j], size-j, va_arg( ap, double ), n_pad, d_len, c_pad, pos_sign, fmt[i] );
                {
                    int k;
                    double d = va_arg( ap, double );
                    long part = d;
                    char small = 0;

                    if (0 == part) {
                        small = 1;
                        part = (d < 0) ? -1 : 1;
                    }

                    /* Print whole number part */
                    j += sprintInt( &out[j], size - j, part, n_pad, c_pad, pos_sign, 'd' );
                    if (small) {
                        out[j-1] = '0';
                    }
                    if (j > size-2) break;

                    /* Print decimal point */
                    out[j++] = '.';
                    out[j] = 0;
                    if (j > size-2) break;

                    /* Print fraction part */
                    if (d<0) d *= -1;
                    part = 0;
                    for( k = 0; k < d_len; ++k )
                    {
                        d *= 10;
                        part = (part * 10) + ((long) d) % 10;
                    }
                    j += sprintInt( &out[j], size - j, part, d_len, '0', 0, 'd' );
                }
#else
                j += sprintInt( &out[j], size-j, va_arg( ap, double ), n_pad, c_pad, pos_sign, 'd' );
#endif
                    break;
                case 'c':
                    out[j++] = va_arg( ap, char );
                    break;
                case 's':
                {
                    char* str;
                    str = va_arg( ap, char* );
                    for( ; *str && j<size-1; ++str )
                    {
                        out[j++] = *str;
                    }
                    break;
                }
                default:
                    out[j++] = '%';
                    if (j<size-1) out[j++] = '?';
            }
            continue;
        }

        out[j++] = fmt[i];
    }

    out[j] = 0;
}

void mysnprintf( char *str, size_t size, const char *fmt, ... )
{
    va_list ap;
    va_start(ap, fmt);
    myvsnprintf( str, size, fmt, ap );
    va_end(ap);
}
