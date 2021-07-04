#include "XMLParser.h"

XMLParser::XMLParser() {
	m_node = nullptr;
	m_parser = new XercesDOMParser();
	m_document = nullptr;
}

// ファイルから読み込み
XMLParser::XMLParser(std::string file_path)
{
	m_node = nullptr;
	m_parser = new XercesDOMParser();
	m_document = nullptr;
	loadXMLFile(file_path);
}

// DOMから読み込み
XMLParser::XMLParser(xercesc::DOMNode* src_node)
{
	m_node = nullptr;
	m_parser = nullptr;
	m_document = nullptr;
	loadXMLNode(src_node);
}

XMLParser::~XMLParser() {

}

// XML読み込み from ファイル
bool XMLParser::loadXMLFile(std::string file_path) {
	m_parser->parse(file_path.c_str());
	m_document = m_parser->getDocument();
	m_node = m_document->getFirstChild();

	return m_node != nullptr;
}

// XML読み込み from DOM
bool XMLParser::loadXMLNode(DOMNode* src_node)
{
	if (src_node == nullptr) return false;
	m_node = src_node;
	return true;
}

// XMLのような文字列から読み込み
// ex. "<Example><Msg msg=\"yay!\"/></Example>"
bool XMLParser::loadXMLString(std::string str_xml) {
	if (m_parser == nullptr) m_parser = new XercesDOMParser();
	xercesc::MemBufInputSource myxml_buf((const XMLByte*)str_xml.c_str(), str_xml.size(), "dummy");
	m_parser->parse(myxml_buf);
	m_document = m_parser->getDocument();
	m_node = m_document->getFirstChild();
	return m_node != nullptr;
}

// DOMNodeを文字列としてXML状に出力
std::string XMLParser::getAllXML() const {
	std::string output;
	DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("LS"));
	DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
	XMLFormatTarget* myFormTarget;
	myFormTarget = new MemBufFormatTarget();
	DOMLSOutput* outputStream = ((DOMImplementationLS*)impl)->createLSOutput();
	outputStream->setByteStream(myFormTarget);
	theSerializer->write(m_node, outputStream);
	output = (char*)((MemBufFormatTarget*)myFormTarget)->getRawBuffer();
	delete theSerializer;
	delete myFormTarget;
	return output;
}

// タグ名を取得
// ex. <Example name="louth" age="19"/> -> Example
std::string XMLParser::getTagName() const
{
	if (m_node == nullptr) return "";
	char* tag_name_tmp = XMLString::transcode(static_cast<DOMElement*>(m_node)->getTagName());
	std::string tagname = tag_name_tmp;
	XMLString::release(&tag_name_tmp);
	return tagname;
}

// 属性取得
// 直接使うことは少ないというより、非推奨
std::string XMLParser::getAttribute(std::string tag_name) const {
	if (m_node == nullptr) return "__not__found__attribute__";
	XMLStr tag_xmlstr(tag_name.c_str());
	DOMAttr* attr = static_cast<DOMElement*>(m_node)->getAttributeNode(tag_xmlstr);
	if (attr == nullptr) return "__not__found__attribute__";
	XMLStr attr_name(attr->getName());
	if (!strcmp(attr_name, tag_name.c_str())) {
		XMLStr attr_value(attr->getValue());
		std::string attr(attr_value);
		return attr;
	}
	return "__not__found__attribute__";
}

// Textノード取得
// ex. "<Example>yay</Example>" -> yay
std::string XMLParser::getText() const
{
	if (m_node == nullptr) return "";
	const XMLCh* buffer = m_node->getTextContent();
	return XMLString::transcode(buffer);
}

// 属性があるかを確認
// ex. <Example name="louth" age="19"/> hasAttribute("name") -> true
bool XMLParser::hasAttribute(std::string tagname) const
{
	return getAttribute(tagname) != "__not__found__attribute__";
}

// 属性をブール値で取得
// 取得した値が0以外の時 true
// 属性が取得できなかった時 false
bool XMLParser::attributeBool(std::string tagname) const
{
	std::string attr = getAttribute(tagname);
	if (attr == "__not__found__attribute__") { return false; }
	return attr != "0";
}

// 子ノードから特定のタグ名を絞り込みリストへ出力
// ex. "<Example><Msg msg=\"yay!\"/><Popup msg=\"yay!\"/></Example>" queryByTag(list, "Msg")
//     -> <Msg msg=\"yay!\"/>
// tagnameの初期値は"*"、省略すると全ての子ノードを取得する
bool XMLParser::queryByTag(std::list<XMLParser>& output, std::string tagname) const{
	DOMNodeList* node_list = static_cast<DOMElement*>(m_node)->getElementsByTagName(XMLString::transcode(tagname.c_str()));
	for (int i = 0; i < node_list->getLength(); i++) {
		DOMNode* as = node_list->item(i);
		output.push_back(XMLParser(as));
	}
	return output.size() > 0;
}

