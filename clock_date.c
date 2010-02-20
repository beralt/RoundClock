#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "clock_date.h"

int clock_date_year_is_leap_year(int year);
double floor_divide(double num, double den, double *rem);

void clock_date_compute(double time, int32_t *month, int32_t *day_of_month,
	int32_t	*day_of_year, int32_t *year)
{
    double millis_in_day = 0.0;
    double doy = 0.0;

    double day = floor_divide(time, MILLIS_PER_DAY, &millis_in_day) +
	JULIAN_1970_CE - JULIAN_1_CE;

    double n400 = floor_divide(day, 146097, &doy);
    double n100 = floor_divide(doy, 36524, &doy);
    double n4 = floor_divide(doy, 1461, &doy);
    double n1 = floor_divide(doy, 365, &doy);

    *year = (int32_t) (400 * n400 + 100 * n100 + 4 * n4 + n1);
    if(n100 == 4 || n1 == 4)
	doy = 365;
    else
	++(*year);

    bool is_leap = clock_date_year_is_leap_year(*year);
    int32_t correction = 0;
    int32_t march_1 = ((is_leap > 0) ? 60 : 59);
    if(doy >= march_1)
	correction = is_leap ? 1 : 2;
    *month = floor((12 * (doy + correction) + 6) / 367);
    *day_of_month = doy - c_num_days[*month + (is_leap ? 12 : 0)] + 1;
    ++doy;
    *day_of_year = doy;
}

void clock_time_compute(double time, int32_t *seconds, int32_t *minutes,
	int32_t *hours)
{
	double millis_in_day = 0.0;
	floor_divide(time, MILLIS_PER_DAY, &millis_in_day);

	millis_in_day /= 1000;
	*seconds = fmod(millis_in_day, 60.0);
	millis_in_day /= 60;
	*minutes = fmod(millis_in_day, 60.0);
	millis_in_day /= 60;
	*hours = floor(millis_in_day);
}

int clock_date_year_is_leap_year(int year)
{
    return ((year & 0x3) == 0) && ((year % 100 != 0) || (year % 400 == 0));
}

double floor_divide(double num, double den, double *rem)
{
    double quot = floor(num / den);
    *rem = floor(num - (quot * den));
    return quot;
}
