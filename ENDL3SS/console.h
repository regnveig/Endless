#ifndef CONSOLE_H
#define CONSOLE_H

#include <QMainWindow>
#include <QDebug>

namespace Ui { class Console; }

class Console : public QMainWindow {

    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);
    ~Console();

signals:
    void SkyConsole(QString var, QString value);

public slots:
    void ConsoleOutput(QString message);

private slots:
    void SendCommand();

private:
    Ui::Console *ui;
};

#endif // CONSOLE_H
