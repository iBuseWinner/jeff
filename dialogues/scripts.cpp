#include "scripts.h"

ScriptDialog::ScriptDialog(Basis *_basis, QWidget *parent = nullptr) 
  : QWidget(parent), basis(_basis), _m_handler(m_handler) {
  path_info.setText(tr("Specify script path:"));
  path_input.setText(tr("Select a file..."));
  stype_info.setText(tr("Specify script type:"));
  QStringList stypes = {
    tr("Startup script (suitable for startup prompts)"),
    tr("Daemon (works in background with Jeff's server)"),
    tr("Server (works in background and receives all messages)"),
    tr("Custom scaner (another answering system)"),
    tr("Custom composer (receives choosed variants and answers in another manner)")
  };
  stype_input.addItems(stypes);
}
