//
// Created by aspgo on 3/28/2026.
//

#include <mathutils.h>
#include <cstring>

void sprintfScientificNotation(char *out, double val)
{
    int exp = 0;
    int sign = 0;

    if (val < 0.0)
    {
        sign = 1;
        val = -val;
    }

    if (val != 0.0)
    {
        while (val < 1.0)
        {
            val *= 10.0;
            exp--;
        }
        while (val >= 10.0)
        {
            val /= 10.0;
            exp++;
        }
    }

    const int temp_int = (int) val;
    const int temp_frac = (int) ((val - temp_int) * 10000 + 0.5);

    __builtin_sprintf(
        out, "%s%d.%04dE%d",
        sign ? "-" : "",
        temp_int,
        temp_frac,
        exp
    );
}

void getDirectionFromString(const char *direction_string, double res[3])
{
    res[0] = 0.0;
    res[1] = 0.0;
    res[2] = 0.0;

    if (strchr(direction_string, 'x'))
    {
        res[0] = (direction_string[0] == '-') ? -1.0 : 1.0;
    } else if (strchr(direction_string, 'y'))
    {
        res[1] = (direction_string[0] == '-') ? -1.0 : 1.0;
    } else if (strchr(direction_string, 'z'))
    {
        res[2] = (direction_string[0] == '-') ? -1.0 : 1.0;
    }
}

const char *vectorToString(const double vec[3])
{
    if (vec[0] != 0)
    {
        return (vec[0] > 0) ? "+x" : "-x";
    }
    if (vec[1] != 0)
    {
        return (vec[1] > 0) ? "+y" : "-y";
    }
    if (vec[2] != 0)
    {
        return (vec[2] > 0) ? "+z" : "-z";
    }
    return "0";
}
