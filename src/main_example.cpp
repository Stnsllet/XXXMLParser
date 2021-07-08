#include <iostream>
#include <regex>
#include <map>
#include "XMLParser.h"


int main()
{
    XMLPlatformUtils::Initialize();
    XalanTransformer::initialize();
    XSLTranser* xslt = new XSLTranser();
    // example.1 normal xml read
    //XMLParser song;
    //song.loadXMLFile("xml/example1.xml");
    //std::cout << song.attributeString("title") << "\n";
    //XMLList lyrics;
    //song.queryByTag(lyrics, "Msg");
    //for (auto& lyric : lyrics) {
    //    std::cout << lyric.attributeString("name") << ":" << lyric.getText() << "\n";
    //}
    //
    //// example.2 read from text
    //XMLParser greet;
    //greet.loadXMLString("<Greet>Houdy!!</Greet>");
    //std::cout << greet.getTagName() << "<" << greet.getText() << "\n";
    //std::string raw_greet = greet.getAllXML();
    //std::cout << "ToString:" << raw_greet << "\n";

    ////example.3 read cutted xmlstring;
    //XMLParser system;
    //system.loadXMLFile("xml/example2.xml");
    //XMLParser battle;
    //system.queryByTagOnce(battle, "Battle");
    //XMLParser new_battle;
    //std::string battle_string = battle.getAllXML();
    //battle_string = std::regex_replace(battle.getAllXML(), std::regex("Money"), "Gold");
    //new_battle.loadXMLString(battle_string);
    //XMLList battle_nodes;
    //new_battle.queryByTag(battle_nodes);
    //for (auto& node : battle_nodes) {
    //    std::cout << node.getTagName() << "\n";
    //}
    //example.4 XSLT with params;
    XMLParser calltrans;
    calltrans.loadXMLFile("xml/example3.xml");
    std::string xsl_filepath = calltrans.attributeString("path");
    std::map<std::string, std::string> xslt_param;
    xslt_param["_temp_wep"] = "100/200/0.5";
    xslt->trans(calltrans, xsl_filepath, xslt_param);
    std::cout << "\n" << calltrans.getAllXML() << "\n";

    /* Output
        Hell World!
        lyra:Step 1, Try not to be too self-conscious.
        lyra:2, Shift your weight into your haunches.
        lyra:3, Give a leap into the air and
        lyra:4, Just forget your parents are both dead!
        Greet<Houdy!!
        ToString:<Greet>Houdy!!</Greet>
        Gold
        Gold
        Gold
        Range
        BraveRate

        <Event>
                <Paty army="friend" dir="0.8" x="1267" y="909">
        <Member job="swdman" pason="ruuva" wep="Excalibur"/>
        </Paty>
        </Event>
    */

    XalanTransformer::terminate();
    XMLPlatformUtils::Terminate();
    return 0;
}
