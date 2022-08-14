#include "TsException.h"

const char* TsException::what() const
{
	return exceptionDetail;//∑µªÿ“Ï≥£µƒ√Ë ˆ
}

TsException::TsException(const char* what, int _level) :exceptionDetail(what), level(_level)
{
}
