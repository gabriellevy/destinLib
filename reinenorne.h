#ifndef REINENORNE_H
#define REINENORNE_H

#include "perso.h"

class ReineNorne : public IPerso
{
    Q_OBJECT
public:
    explicit ReineNorne(QWidget *parent = nullptr);
    virtual void GenererPersos();

signals:

public slots:
};

#endif // REINENORNE_H
