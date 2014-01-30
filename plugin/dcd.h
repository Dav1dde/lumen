#ifndef LUMEN_DCD_H
#define LUMEN_DCD_H

#include <qobject.h>
#include <qmap.h>
#include <kprocess.h>
#include <kicon.h>

namespace DCDCompletionType { enum DCDCompletionType { Identifiers, Calltips }; };
namespace DCDCompletionItemType {
	enum DCDCompletionItemType {
		Invalid,

		Calltip,

		ClassName,
		InterfaceName,
		StructName,
		UnionName,
		VariableName,
		MemberVariableName,
		Keyword,
		FunctionName,
		EnumName,
		EnumMember,
		PackageName,
		ModuleName,
	};

	char toChar(DCDCompletionItemType e);
	DCDCompletionItemType fromChar(char c);
};

struct DCDCompletionItem {
	DCDCompletionItem(DCDCompletionItemType::DCDCompletionItemType, QString);

	DCDCompletionItemType::DCDCompletionItemType type;
	QString name;

	QIcon icon() const;
	QString typeLong() const;
};

struct DCDCompletion {
	DCDCompletionType::DCDCompletionType type;
	QList<DCDCompletionItem> completions;
};

class DCD
{
	public:
		DCD(int, QString, QString);
		virtual ~DCD();
		int port();
		bool running();
		bool startServer();
		bool stopServer();
		DCDCompletion complete(QString, int);
		DCDCompletion complete(QByteArray, int);
        QString doc(QByteArray, int);
		void shutdown();
		void addImportPath(QString);
		void addImportPath(QStringList);
	private:
		DCDCompletion processCompletion(QString);
		int m_port;
		QString m_server;
		QString m_client;
		KProcess m_sproc;
};

#endif