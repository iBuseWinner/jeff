#include "basis.h"

/*!
 * @fn Basis::check_settings_file
 * @brief Checks the settings file for any errors.
 */
void Basis::check_settings_file() {
  if (not accessible())
    emit settings_warning(
      tr("An access error occurred (e.g. trying to write to a read-only file).")
    );
  else if (not correct())
    emit settings_warning(tr("A format error occurred (e.g. loading a malformed file)."));
}

/*!
 * @fn Basis::check_default_source
 * @brief Checks if there is a standard source, and otherwise sets the first source in the list 
 * as standard.
 */
void Basis::check_default_source() {
  QVariant path = read(defaultSourcePath);
  QVariant container = read(defaultSourceContainer);
  if (not path.isValid() or not container.isValid())
    set_first_source_as_default();
  QString p = path.toString();
  QString c = container.toString();
  Source s;
  s.path = p;
  s.table_name = c;
  if (not get_sources().contains(s))
    set_first_source_as_default();
}

/*!
 * @fn Basis::set_first_source_as_default
 * @sa Basis::check_default_source
 */
void Basis::set_first_source_as_default() {
  sources_mutex.lock();
  if (not sources.isEmpty()) {
    write(defaultSourcePath, sources[0].path);
    write(defaultSourceContainer, sources[0].table_name);
  } else {
    write(defaultSourcePath, "");
    write(defaultSourceContainer, "");
  }
  sources_mutex.unlock();
}

/*!
 * @fn Basis::write
 * @brief Sets the value of the parameter.
 * @param[in] key parameter name
 * @param[in] data parameter value
 */
void Basis::write(const QString &key, const QVariant &data) {
  if (not correct()) {
    /*! If the file is incorrectly formatted, Basis will not be able to restore
     *  the data structure, so it clears the file.  */
    _settings.clear();
    _settings.sync();
  }
  _settings.setValue(key, data);
}
