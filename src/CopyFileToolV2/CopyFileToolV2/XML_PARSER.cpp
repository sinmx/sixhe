#include "StdAfx.h"
#include "XML_PARSER.h"

// Su工程需要链接 MD 版本的 tinyxmlSTL.lib, SuInstaller.exe 工程需要链接 MT 版本的 inyxmlSTLMT.lib
// 所以不在代码中显式链接
//#pragma comment(lib,"tinyxmlSTL.lib")

XML_PARSER::XML_PARSER()
{
	m_currentNode = NULL;
}

XML_PARSER::~XML_PARSER()
{

}

bool XML_PARSER::Load_XML_Document(const char *strFileName)
{
	bool success =  m_doc.LoadFile(strFileName);

	if(success)
	{
		m_currentNode = m_doc.RootElement();
	}
	else
	{
		m_currentNode = NULL;
	}

	return success;
}

// Load an XML Document from file
bool XML_PARSER::Load_XML_String(const char * strXml)
{
	m_doc.Parse(strXml);
	m_currentNode = m_doc.RootElement();

	return m_currentNode ? true : false;
}


// Save the Current XML Document into a file
bool XML_PARSER::Save_XML_Document(const char * strFileName)
{
	return m_doc.SaveFile(strFileName);
}

bool XML_PARSER::Get_XML(std::string &xml)
{
	bool success = false;
	TiXmlPrinter printer;

	if(m_doc.Accept( &printer ))
	{
		xml = printer.CStr();
		success = true;
	}

	return success;
}

void XML_PARSER::Go_to_Root()
{
	m_currentNode = m_doc.FirstChildElement();
}

bool XML_PARSER::Go_to_Child(const char * NodeName)
{
	bool success = false;
	TiXmlNode *node;

	if(m_currentNode)
	{
		if(NULL != (node = m_currentNode->FirstChildElement(NodeName)))
		{
			m_currentNode = node;
			success = true;
		}
	}
	
	return success;
}

bool XML_PARSER::Go_to_Parent(const char * NodeName)
{
	bool success = false;

	if(m_currentNode)
	{
		TiXmlNode * parent =  m_currentNode->Parent();
		while(parent)
		{
			if(0 == strcmp(NodeName,parent->Value()))
			{
				m_currentNode = parent;
				success = true;
				break;
			}

			parent = parent->Parent();
		}
	}

	return success;
}

bool XML_PARSER::Go_to_NextSibling(const char * NodeName)
{
	bool success = false;
	TiXmlNode *sibling = NULL;

	if(m_currentNode)
	{
		if(NULL != (sibling = m_currentNode->NextSiblingElement(NodeName)))
		{
			m_currentNode = sibling;
			success = true;
		}
	}

	return success;
}

bool XML_PARSER::Go_to_PrevSibling(const char * NodeName)
{
	bool success = false;
	TiXmlNode *sibling = NULL;

	if(m_currentNode)
	{
		sibling = m_currentNode->PreviousSibling(NodeName);
		while(sibling)
		{
			if(TiXmlNode::TINYXML_ELEMENT == sibling->Type())
			{
				m_currentNode = sibling;
				success = true;
				break;
			}
			else
			{
				sibling = sibling->PreviousSibling(NodeName);
			}
		}
	}

	return success;
}

std::string XML_PARSER::GetAttributeValue(const char * attrName)
{
	TiXmlElement* element = NULL;
	std::string attrValue;

	attrValue = "";
	
	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->ToElement()))
		{
			if(TIXML_SUCCESS == element->QueryStringAttribute(attrName,&attrValue))
			{
			}
		}
	}

	return attrValue;	
}

bool XML_PARSER::GetAttributeValue(const char * attrName, std::string &attrValue)
{
	bool success = false;
	TiXmlElement* element = NULL;

	attrValue = "";
	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->ToElement()))
		{
			if(TIXML_SUCCESS == element->QueryStringAttribute(attrName,&attrValue))
			{
				success = true;
			}
		}
	}

	return success;
}

bool XML_PARSER::GetAttributeValueInt(const char * attrName,int &num)
{
	bool success = false;
	TiXmlElement* element = NULL;

	num = 0;
	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->ToElement()))
		{
			if(TIXML_SUCCESS == element->QueryIntAttribute(attrName,&num))
			{
				success = true;
			}
		}
	}

	return success;
}

int XML_PARSER::GetAttributeValueInt(const char * attrName)
{
	TiXmlElement* element = NULL;

	int num = 0;
	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->ToElement()))
		{
			if(TIXML_SUCCESS == element->QueryIntAttribute(attrName,&num))
			{
			
			}
		}
	}

	return num;	
}

bool XML_PARSER::Is_Having_Attribute(const char * attrName)
{
	bool success = false;
	std::string attrValue;
	TiXmlElement* element = NULL;

	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->ToElement()))
		{
			if(TIXML_SUCCESS == element->QueryStringAttribute(attrName,&attrValue))
			{
				success = true;
			}
		}
	}

	return success;
}

