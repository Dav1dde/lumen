#ifndef LUMEN_COMPLETION_H
#define LUMEN_COMPLETION_H
#include <ktexteditor/codecompletionmodel.h>
#include <ktexteditor/codecompletionmodelcontrollerinterface.h>
#include "dcdprocess.h"

using namespace KTextEditor;

class LumenCompletionModel: public CodeCompletionModel2,
	public KTextEditor::CodeCompletionModelControllerInterface3
{
	Q_OBJECT
	Q_INTERFACES(KTextEditor::CodeCompletionModelControllerInterface3)
	public:
		LumenCompletionModel(QObject* parent, DCDProcess* dcd);
		virtual ~LumenCompletionModel();

// 		virtual QModelIndex parent(const QModelIndex& index) const;
// 	   	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
// 		virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

		virtual bool shouldStartCompletion(View* view, const QString& insertedText, bool userInsertion, const Cursor& position);
		virtual void completionInvoked(View* view, const Range& range, InvocationType invocationType);
		virtual void executeCompletionItem2(Document* document, const Range& word, const QModelIndex& index) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	private:
		DCDProcess* m_dcd;
		DCDCompletion m_data;
};



#endif