#include "basis.h"

/*!
 * @fn Basis::check_settings_file
 * @brief Checks the settings file for any errors.
 */
void Basis::check_settings_file() {
  if (not accessible())
    emit settings_warning(
        tr("An access error occurred (e.g. trying to write to a read-only "
           "file)."));
  else if (not correct())
    emit settings_warning(
        tr("A format error occurred (e.g. loading a malformed file)."));
}

/*!
 * @fn Basis::write
 * @brief Sets the value of the parameter.
 * @param[in] key parameter name
 * @param[in] data parameter value
 */
void Basis::write(const QString &key, const QVariant &data) {
  if (not correct()) {
    /*!
     * If the file is incorrectly formatted, Basis will not be able to restore
     * the data structure, so it clears the file.
     */
    _settings->clear();
    _settings->sync();
  }
  _settings->setValue(key, data);
}
