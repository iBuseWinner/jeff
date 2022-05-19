#include "spoiler.h"

/*! @brief The constructor. */
Spoiler::Spoiler(const QString &title, const int _animation_duration, QWidget *parent)
    : QWidget(parent), animation_duration(_animation_duration) {
  toggle_button.setStyleSheet("QToolButton { border: none; }");
  toggle_button.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  toggle_button.setArrowType(Qt::ArrowType::RightArrow);
  toggle_button.setText(title);
  toggle_button.setCheckable(true);
  toggle_button.setChecked(false);
  header_line.setFrameShape(QFrame::HLine);
  header_line.setFrameShadow(QFrame::Sunken);
  header_line.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  content_area.setStyleSheet("QScrollArea { border: none; }");
  content_area.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  content_area.setMaximumHeight(0);
  content_area.setMinimumHeight(0);
  toggle_animation.addAnimation(new QPropertyAnimation(this, "minimumHeight"));
  toggle_animation.addAnimation(new QPropertyAnimation(this, "maximumHeight"));
  toggle_animation.addAnimation(new QPropertyAnimation(&content_area, "maximumHeight"));
  main_layout.setVerticalSpacing(0);
  main_layout.setContentsMargins(0, 0, 0, 0);
  int row = 0;
  main_layout.addWidget(&toggle_button, row, 0, 1, 1, Qt::AlignLeft);
  main_layout.addWidget(&header_line, row++, 2, 1, 1);
  main_layout.addWidget(&content_area, row, 0, 1, 3);
  setLayout(&main_layout);
  connect(&toggle_button, &QToolButton::clicked, [this](const bool checked) {
    toggle_button.setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
    toggle_animation.setDirection(
      checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward
    );
    toggle_animation.start();
  });
}

/*! @brief Sets the correct values for animations to work. */
void Spoiler::set_content_layout(QLayout *content_layout) {
  if (content_area.layout()) delete content_area.layout();
  content_area.setLayout(content_layout);
  const auto collapsed_height = sizeHint().height() - content_area.maximumHeight();
  auto content_height = content_layout->sizeHint().height();
  for (int i = 0; i < toggle_animation.animationCount() - 1; ++i) {
    auto *spoiler_animation = static_cast<QPropertyAnimation *>(toggle_animation.animationAt(i));
    spoiler_animation->setDuration(animation_duration);
    spoiler_animation->setStartValue(collapsed_height);
    spoiler_animation->setEndValue(collapsed_height + content_height);
  }
  auto *content_animation = static_cast<QPropertyAnimation *>(
    toggle_animation.animationAt(toggle_animation.animationCount() - 1)
  );
  content_animation->setDuration(animation_duration);
  content_animation->setStartValue(0);
  content_animation->setEndValue(content_height);
}

/*! @brief TBD */
void Spoiler::set_title(QString title) { toggle_button.setText(title); }
