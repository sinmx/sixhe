#ifndef		TINYXML_XML_PARSER_HDR
#define		TINYXML_XML_PARSER_HDR

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#include "tinyxml\tinyxml.h"

#include <string>

class XML_PARSER
{

public:
	// -- Constructor --
	//
	XML_PARSER();

	// -- Destructor --
	//
	 ~XML_PARSER();

	// -- Load // Save  An XML document --
	//
	bool Load_XML_Document(const char *strFileName);

	// Load an XML Document from file
	bool Load_XML_String(const char * strXml);

	// Save the Current XML Document into a file
	bool Save_XML_Document(const char * strFileName);			

	bool Get_XML(std::string &xml);

	// -- *********************************** --
	// -- *** XML Document Moving by name *** --
	// -- *********************************** --
	void Go_to_Root();												// Go to the Root node
	bool Go_to_Child(const char * NodeName);								// Go to a Direct Child Node
	bool Go_to_Parent(const char * NodeName);							// Go to a Parent Node
	bool Go_to_NextSibling(const char * NodeName);						// Go to a sibling Node (Forward sens)
	bool Go_to_PrevSibling(const char * NodeName);						// Go to a sibling Node (Reverse sens)


	bool GetAttributeValue(const char * attrName, std::string &attrValue);
	std::string GetAttributeValue(const char * attrName);
	bool GetAttributeValueInt(const char * attrName,int &num);
	int GetAttributeValueInt(const char * attrName);

	// -- Attribute Methods Access
	// return true if the current node have the specified attribute defined
	bool Is_Having_Attribute(const char * attrName);				
	
	// -- Setting Attributes & Text
	// Set an attribute to the Current Node
	bool Set_Attribute(const char* AttribName,const char* AttribValue);
	// Remove an attribute the the Current Node
	bool Remove_Attribute(const char* AttribName);		
	// Set a Text to the Current Node
	bool Set_Text(const char* TextValue);
	// 获取当前元素的文本
	const char* Get_Text();



	// 添加一个元素作为当前元素的最后一个子元素,如果添加成功，当前元素将被更新为该子元素
	bool Add_LastChildElement(const char* Name);			
	// 添加一个元素作为当前元素的第一个子元素,如果添加成功，当前元素将被更新为该子元素
	bool Add_FirstChildElement(const char* Name);			
	// 在当元素前面，添加一个元素，如果添加成功，当前元素将被更新为该元素
	bool Add_ElementBefore(const char* Name);
	// 在当元素后面，添加一个元素，如果添加成功，当前元素将被更新为该元素
	bool Add_ElementAfter(const char* Name);

	// -- Removing
	// 删除第一个名称为Name的子元素,如果不存在这个子元素那么返回成功
	bool RemoveFirstChildElement(const char* Name);

	// 删除所有Name名称的子元素,如果不存在这些子元素那么返回成功
	bool RemoveChildElements(const char* Name);

private:
	TiXmlDocument m_doc;
	TiXmlNode * m_currentNode;
};


#endif