bool XML_PARSER::Set_Attribute(const char* AttribName,const char* AttribValue)
{
	bool success = false;
	TiXmlElement* element = NULL;

	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->ToElement()))
		{
			element->SetAttribute(AttribName,AttribValue);
			success = true;
		}
	}

	return success;
}
// Remove an attribute the the Current Node
bool XML_PARSER::Remove_Attribute(const char* AttribName)
{
	bool success = false;
	TiXmlElement* element = NULL;

	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->ToElement()))
		{
			element->RemoveAttribute(AttribName);
			success = true;
		}
	}

	return success;
}
// Set a Text to the Current Node
bool XML_PARSER::Set_Text(const char* TextValue)
{
	bool success = true;
	TiXmlText xmlText(TextValue);
	TiXmlElement* element = NULL;
	

	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->ToElement()))
		{
			// 没有直接设置文本结点接口，所以只好先移除所有文本结点,移除后，再添加该文本结点
			TiXmlNode *node = NULL;
			bool removeSuccess = true;

			node = element->FirstChild();
			while(node)
			{
				if(TiXmlNode::TINYXML_TEXT == node->Type())
				{
					TiXmlNode *oldNode = node;
					node = node->NextSibling();
					if(!element->RemoveChild(oldNode))
					{
						removeSuccess = false;
						break;
					}
				}
				else
				{
					node = node->NextSibling();
				}
			}

			if(removeSuccess 
				&&  NULL != element->InsertEndChild(xmlText))
			{
				success = true;
			}
		}
	}

	return success;
}

const char* XML_PARSER::Get_Text()
{
	const char *eleText = NULL;
	TiXmlElement* element = NULL;

	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->ToElement()))
		{
			eleText = element->GetText();
		}
	}
	if(NULL == eleText)
	{
		eleText = "";
	}

	return eleText;
}

bool XML_PARSER::Add_LastChildElement(const char* Name)
{
	bool success = true;
	TiXmlElement child(Name);
	TiXmlNode* newNode = NULL;

	if(m_currentNode)
	{
		if(NULL != (newNode = m_currentNode->InsertEndChild(child)))
		{
			m_currentNode = newNode;
			success = true;
		}
	}

	return success;
}

bool XML_PARSER::Add_FirstChildElement(const char* Name)
{
	bool success = true;
	TiXmlElement child(Name);
	TiXmlElement *element = NULL;
	TiXmlNode* newNode = NULL;

	if(m_currentNode)
	{
		if(NULL != ( element = m_currentNode->FirstChildElement()))
		{
			// 在当前第一个子元素前插入
			if(NULL != (newNode = m_currentNode->InsertBeforeChild(element,child)))
			{
				m_currentNode = element;
				success = true;
			}
		}
		else
		{
			// 无子元素，直接添加到子元素尾部
			if(NULL != (newNode = m_currentNode->InsertEndChild(child)))
			{
				m_currentNode = newNode;
				success = true;
			}
		}
	}

	return success;
}

bool XML_PARSER::Add_ElementBefore(const char* Name)
{
	bool success = true;
	TiXmlElement child(Name);
	TiXmlNode *parent = NULL;
	TiXmlNode* newNode = NULL;

	if(m_currentNode)
	{
		if(NULL != ( parent = m_currentNode->Parent()))
		{
			if(NULL != (newNode = parent->InsertBeforeChild(m_currentNode,child)))
			{
				m_currentNode = newNode;
				success = true;
			}
		}
	}

	return success;
}

bool XML_PARSER::Add_ElementAfter(const char* Name)
{
	bool success = true;
	TiXmlElement child(Name);
	TiXmlNode *parent = NULL;
	TiXmlNode* newNode = NULL;

	if(m_currentNode)
	{
		if(NULL != ( parent = m_currentNode->Parent()))
		{
			if(NULL != (newNode = parent->InsertAfterChild(m_currentNode,child)))
			{
				m_currentNode = newNode;
				success = true;
			}
		}
	}

	return success;
}

bool XML_PARSER::RemoveFirstChildElement(const char* Name)
{
	bool success = false;
	TiXmlElement *element;

	if(m_currentNode)
	{
		if(NULL != (element = m_currentNode->FirstChildElement(Name)))
		{
			if(m_currentNode->RemoveChild(element))
			{
				success = true;
			}
		}
		else
		{
			// 不存在，则认为成功
			success = true;
		}
	}

	return success;
}

bool XML_PARSER::RemoveChildElements(const char* Name)
{
	bool success = false;
	TiXmlElement *element;

	if(m_currentNode)
	{
		element = m_currentNode->FirstChildElement(Name);

		while(element)
		{
			if(m_currentNode->RemoveChild(element))
			{
				element = m_currentNode->FirstChildElement(Name);
			}
			else
			{
				break;
			}
		}

		if(NULL == element)
		{
			success = true;
		}
	}

	return success;
}