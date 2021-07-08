#include "XMLParser.h"

#include "String.h"

std::string toString(XObjectPtr xobject) {
    XalanDOMString::CharVectorType vArg = TranscodeToLocalCodePage(xobject->str());
    std::string sArg(vArg.begin(), vArg.end());
    return sArg;
}

class FunctionFlutter : public Function
{
public:
    virtual XObjectPtr execute( XPathExecutionContext& executionContext, XalanNode* context, const XObjectArgVectorType& args, const Locator* locator) const
    {
        const char* theTimeString = "yay!";
        assert(theTimeString != 0);
        return executionContext.getXObjectFactory().createString(XalanDOMString(theTimeString));
    }
    using Function::execute;
    virtual FunctionFlutter* clone(MemoryManager& theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }
protected:
    const XalanDOMString& getError(XalanDOMString& theResult) const
    {
        theResult.assign("The asctime() function accepts one argument!");
        return theResult;
    }
};

class FunctionShiftPos : public Function
{
public:
    virtual XObjectPtr execute(XPathExecutionContext& executionContext, XalanNode* context, const XObjectArgVectorType& args, const Locator* locator) const
    {
        if (args.size() != 1)
        {
            return executionContext.getXObjectFactory().createString(XalanDOMString("$Only 3 args need."));
        }
        //XalanDOMString::CharVectorType vArg = TranscodeToLocalCodePage(args[0]->str());
        //std::string sArg(vArg.begin(), vArg.end());
        String strs = toString(args[0]);
        StringVector sv = strs.split('/');
        const char* delim = ",";

        std::ostringstream os;
        std::copy(sv.begin(), sv.end(), std::ostream_iterator<std::string>(os, delim));
        std::string s = os.str(); // "a,b,c,"
        s.erase(s.size() - std::char_traits<char>::length(delim));
        return executionContext.getXObjectFactory().createString(XalanDOMString(s.c_str()));
    }
    using Function::execute;
    virtual FunctionShiftPos* clone(MemoryManager& theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }
protected:
    const XalanDOMString& getError(XalanDOMString& theResult) const
    {
        theResult.assign("The asctime() function accepts one argument!");
        return theResult;
    }
};

XSLTranser::XSLTranser() {
    const XalanDOMString theNamespace("http://ExternalFunction.xalan-c++.xml.apache.org");
    transformer.installExternalFunction(theNamespace, XalanDOMString("yay"), FunctionFlutter());
    transformer.installExternalFunction(theNamespace, XalanDOMString("shiftpos"), FunctionShiftPos());
}

bool XSLTranser::trans(XMLParser& xml, std::string xsl_path, const std::map<std::string, std::string>& params) {
    MemBufInputSource node_xml((const XMLByte*)xml.getAllXML().c_str(), xml.getAllXML().length(), "memory_buffer");
    std::stringstream stream;
    std::stringstream theXMLStream(xml.getAllXML().c_str(), strlen(xml.getAllXML().c_str()));
    if (theXMLStream.str() == "") {
        theXMLStream = std::stringstream(xml.getAllXML().c_str(), strlen(xml.getAllXML().c_str()) + 1);
    }
    XSLTInputSource inputSource(&theXMLStream);
    for (const auto& param : params) {
        std::string valstr = "'" + param.second + "'";
        transformer.setStylesheetParam(param.first.c_str(), valstr.c_str());
    }
    const XalanCompiledStylesheet* theCompiledStylesheet;
    transformer.compileStylesheet(xsl_path.c_str(), theCompiledStylesheet);

    int success = transformer.transform(&theXMLStream, theCompiledStylesheet, stream);
    stream << '\0';
    std::string buffer = stream.str();
    MemBufInputSource source((const XMLByte*)buffer.c_str(), buffer.length(), "memory_buffer");
    xml.m_parser->parse(source);
    xml.m_document = xml.m_parser->getDocument();
    xml.m_node = xml.m_document->getFirstChild();
    return true;
}
