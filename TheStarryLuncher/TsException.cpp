#include "TsException.h"

const char* TsException::what() const
{
	return exceptionDetail;//�����쳣������
}

TsException::TsException(const char* what, int _level) :exceptionDetail(what), level(_level)
{
}
