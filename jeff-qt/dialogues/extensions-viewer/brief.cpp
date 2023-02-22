#include "brief.hpp"

/*! @brief The constructor. */
ExtensionsViewerBrief::ExtensionsViewerBrief(ExtensionsManager *_em, QWidget *parent) : QWidget(parent), em(_em) {
  connect(&back_btn, &Button::clicked, this, [this] { extension_meta = nullptr; emit close_brief(); });
  connect(&remove_btn, &Button::clicked, this, &ExtensionsViewerBrief::remove_extension);
  connect(&print_stdout_btn, &Button::clicked, this, &ExtensionsViewerBrief::get_ext_stdout);
  connect(&print_stderr_btn, &Button::clicked, this, &ExtensionsViewerBrief::get_ext_stderr);
  remove_btn.setText(tr("Remove this extension from Jeff"));
  print_stdout_btn.setText(tr("Print stdout"));
  print_stderr_btn.setText(tr("Print stderr"));
  back_btn.setIcon(QIcon::fromTheme("go-previous", QIcon(":/arts/icons/16/go-previous.svg")));
  remove_btn.setIcon(QIcon::fromTheme("edit-delete", QIcon(":/arts/icons/16/edit-delete.svg")));
  desc_lbl.setWordWrap(true);
  authors_lbl.setTextInteractionFlags(Qt::TextSelectableByMouse);
  authors_lbl.setContextMenuPolicy(Qt::CustomContextMenu);
  auto *copy_text_action = new QAction(
    QIcon::fromTheme("edit-copy", QIcon(":/arts/icons/16/copy.svg")), tr("Copy selected text"), this
  );
  connect(copy_text_action, &QAction::triggered, this, [this] {
    auto *clipboard = QGuiApplication::clipboard();
    clipboard->setText(authors_lbl.selectedText());
  });
  auto *context_menu = new Menu(this);
  context_menu->addAction(copy_text_action);
  connect(&authors_lbl, &QLabel::customContextMenuRequested, this, [this, context_menu] {
    context_menu->exec(QCursor::pos());
  });
  links_lbl.setContextMenuPolicy(Qt::NoContextMenu);
  status_lbl.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  setLayout(
    VLineLt::another()
      ->addlt(HLineLt::another()->addw(&back_btn)->addw(&name_lbl)->addi(spacer))
      ->addw(&appeal_lbl)->addw(&desc_lbl)->addw(&authors_lbl)->addw(&license_lbl)
      ->addw(&links_lbl)->addlt(HLineLt::another()->addw(&status_lbl)->addw(&on_off_btn))
      ->addw(&remove_btn)->addlt(HLineLt::another()->addw(&print_stdout_btn)->addw(&print_stderr_btn))
  );
}

/*! @brief Sets up extension details. */
void ExtensionsViewerBrief::setup(ExtensionMeta *_extension_meta) {
  extension_meta = _extension_meta;
  name_lbl.setText("<h1>" + extension_meta->title + "</h1>");
  appeal_lbl.setText("<b>" + tr("Short name:") + "</b> " + extension_meta->name);
  desc_lbl.setText("<b>" + tr("Description:") + "</b> " + extension_meta->desc);
  if (extension_meta->authors.keys().isEmpty()) {
    if (authors_lbl.isVisible()) authors_lbl.setVisible(false);
  } else {
    if (not authors_lbl.isVisible()) authors_lbl.setVisible(true);
    QString authors = "<b>" + tr("Authors:") + "</b>";
    for (auto author : extension_meta->authors.keys()) {
      authors += "<br>· <i>" + author + "</i>";
      if (not extension_meta->authors[author].keys().isEmpty())
        for (auto contact_type : extension_meta->authors[author].keys()) {
          auto contact_content = extension_meta->authors[author][contact_type];
          contact_type[0] = contact_type[0].toUpper();
          authors += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- " + contact_type + ": " + contact_content;
        }
    }
    authors_lbl.setText(authors);
  }
  license_lbl.setText("<b>" + tr("License:") + "</b> " + extension_meta->license);
  if (extension_meta->links.isEmpty()) {
    if (links_lbl.isVisible()) links_lbl.setVisible(false);
  } else {
    if (not links_lbl.isVisible()) links_lbl.setVisible(true);
    QString links = "<b>" + tr("Links:") + "</b>";
    for (auto link : extension_meta->links) links += "<br><a href=\"" + link + "\">" + link + "</a>";
    links_lbl.setText(links);
  }
  status_lbl.setText("<i>" + tr("Waiting status...") + "</i>");
  on_off_btn.setText(tr("Control"));
  on_off_btn.setEnabled(false);
  print_stdout_btn.setEnabled(false);
  print_stderr_btn.setEnabled(false);
  update_status();
}

/*! @brief Displays every second whether an extension is running or not. */
void ExtensionsViewerBrief::update_status() {
  if (not extension_meta) return;
  print_stdout_btn.setEnabled(true);
  print_stderr_btn.setEnabled(true);
  if (isVisible()) {
    if (not on_off_btn.isEnabled()) {
      on_off_btn.setEnabled(true);
    }
    if (em->is_running(extension_meta)) {
      disconnect(&on_off_btn, &Button::clicked, nullptr, nullptr);
      on_off_btn.setText(tr("Disable the extension"));
      on_off_btn.setIcon(QIcon::fromTheme("media-playback-stop", QIcon(":/arts/icons/16/media-playback-stop.svg")));
      connect(&on_off_btn, &Button::clicked, this, [this] {
        em->stop_extension(extension_meta);
        extension_meta->enabled = false;
      });
      status_lbl.setText("<p style=\"color:#3b961a\">" + tr("Running") + "</p>");
    }
    else {
      disconnect(&on_off_btn, &Button::clicked, nullptr, nullptr);
      on_off_btn.setText(tr("Enable the extension"));
      on_off_btn.setIcon(QIcon::fromTheme("media-playback-start", QIcon(":/arts/icons/16/media-playback-start.svg")));
      connect(&on_off_btn, &Button::clicked, this, [this] {
        em->start_extension(extension_meta);
        extension_meta->enabled = true;
      });
      status_lbl.setText("<p style=\"color:#d22a2a\">" + tr("Stopped") + "</p>");
    }
  }
  QTimer::singleShot(1000, this, &ExtensionsViewerBrief::update_status);
}

/*! @brief Removes an extension from Jeff. */
void ExtensionsViewerBrief::remove_extension() {
  em->remove_extension(extension_meta);
  emit close_brief();
}

/*! @brief TBD */
void ExtensionsViewerBrief::get_ext_stdout() {
  auto bytes = em->get_stdout(extension_meta);
  if (bytes.isEmpty()) return;
  emit show_info(tr("Stdout data from @") + extension_meta->name + ": `" + QString::fromUtf8(bytes) + "`");
}

/*! @brief TBD */
void ExtensionsViewerBrief::get_ext_stderr() {
  auto bytes = em->get_stderr(extension_meta);
  if (bytes.isEmpty()) return;
  emit show_info(tr("Stderr data from @") + extension_meta->name + ": `" + QString::fromUtf8(bytes) + "`");
}
