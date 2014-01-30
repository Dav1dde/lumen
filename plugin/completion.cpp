#include "completion.h"
#include <ktexteditor/view.h>
#include <ktexteditor/document.h>
#include <ktexteditor/codecompletioninterface.h>
#include <ktexteditor/codecompletionmodelcontrollerinterface.h>
#include <klocalizedstring.h>


LumenCompletionModel::LumenCompletionModel(QObject* parent, DCD* dcd): CodeCompletionModel2(parent)
{
	m_dcd = dcd;
}

LumenCompletionModel::~LumenCompletionModel()
{

}

// QModelIndex LumenCompletionModel::parent(const QModelIndex& index) const
// {
//     if (index.internalId()) {
//         return createIndex(0, 0, 0);
//     } else {
//         return QModelIndex();
//     }
// }
//
// int LumenCompletionModel::rowCount(const QModelIndex& parent) const
// {
//     if (!parent.isValid() && !m_data.completions.isEmpty()) {
//         return 1; //one toplevel node (group header)
//     } else if(parent.parent().isValid()) {
//         return 0; //we don't have sub children
//     } else {
//         return m_data.completions.length(); // only the children
//     }
// }
//
// QModelIndex LumenCompletionModel::index(int row, int column, const QModelIndex& parent) const
// {
//     if (!parent.isValid()) {
//         if (row == 0) {
//             return createIndex(row, column, 0); //header  index
//         } else {
//             return QModelIndex();
//         }
//     } else if (parent.parent().isValid()) { //we only have header and children, no subheaders
//         return QModelIndex();
//     }
//
//     if (row < 0 || row >= m_data.completions.length() || column < 0 || column >= ColumnCount ) {
//         return QModelIndex();
//     }
//
//     return createIndex(row, column, 1); // normal item index
// }


bool LumenCompletionModel::shouldStartCompletion(View* view, const QString& insertedText, bool userInsertion, const Cursor& position)
{
	bool complete = KTextEditor::CodeCompletionModelControllerInterface3::shouldStartCompletion(
		view, insertedText, userInsertion, position
	);

	complete = complete || insertedText.endsWith("("); // calltip
	complete = complete || insertedText.endsWith("import "); // import

	return complete;
}

void LumenCompletionModel::completionInvoked(View* view, const Range& range, CodeCompletionModel::InvocationType invocationType)
{
	Q_UNUSED(invocationType);
	KTextEditor::Document* document = view->document();

	KTextEditor::Cursor cursor = range.end();
	KTextEditor::Cursor cursorEnd = document->documentEnd();
	KTextEditor::Range range0c = KTextEditor::Range(0, 0, cursor.line(), cursor.column());
	KTextEditor::Range rangece = KTextEditor::Range(cursor.line(), cursor.column(),
	                                                cursorEnd.line(), cursorEnd.column());
	QString text0c = document->text(range0c, false);
	QByteArray utf8 = text0c.toUtf8();
	int offset = utf8.length();
	utf8.append(document->text(rangece, false).toUtf8());

	m_data = m_dcd->complete(utf8, offset);
	setRowCount(m_data.completions.length());

	setHasGroups(false);
}

void LumenCompletionModel::executeCompletionItem2(Document* document, const Range& word, const QModelIndex& index) const
{
	QModelIndex sibling = index.sibling(index.row(), Name);
	KTextEditor::View* view = document->activeView();

	document->replaceText(word, data(sibling).toString());

	int crole = data(sibling, CompletionRole).toInt();
	if(crole & Function) {
		KTextEditor::Cursor cursor = document->activeView()->cursorPosition();
		document->insertText(cursor, QString("()"));
		view->setCursorPosition(Cursor(cursor.line(), cursor.column()+1));
	}
}

QVariant LumenCompletionModel::data(const QModelIndex& index, int role) const
{
//     if (!index.parent().isValid()) {
// 		switch(role) {
// 			case Qt::DisplayRole: return i18n("Lumen");
// 			case GroupRole: return Qt::DisplayRole;
// 			case InheritanceDepth: return 50;
// 		}
// 		return QVariant();
//     }

    DCDCompletionItem item = m_data.completions[index.row()];

	switch(role)
	{
		case Qt::DecorationRole:
		{
			if(index.column() == Icon) {
				return item.icon();
			}
			break;
		}
		case Qt::DisplayRole:
		{
			if(item.type == DCDCompletionItemType::Calltip) {
				QRegExp funcRE("^\\s*(\\w+)\\s+(\\w+\\s*\\(.*\\))\\s*$");
				funcRE.indexIn(item.name);
				QStringList matches = funcRE.capturedTexts();

				switch(index.column()) {
					case Prefix: return matches[1];
					case Name: return matches[2];
// 					case Arguments: return matches[3];
// 					case Postfix: return QVariant();
				}
			} else {
				if(index.column() == Name) {
					return item.name;
				}
			}
			break;
		}
		case CompletionRole:
		{
			int p = NoProperty;
			switch(item.type) {
				case DCDCompletionItemType::FunctionName: p |= Function; break;
				case DCDCompletionItemType::VariableName: p |= Variable; break;
				default: break;
			}
			return p;
		}
		case BestMatchesCount:
		{
			return 5;
		}
		case ArgumentHintDepth:
		{
			if(item.type == DCDCompletionItemType::Calltip) {
				return 1;
			}
			break;
		}
		case GroupRole:
		{
			break;
		}
		case IsExpandable:
		{
			// I like the green arrow
			return true;
		}
		case ExpandingWidget:
		{
			// TODO well implementation in DCD is missing
			return QVariant();
		}
	}

	return QVariant();
}

#include "completion.moc"