#include "console.h"
#include "ui_console.h"

Console::Console(QWidget *parent) : QMainWindow(parent), ui(new Ui::Console) {

	ui->setupUi(this);
	connect(ui->GoButton, SIGNAL(clicked()), this, SLOT(SendCommand()));
}

Console::~Console() { delete ui; }

void Console::ConsoleOutput(QString message) {

	ui->ConsoleScreen->setTextColor(QColor(0, 0, 255));
	ui->ConsoleScreen->append(message);
	qInfo() << message;
}

void Console::SendCommand() {

	ui->ConsoleScreen->setTextColor(QColor(0, 0, 0));
	ui->ConsoleScreen->append(">> " + ui->CommandLine->text());
	qInfo() << ">> " + ui->CommandLine->text();

	ui->ConsoleScreen->setTextColor(QColor(0, 0, 255));
	QStringList command = ui->CommandLine->text().split(" ");
	ui->CommandLine->clear();

	if ((command[0] == "sky") && (command.size() == 3)) { 
		emit SkyConsole(command[1], command[2]); return; 

	}

	ConsoleOutput("Console: unknown command");
}




