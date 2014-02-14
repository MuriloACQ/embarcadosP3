#define ZERO 48
#define NINE 57
#define MINUS 45
#define DECPNT 46

long strtolng_n(char* str, int n) {
	int sign = 1;
	int place = 1;
	long ret = 0;

	int i;
	for (i = n - 1; i >= 0; i--, place *= 10) {
		int c = str[i];
		switch (c) {
		case MINUS:
			if (i == 0)
				sign = -1;
			else
				return -1;
			break;
		default:
			if (c >= ZERO && c <= NINE)
				ret += (c - ZERO) * place;
			else
				return -1;
		}
	}

	return sign * ret;
}

double _double_fraction(char* str, int n) {
	double place = 0.1;
	double ret = 0.0;

	int i;
	for (i = 0; i < n; i++, place /= 10) {
		int c = str[i];
		ret += (c - ZERO) * place;
	}
	return ret;
}
double strtodbl(char* str) {
	int n = 0;
	int sign = 1;
	int d = -1;
	long ret = 0;

	char* temp = str;
	while (*temp != '\0') {
		switch (*temp) {
		case MINUS:
			if (n == 0)
				sign = -1;
			else
				return -1;
			break;
		case DECPNT:
			if (d == -1)
				d = n;
			else
				return -1;
			break;
		default:
			if (*temp < ZERO && *temp > NINE)
				return -1;
		}
		n++;
		temp++;
	}

	if (d == -1) {
		return (double) (strtolng_n(str, n));
	} else if (d == 0) {
		return _double_fraction((str + d + 1), (n - d - 1));
	} else if (sign == -1 && d == 1) {
		return (-1) * _double_fraction((str + d + 1), (n - d - 1));
	} else if (sign == -1) {
		ret = strtolng_n(str + 1, d - 1);
		return (-1) * (ret + _double_fraction((str + d + 1), (n - d - 1)));
	} else {
		ret = strtolng_n(str, d);
		return ret + _double_fraction((str + d + 1), (n - d - 1));
	}
}