// 特定のタグ名の子ノードの最初にヒットしたノードを返す
// ex. "<Example><Msg msg=\"1!\"/><Msg msg=\"2!\"/></Example>" queryByTagOnce(list, "Msg")
//     -> <Msg msg=\"1!\"/>
bool XMLParser::queryByTagOnce(XMLParser& output, std::string tagname) const
{
	DOMNodeList* node_list = static_cast<DOMElement*>(m_node)->getElementsByTagName(XMLString::transcode(tagname.c_str()));
	if (node_list->getLength() == 0) {
		output.m_node = nullptr;
		output.m_parser = new XercesDOMParser();
		output.m_document = nullptr;
		return false;
	}
	if (this->m_document != nullptr) {
		output.m_document = (DOMDocument*)malloc(sizeof(this->m_document));
		memcpy(output.m_document, this->m_document, sizeof(this->m_document));
	}
	//output.m_parser = new XercesDOMParser();
	//output.m_document = this->m_document;
	output.m_node = static_cast<DOMElement*>(m_node)->getElementsByTagName(XMLString::transcode(tagname.c_str()))->item(0);
	return true;
}


// 属性を文字列として取得
// ex. <Example name="louth" age="19"/> attributeString("age") -> "19"
std::string XMLParser::attributeString(std::string tagname) const
{
	std::string attr = getAttribute(tagname);
	if (attr == "__not__found__attribute__") { return ""; }
	return attr;

}
// 属性を int として取得
// ex. <Example name="louth" age="19"/> attributeInt("age") -> 19
int XMLParser::attributeInt(std::string tagname) const
{
	std::string attr = getAttribute(tagname);
	if (attr == "__not__found__attribute__") { return 0; }
	return std::atoi(attr.c_str());
}

// 属性を long として取得
// ex. <Example name="louth" age="19"/> attributeLong("age") -> 19
long XMLParser::attributeLong(std::string tagname) const
{
	std::string attr = getAttribute(tagname);
	if (attr == "__not__found__attribute__") { return 0; }
	return std::strtol(attr.c_str(), NULL, 10);
}

// 属性を float として取得
// ex. <Example name="louth" age="19"/> attributeFloat("age") -> 19.0f
float XMLParser::attributeFloat(std::string tagname) const
{
	std::string attr = getAttribute(tagname);
	return std::strtof(attr.c_str(), NULL);
}

// 属性を double として取得
// ex. <Example name="louth" age="19"/> attributeDouble("age") -> 19.0
double XMLParser::attributeDouble(std::string tagname) const
{
	std::string attr = getAttribute(tagname);
	if (attr == "__not__found__attribute__") { return 0.0; }
	return std::strtod(attr.c_str(), NULL);
}

// 属性を 16進数 unsigned int として取得
// ex. <Example name="louth" age="19"/> attributeHex("age") -> 25
unsigned int XMLParser::attributeHex(std::string tagname) const
{
	std::string attr = getAttribute(tagname);
	if (attr == "__not__found__attribute__") { return 0; }
	try {
		return (unsigned int)std::stoi(attr, nullptr, 16);
	}
	catch (const std::invalid_argument&) {
		return 0;
	}
}

// Xslt変換を現在Parserが持つノードで行う
// paramは<xsl:param name="パラメーターキー"/>にバインドされる
// paramは無くてもいい
bool XMLParser::trans(std::string xsl_path, const std::map<std::string, std::string>& params) {
	MemBufInputSource node_xml((const XMLByte*)getAllXML().c_str(), getAllXML().length(), "memory_buffer");
	// 変換
	std::stringstream stream;
	XalanTransformer transformer;
	std::stringstream theXMLStream(getAllXML().c_str(), strlen(getAllXML().c_str()));
	XSLTInputSource inputSource(&theXMLStream);
	for (const auto& param : params) {
		std::string valstr = "'" + param.second + "'";
		transformer.setStylesheetParam(param.first.c_str(), valstr.c_str());
	}
	const XalanCompiledStylesheet* theCompiledStylesheet;
	transformer.compileStylesheet(xsl_path.c_str(), theCompiledStylesheet);
	transformer.transform(&theXMLStream, theCompiledStylesheet, stream);
	stream << '\0';
	// アウトプット
	std::string buffer = stream.str();
	MemBufInputSource source((const XMLByte*)buffer.c_str(), buffer.length(), "memory_buffer");
	m_parser->parse(source);
	m_document = m_parser->getDocument();
	m_node = m_document->getFirstChild();
	return true;
}
