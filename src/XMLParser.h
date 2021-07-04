#pragma once
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

using namespace xercesc;
using namespace xalanc;

class XMLStr {
private:
	char* buffer_;

public:
	explicit XMLStr(const XMLCh* const buffer) {
		buffer_ = new char[strlen(XMLString::transcode(buffer)) + 1];
		buffer_ = XMLString::transcode(buffer);
	}
	explicit XMLStr(const char* buffer) {
		buffer_ = new char[strlen(buffer) + 1];
		strcpy_s(buffer_, strlen(buffer) + 1, buffer);
	}
	~XMLStr() {
		xercesc::XMLString::release(&buffer_);
	}
	operator const char* const () const {
		return buffer_;
	}
	operator const XMLCh* () {
		return XMLString::transcode(buffer_);
	}
};

class XMLParser
{
public:
	XMLParser();
	XMLParser(std::string file_path);
	XMLParser(xercesc::DOMNode* src_node);
	~XMLParser();
	xercesc::XercesDOMParser* m_parser;
	xercesc::DOMDocument* m_document;
	xercesc::DOMNode* m_node;

	bool loadXMLFile(std::string file_path);
	bool loadXMLNode(xercesc::DOMNode* src_node);
	bool loadXMLString(std::string file_path);

	std::string getAllXML() const;
	std::string getTagName() const;;
	std::string getText() const;;
	std::string getAttribute(std::string tag_name) const;
	bool empty() const{ return m_node == NULL; };

	bool hasAttribute(std::string tagname) const;

	int attributeInt(std::string tagname) const;
	long attributeLong(std::string tagname) const;
	float attributeFloat(std::string tagname) const;
	double attributeDouble(std::string tagname) const;
	std::string attributeString(std::string tagname) const;
	unsigned int attributeHex(std::string tagname) const;
	bool attributeBool(std::string tagname) const;

	bool queryByTag(std::list<XMLParser>& output, std::string tagname="*") const;
	bool queryByTagOnce(XMLParser& output, std::string tagname) const;

	bool trans(std::string xsl_path, const std::map<std::string, std::string>& params = std::map<std::string, std::string>());
};

typedef std::list<XMLParser> XMLList;