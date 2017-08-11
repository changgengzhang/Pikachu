#include "Pikachu.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Pikachu pikachu;
	pikachu.show();
	return app.exec();
}
