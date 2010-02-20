#define JULIAN_1_CE 1721426
#define JULIAN_1970_CE 2440588
#define JULIAN_DAY_CUTOVER 2299161
#define GREGORIAN_CUTOVER_YEAR 1582
#define JULIAN_DAY_1_JAN 1721426
#define MILLIS_PER_DAY 86400000

static const int16_t c_num_days[]
= {0,31,59,90,120,151,181,212,243,273,304,334,
   0,31,60,91,121,152,182,213,244,274,305,335};

void clock_date_compute(double time, int32_t *month, int32_t *day_of_month,
	int32_t	*day_of_year, int32_t *year);
void clock_time_compute(double time, int32_t *seconds, int32_t *minutes,
	int32_t *hours);
