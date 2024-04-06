#ifndef PROCESSINGEXCEPTION_H
#define PROCESSINGEXCEPTION_H

#include <QException>

class ProcessingException : public QException
{
public:
    ProcessingException(QString const& message);
    virtual ~ProcessingException();
    void raise() const;
    ProcessingException *clone() const;
    QString getMessage() const;
private:
    QString message;
};

#endif // PROCESSINGEXCEPTION_H
