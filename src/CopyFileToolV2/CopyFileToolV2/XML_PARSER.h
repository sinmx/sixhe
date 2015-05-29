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
	// ��ȡ��ǰԪ�ص��ı�
	const char* Get_Text();



	// ���һ��Ԫ����Ϊ��ǰԪ�ص����һ����Ԫ��,�����ӳɹ�����ǰԪ�ؽ�������Ϊ����Ԫ��
	bool Add_LastChildElement(const char* Name);			
	// ���һ��Ԫ����Ϊ��ǰԪ�صĵ�һ����Ԫ��,�����ӳɹ�����ǰԪ�ؽ�������Ϊ����Ԫ��
	bool Add_FirstChildElement(const char* Name);			
	// �ڵ�Ԫ��ǰ�棬���һ��Ԫ�أ������ӳɹ�����ǰԪ�ؽ�������Ϊ��Ԫ��
	bool Add_ElementBefore(const char* Name);
	// �ڵ�Ԫ�غ��棬���һ��Ԫ�أ������ӳɹ�����ǰԪ�ؽ�������Ϊ��Ԫ��
	bool Add_ElementAfter(const char* Name);

	// -- Removing
	// ɾ����һ������ΪName����Ԫ��,��������������Ԫ����ô���سɹ�
	bool RemoveFirstChildElement(const char* Name);

	// ɾ������Name���Ƶ���Ԫ��,�����������Щ��Ԫ����ô���سɹ�
	bool RemoveChildElements(const char* Name);

private:
	TiXmlDocument m_doc;
	TiXmlNode * m_currentNode;
};


#endif