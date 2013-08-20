#ifndef LUMEN_H
#define LUMEN_H

#include <ktexteditor/plugin.h>
#include <ktexteditor/range.h>

#include <kpluginfactory.h>

#include <QVariantList>

#include <kaction.h>
#include <kxmlguiclient.h>
#include "dcdprocess.h"
#include "completion.h"

using namespace KTextEditor;

class LumenPlugin;

class LumenPluginView: public QObject, public KXMLGUIClient
{
	Q_OBJECT
	public:
		LumenPluginView(LumenPlugin *plugin, KTextEditor::View *view);
		virtual ~LumenPluginView();
		void registerCompletion();
	private slots:
		void urlChanged(KTextEditor::Document*);
	private:
		LumenPlugin *m_plugin;
		QPointer<KTextEditor::View> m_view;
		LumenCompletionModel *m_model;
		bool m_registered;
};

class LumenPlugin: public Plugin
{
	Q_OBJECT
	public:
		LumenPlugin(QObject *parent, const QVariantList & = QVariantList());
		~LumenPlugin();
		DCDProcess* dcd();
		void addView(View *view);
		void removeView(View *view);
		virtual void readConfig(KConfig*) {}
		virtual void writeConfig(KConfig*) {}
	private:
		QMap<KTextEditor::View*,LumenPluginView*> m_views;
		DCDProcess* m_dcd;
};

K_PLUGIN_FACTORY_DECLARATION(LumenPluginFactory)

#endif
