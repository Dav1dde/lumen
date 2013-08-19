#include "lumen.h"

#include <kpluginfactory.h>
#include <kaboutdata.h>
#include <ktexteditor/document.h>
#include <ktexteditor/range.h>
#include <ktexteditor/view.h>
#include <ktexteditor/codecompletioninterface.h>
#include <ktexteditor/codecompletionmodel.h>
#include <kactioncollection.h>
#include <qfile.h>

K_PLUGIN_FACTORY_DEFINITION(
    LumenPluginFactory, registerPlugin<LumenPlugin>("lumen");
)
K_EXPORT_PLUGIN(
    LumenPluginFactory(
        KAboutData(
			"lumen",
			"lumen",
			ki18n("lumen2"),
			"0.1",
			ki18n("D Autocompletion"),
			KAboutData::License_Custom,
			ki18n("© David Herberth"),
			ki18n("D Autocompletion plugin using DCD as completion server."),
			"https://github.com/Dav1dde/lumen",
			"submit-the-bug@github.now"
		)
    )
)

LumenPluginView::LumenPluginView(LumenPlugin *plugin, KTextEditor::View *view): QObject(plugin),KXMLGUIClient(view),m_view(view),m_registered(false)
{
	m_plugin = plugin;
	m_model = new LumenCompletionModel((QObject*)m_view, m_plugin->dcd());

	KTextEditor::Document* document = view->document();

	connect(document, SIGNAL(documentUrlChanged(KTextEditor::Document*)),
			this, SLOT(urlChanged(KTextEditor::Document*)));

	registerCompletion();
}

void LumenPluginView::registerCompletion()
{
	KTextEditor::CodeCompletionInterface *completion =
	    qobject_cast<KTextEditor::CodeCompletionInterface*>(m_view);

	bool isD = m_view->document()->url().path().endsWith(".d") ||
	           m_view->document()->highlightingMode() == "D";

	if(isD && !m_registered) {
		completion->registerCompletionModel(m_model);
		m_registered = true;
	} else if(!isD && m_registered) {
		completion->unregisterCompletionModel(m_model);
		m_registered = false;
	}
}


LumenPluginView::~LumenPluginView()
{
}

void LumenPluginView::urlChanged(Document* document)
{
	registerCompletion();

	for(KUrl url = document->url(); !url.equals(KUrl("/")); url.cd("..")) {
		url = url.directory();
		url.addPath(".kdev_include_paths");

		QFile file(url.path());
		if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			while(!file.atEnd()) {
				m_plugin->dcd()->addImportPath(file.readLine());
		    }
		}
    }
}


LumenPlugin::LumenPlugin(QObject *parent, const QVariantList &): Plugin(parent)
{
	m_dcd = new DCD(9977, "dcd-server", "dcd-client");
	m_dcd->startServer();

	m_dcd->addImportPath("/usr/include/d");
	m_dcd->addImportPath("/usr/include/d/druntime/import");
}

LumenPlugin::~LumenPlugin()
{
	m_dcd->stopServer();
	delete m_dcd;
}

DCD* LumenPlugin::dcd()
{
	return m_dcd;
}


void LumenPlugin::addView(KTextEditor::View *view)
{
	m_views.insert(view, new LumenPluginView(this, view));
}

void LumenPlugin::removeView(KTextEditor::View *view)
{
	delete m_views.take(view);
}

#include "lumen.moc"
