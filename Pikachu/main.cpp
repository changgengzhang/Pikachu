/**
*　　　　　　　　�芥掘　　？芥�+ +
*　　　　　　　�芥食潯ォォォ食潯� + +
*　　　　　　　�А　　　　　　？� 　
*　　　　　　　�А　　？ァ　　？� ++ + + +
*　　　　　　 ���������エ������� ��+
*　　　　　　　�А　　　　　　？� +
*　　　　　　　�А　　？漾　　？�
*　　　　　　　�А　　　　　　？� + +
*　　　　　　　�皐ォ掘　　？芥ォ�
*　　　　　　　　　�А　　？А　　　　　　　　　　�
*　　　　　　　　　�А　　？� + + + +
*　　　　　　　　　�А　　？А　　　�Code is far away from bug with the animal protecting　　　　　　　
*　　　　　　　　　�А　　？� + 　　　　舞舗隠嗷,旗鷹涙bug　　
*　　　　　　　　　�А　　？�
*　　　　　　　　　�А　　？А　�+　　　　　　　　　
*　　　　　　　　　�А� 　　�皐ォォォ� + +
*　　　　　　　　　�� 　　　　　　　�禰�
*　　　　　　　　　�� 　　　　　　　�芥�
*　　　　　　　　　�皐粂粂芥ォ廩粂芥� + + + +
*　　　　　　　　　　�З路蓮？З路�
*　　　　　　　　　　�皐潯拭？皐潯�+ + + +
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