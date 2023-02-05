#include <libcommon.h>

bool _MatchDuration(int16_t actual, int16_t expectation, int16_t inaccuracy)
{
	return abs(expectation - actual) <= inaccuracy;
}