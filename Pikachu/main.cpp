/**
*　　　　　　　　┏┓　　　┏┓+ +
*　　　　　　　┏┛┻━━━┛┻┓ + +
*　　　　　　　┃　　　　　　　┃ 　
*　　　　　　　┃　　　━　　　┃ ++ + + +
*　　　　　　 ████━████ ┃+
*　　　　　　　┃　　　　　　　┃ +
*　　　　　　　┃　　　┻　　　┃
*　　　　　　　┃　　　　　　　┃ + +
*　　　　　　　┗━┓　　　┏━┛
*　　　　　　　　　┃　　　┃　　　　　　　　　　　
*　　　　　　　　　┃　　　┃ + + + +
*　　　　　　　　　┃　　　┃　　　　Code is far away from bug with the animal protecting　　　　　　　
*　　　　　　　　　┃　　　┃ + 　　　　神兽保佑,代码无bug　　
*　　　　　　　　　┃　　　┃
*　　　　　　　　　┃　　　┃　　+　　　　　　　　　
*　　　　　　　　　┃　 　　┗━━━┓ + +
*　　　　　　　　　┃ 　　　　　　　┣┓
*　　　　　　　　　┃ 　　　　　　　┏┛
*　　　　　　　　　┗┓┓┏━┳┓┏┛ + + + +
*　　　　　　　　　　┃┫┫　┃┫┫
*　　　　　　　　　　┗┻┛　┗┻┛+ + + +
*/

#include "Pikachu.h"
#include <QtWidgets/QApplication>
#include <QtGlobal>
#include <QTextStream>

void myMessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
	qInstallMessageHandler(myMessageHandler);
	QApplication app(argc, argv);
	Pikachu pikachu;
	pikachu.show();
	return app.exec();
}



void myMessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg)
{
	QString text;
	QFile *outFile;


	switch (msgType) {
	case QtDebugMsg:
		text = QString("Debug: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//debug.log");
		break;
	case QtInfoMsg:
		text = QString("Info: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//info.log");
		break;
	case QtWarningMsg:
		text = QString("Warning: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//warning.log");
		break;
	case QtCriticalMsg:
		text = QString("Critical: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//critical.log");
		break;
	case QtFatalMsg:
		text = QString("Fatal: (%1:%2, %3)\n%4\n").arg(context.file).arg(context.line).arg(context.function).arg(msg);
		outFile = new QFile("..//Log//fatal.log");
		abort();
	}

	outFile->open(QIODevice::WriteOnly | QIODevice::Append);
	//QTextStream textStream(outFile);
	//textStream << text;
	outFile->write(text.toStdString().c_str(), qstrlen(text.toStdString().c_str()));
	outFile->close();

	delete outFile;
}