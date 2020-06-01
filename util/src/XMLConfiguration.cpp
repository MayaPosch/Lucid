//
// XMLConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  XMLConfiguration
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Util/XMLConfiguration.h"


#ifndef POCO_UTIL_NO_XMLCONFIGURATION


#include "lucid/SAX/InputSource.h"
#include "lucid/DOM/DOMParser.h"
#include "lucid/DOM/Element.h"
#include "lucid/DOM/Attr.h"
#include "lucid/DOM/Text.h"
#include "lucid/XML/XMLWriter.h"
#include "lucid/Exception.h"
#include "lucid/NumberParser.h"
#include "lucid/NumberFormatter.h"
#include <set>


namespace Lucid {
namespace Util {


XMLConfiguration::XMLConfiguration():
	_delim('.')
{
	loadEmpty("config");
}


XMLConfiguration::XMLConfiguration(char delim):
	_delim(delim)
{
	loadEmpty("config");
}


XMLConfiguration::XMLConfiguration(Lucid::XML::InputSource* pInputSource):
	_delim('.')
{
	load(pInputSource);
}


XMLConfiguration::XMLConfiguration(Lucid::XML::InputSource* pInputSource, char delim):
	_delim(delim)
{
	load(pInputSource);
}


XMLConfiguration::XMLConfiguration(std::istream& istr):
	_delim('.')
{
	load(istr);
}


XMLConfiguration::XMLConfiguration(std::istream& istr, char delim):
	_delim(delim)
{
	load(istr);
}


XMLConfiguration::XMLConfiguration(const std::string& path):
	_delim('.')
{
	load(path);
}


XMLConfiguration::XMLConfiguration(const std::string& path, char delim):
	_delim(delim)
{
	load(path);
}


XMLConfiguration::XMLConfiguration(const Lucid::XML::Document* pDocument):
	_delim('.')
{
	load(pDocument);
}


XMLConfiguration::XMLConfiguration(const Lucid::XML::Document* pDocument, char delim):
	_delim(delim)
{
	load(pDocument);
}

	
XMLConfiguration::XMLConfiguration(const Lucid::XML::Node* pNode):
	_delim('.')
{
	load(pNode);
}


XMLConfiguration::XMLConfiguration(const Lucid::XML::Node* pNode, char delim):
	_delim(delim)
{
	load(pNode);
}


XMLConfiguration::~XMLConfiguration()
{
}


void XMLConfiguration::load(Lucid::XML::InputSource* pInputSource, unsigned long namePoolSize)
{
	poco_check_ptr (pInputSource);
	
	Lucid::XML::DOMParser parser(namePoolSize);
	parser.setFeature(Lucid::XML::XMLReader::FEATURE_NAMESPACES, false);
	parser.setFeature(Lucid::XML::DOMParser::FEATURE_FILTER_WHITESPACE, true);
	Lucid::XML::AutoPtr<Lucid::XML::Document> pDoc = parser.parse(pInputSource);
	load(pDoc);
}


void XMLConfiguration::load(Lucid::XML::InputSource* pInputSource)
{
	load(pInputSource, POCO_XML_NAMEPOOL_DEFAULT_SIZE);
}


void XMLConfiguration::load(std::istream& istr)
{
	Lucid::XML::InputSource src(istr);
	load(&src);	
}


void XMLConfiguration::load(const std::string& path)
{
	Lucid::XML::InputSource src(path);
	load(&src);	
}

	
void XMLConfiguration::load(const Lucid::XML::Document* pDocument)
{
	poco_check_ptr (pDocument);
	
	_pDocument = Lucid::XML::AutoPtr<Lucid::XML::Document>(const_cast<Lucid::XML::Document*>(pDocument), true);
	_pRoot     = Lucid::XML::AutoPtr<Lucid::XML::Node>(pDocument->documentElement(), true);
}


void XMLConfiguration::load(const Lucid::XML::Node* pNode)
{
	poco_check_ptr (pNode);

	if (pNode->nodeType() == Lucid::XML::Node::DOCUMENT_NODE)
	{
		load(static_cast<const Lucid::XML::Document*>(pNode));
	}
	else
	{
		_pDocument = Lucid::XML::AutoPtr<Lucid::XML::Document>(pNode->ownerDocument(), true);
		_pRoot     = Lucid::XML::AutoPtr<Lucid::XML::Node>(const_cast<Lucid::XML::Node*>(pNode), true);
	}
}


void XMLConfiguration::loadEmpty(const std::string& rootElementName)
{
	_pDocument = new Lucid::XML::Document;
	_pRoot     = _pDocument->createElement(rootElementName);
	_pDocument->appendChild(_pRoot);
}


void XMLConfiguration::save(const std::string& path) const
{
	Lucid::XML::DOMWriter writer;
	writer.setNewLine("\n");
	writer.setOptions(Lucid::XML::XMLWriter::PRETTY_PRINT);
	writer.writeNode(path, _pDocument);
}


void XMLConfiguration::save(std::ostream& ostr) const
{
	Lucid::XML::DOMWriter writer;
	writer.setNewLine("\n");
	writer.setOptions(Lucid::XML::XMLWriter::PRETTY_PRINT);
	writer.writeNode(ostr, _pDocument);
}


void XMLConfiguration::save(Lucid::XML::DOMWriter& writer, const std::string& path) const
{
	writer.writeNode(path, _pDocument);
}


void XMLConfiguration::save(Lucid::XML::DOMWriter& writer, std::ostream& ostr) const
{
	writer.writeNode(ostr, _pDocument);
}


bool XMLConfiguration::getRaw(const std::string& key, std::string& value) const
{
	const Lucid::XML::Node* pNode = findNode(key);
	if (pNode)
	{
		value = pNode->innerText();
		return true;
	}
	else return false;
}


void XMLConfiguration::setRaw(const std::string& key, const std::string& value)
{
	std::string::const_iterator it = key.begin();
	Lucid::XML::Node* pNode = findNode(it, key.end(), _pRoot, true);
	if (pNode)
	{
        unsigned short nodeType = pNode->nodeType();
        if (Lucid::XML::Node::ATTRIBUTE_NODE == nodeType)
        {
            pNode->setNodeValue(value);
        }
        else if (Lucid::XML::Node::ELEMENT_NODE == nodeType)
        {
            Lucid::XML::Node* pChildNode = pNode->firstChild();
            if (pChildNode)
            {
                if (Lucid::XML::Node::TEXT_NODE == pChildNode->nodeType())
                {
                    pChildNode->setNodeValue(value);
                }
            }
            else
            {
				Lucid::AutoPtr<Lucid::XML::Node> pText = _pDocument->createTextNode(value);
				pNode->appendChild(pText);
            }
        }
	}
    else throw NotFoundException("Node not found in XMLConfiguration", key);
}


void XMLConfiguration::enumerate(const std::string& key, Keys& range) const
{
	using Lucid::NumberFormatter;
	
	std::multiset<std::string> keys;
	const Lucid::XML::Node* pNode = findNode(key);
	if (pNode)
	{
		const Lucid::XML::Node* pChild = pNode->firstChild();
		while (pChild)
		{
			if (pChild->nodeType() == Lucid::XML::Node::ELEMENT_NODE)
			{
				const std::string& nodeName = pChild->nodeName();
				int n = (int) keys.count(nodeName);
				if (n)
					range.push_back(nodeName + "[" + NumberFormatter::format(n) + "]");
				else
					range.push_back(nodeName);
				keys.insert(nodeName);
			}
			pChild = pChild->nextSibling();
		}
	}
}


void XMLConfiguration::removeRaw(const std::string& key)
{
	Lucid::XML::Node* pNode = findNode(key);

	if (pNode)
	{
		if (pNode->nodeType() == Lucid::XML::Node::ELEMENT_NODE)
		{
			Lucid::XML::Node* pParent = pNode->parentNode();
			if (pParent)
			{
				pParent->removeChild(pNode);
			}
		}
		else if (pNode->nodeType() == Lucid::XML::Node::ATTRIBUTE_NODE)
		{
			Lucid::XML::Attr* pAttr = dynamic_cast<Lucid::XML::Attr*>(pNode);
			Lucid::XML::Element* pOwner = pAttr->ownerElement();
			if (pOwner)
			{
				pOwner->removeAttributeNode(pAttr);
			}
		}
	}
}


const Lucid::XML::Node* XMLConfiguration::findNode(const std::string& key) const
{
	std::string::const_iterator it = key.begin();
	Lucid::XML::Node* pRoot = const_cast<Lucid::XML::Node*>(_pRoot.get());
	return findNode(it, key.end(), pRoot);
}


Lucid::XML::Node* XMLConfiguration::findNode(const std::string& key)
{
	std::string::const_iterator it = key.begin();
	Lucid::XML::Node* pRoot = const_cast<Lucid::XML::Node*>(_pRoot.get());
	return findNode(it, key.end(), pRoot);
}


Lucid::XML::Node* XMLConfiguration::findNode(std::string::const_iterator& it, const std::string::const_iterator& end, Lucid::XML::Node* pNode, bool create) const
{
	if (pNode && it != end)
	{
		if (*it == '[')
		{
			++it;
			if (it != end && *it == '@')
			{
				++it;
				std::string attr;
				while (it != end && *it != ']' && *it != '=') attr += *it++;
				if (it != end && *it == '=')
				{
					++it;
					std::string value;
					if (it != end && *it == '\'')
					{
						++it;
						while (it != end && *it != '\'') value += *it++;
						if (it != end) ++it;
					}
					else
					{
						while (it != end && *it != ']') value += *it++;
					}
					if (it != end) ++it;
					return findNode(it, end, findElement(attr, value, pNode), create);
				}
				else
				{
					if (it != end) ++it;
					return findAttribute(attr, pNode, create);
				}
			}
			else
			{
				std::string index;
				while (it != end && *it != ']') index += *it++;
				if (it != end) ++it;
				return findNode(it, end, findElement(Lucid::NumberParser::parse(index), pNode, create), create);
			}
		}
		else
		{
			while (it != end && *it == _delim) ++it;
			std::string key;
			while (it != end && *it != _delim && *it != '[') key += *it++;
			return findNode(it, end, findElement(key, pNode, create), create);
		}
	}
	else return pNode;
}


Lucid::XML::Node* XMLConfiguration::findElement(const std::string& name, Lucid::XML::Node* pNode, bool create)
{
	Lucid::XML::Node* pChild = pNode->firstChild();
	while (pChild)
	{
		if (pChild->nodeType() == Lucid::XML::Node::ELEMENT_NODE && pChild->nodeName() == name)
			return pChild;
		pChild = pChild->nextSibling();
	}
	if (create)
	{
		Lucid::AutoPtr<Lucid::XML::Element> pElem = pNode->ownerDocument()->createElement(name);
		pNode->appendChild(pElem);
		return pElem;
	}
	else return 0;
}


Lucid::XML::Node* XMLConfiguration::findElement(int index, Lucid::XML::Node* pNode, bool create)
{
	Lucid::XML::Node* pRefNode = pNode;
	if (index > 0)
	{
		pNode = pNode->nextSibling();
		while (pNode)
		{
			if (pNode->nodeName() == pRefNode->nodeName())
			{
				if (--index == 0) break;
			}
			pNode = pNode->nextSibling();
		}
	}
	if (!pNode && create)
	{
		if (index == 1)
		{
			Lucid::AutoPtr<Lucid::XML::Element> pElem = pRefNode->ownerDocument()->createElement(pRefNode->nodeName());
			pRefNode->parentNode()->appendChild(pElem);
			return pElem;
		}
		else throw Lucid::InvalidArgumentException("Element index out of range.");
	}
	return pNode;
}


Lucid::XML::Node* XMLConfiguration::findElement(const std::string& attr, const std::string& value, Lucid::XML::Node* pNode)
{
	Lucid::XML::Node* pRefNode = pNode;
	Lucid::XML::Element* pElem = dynamic_cast<Lucid::XML::Element*>(pNode);
	if (!(pElem && pElem->getAttribute(attr) == value))
	{
		pNode = pNode->nextSibling();
		while (pNode)
		{
			if (pNode->nodeName() == pRefNode->nodeName())
			{
				pElem = dynamic_cast<Lucid::XML::Element*>(pNode);
				if (pElem && pElem->getAttribute(attr) == value) break;
			}
			pNode = pNode->nextSibling();
		}
	}
	return pNode;
}


Lucid::XML::Node* XMLConfiguration::findAttribute(const std::string& name, Lucid::XML::Node* pNode, bool create)
{
	Lucid::XML::Node* pResult(0);
	Lucid::XML::Element* pElem = dynamic_cast<Lucid::XML::Element*>(pNode);
	if (pElem)
	{
		pResult = pElem->getAttributeNode(name);
		if (!pResult && create)
		{
			Lucid::AutoPtr<Lucid::XML::Attr> pAttr = pNode->ownerDocument()->createAttribute(name);
			pElem->setAttributeNode(pAttr);
			return pAttr;
		}
	}
	return pResult;
}


} } // namespace Lucid::Util

#endif // POCO_UTIL_NO_XMLCONFIGURATION
