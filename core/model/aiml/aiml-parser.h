#ifndef AIML_PARSER_H
#define AIML_PARSER_H

#include "core/basis.h"
#include "core/model/nlp/cache.h"
#include "core/model/nlp/stringssearch.h"
#include <QXmlStreamReader>

/*!
 * @class AimlParser
 * @brief TBD
 */
class AimlParser : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(AimlParser)
public:
  // Functions described in `aiml-parser.cpp`:
  AimlParser(Basis *_basis, QObject *parent = nullptr);
  CacheWithIndices select_from_aiml(const QString &input);
  QString evaluate(const QString &aiml_expr);
  
signals:
  /*! @brief TBD */
  QString aiml_parse_error(QString error);
  
private:
  // Objects:
  Basis *basis = nullptr;
  
  // Functions described in `aiml-parser.cpp`:
  Cache extract_expressions_from_category(QXmlStreamReader &xml, const QString &input);
};

#endif
