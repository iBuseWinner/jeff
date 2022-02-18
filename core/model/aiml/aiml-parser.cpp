#include "aiml-parser.h"

/*! @brief The constructor. */
AimlParser::AimlParser(Basis *_basis, QObject *parent) : QObject(parent), basis(_basis) {}

/*! @brief TBD */
CacheWithIndices AimlParser::select_from_aiml(const QString &input) {
  QStringList aiml_files; /*! = basis->json->read_aiml_list(); */
  CacheWithIndices selection;
  for (auto filepath : aiml_files) {
    QFile file(filepath);
    if (not file.open(QIODevice::ReadOnly | QIODevice::Text)) continue;
    QXmlStreamReader xml(&file);
    while (not xml.atEnd() and not xml.hasError()) {
      auto token = xml.readNext();
      if (token == QXmlStreamReader::StartDocument) continue;
      if (token == QXmlStreamReader::StartElement) {
        if (xml.name() == "category") {
          auto expressions = extract_expressions_from_category(xml, input);
          for (auto expression : expressions) {
            auto x = StringSearch::contains(input, expression.activator_text, 0.66, true);
            if (x[0] != 0) {
              for (auto ewi : selection) if (ewi.second == expression) continue;
              if (selection.keys().length() == 0) selection[0] = ExpressionWithIndices(x, expression);
              else selection[selection.lastKey() + 1] = ExpressionWithIndices(x, expression);
            }
          }
        }
      }
    }
    if (xml.hasError()) emit aiml_parse_error(xml.errorString());
    xml.clear();
    file.close();
  }
  return selection;
}

/*! @brief TBD */
QString AimlParser::evaluate(const QString &aiml_expr) {
  return QString();
}

/*! @brief TBD */
Cache AimlParser::extract_expressions_from_category(QXmlStreamReader &xml, const QString &input) {
  Cache cache;
  if (xml.tokenType() != QXmlStreamReader::StartElement and xml.name() == "category") return cache;
  xml.readNext();
  if (xml.tokenType() == QXmlStreamReader::StartElement and xml.name() != "pattern") return cache;
  auto expr_pattern = xml.text().toString();
  xml.readNext();
  if (xml.tokenType() != QXmlStreamReader::EndElement and xml.name() == "pattern") return cache;
  xml.readNext();
  if (xml.tokenType() != QXmlStreamReader::StartElement or xml.name() != "template") return cache;
  auto expr_template = xml.text().toString();
  Expression expression;
  expression.aiml = true;
  expression.activator_text = expr_pattern;
  expression.reagent_text = expr_template;
  cache.append(expression);
//   QXmlStreamAttributes attributes = xml.attributes();
//   if(attributes.hasAttribute("id")) person["id"] = attributes.value("id").toString();
//   if(xml.tokenType() != QXmlStreamReader::Characters) return;
  return cache;
}
