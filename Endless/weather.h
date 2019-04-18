#ifndef WEATHER_H
#define WEATHER_H

#include "global.h"

class Weather : public QObject {

    Q_OBJECT

public:

    explicit        Weather         (QObject *parent = nullptr);
                    ~Weather        () override;

signals:

    void            Data            ();

public slots:

    void            Play            (bool play);

protected:

    void            timerEvent      (QTimerEvent *event) override;

private:

};

#endif // WEATHER_H
