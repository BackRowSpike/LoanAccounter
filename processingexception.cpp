#include "processingexception.h"


ProcessingException::ProcessingException(const QString &message) : message(message) {}

ProcessingException::~ProcessingException() {}

void ProcessingException::raise() const
{
    throw *this;
}

ProcessingException *ProcessingException::clone() const
{
    return new ProcessingException(*this);
}

QString ProcessingException::getMessage() const
{
    return message;
}
