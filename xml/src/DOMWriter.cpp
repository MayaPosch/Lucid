//
// DOMWriter.cpp
//
// Library: XML
// Package: DOM
// Module:  DOMWriter
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//



#include "lucid/DOM/DOMWriter.h"
#include "lucid/XML/XMLWriter.h"
#include "lucid/DOM/Document.h"
#include "lucid/DOM/DocumentFragment.h"
#include "lucid/DOM/DocumentType.h"
#include "lucid/DOM/DOMException.h"
#include "lucid/DOM/DOMSerializer.h"
#include "lucid/SAX/LexicalHandler.h"
#include "lucid/XML/XMLException.h"
#include "lucid/Path.h"
#include "lucid/FileStream.h"


namespace Lucid {
namespace XML {


DOMWriter::DOMWriter():
	_pTextEncoding(0),
	_options(0),
	_indent("\t")
{
}


DOMWriter::~DOMWriter()
{
}


void DOMWriter::setEncoding(const std::string& encodingName, Lucid::TextEncoding& textEncoding)
{
	_encodingName  = encodingName;
	_pTextEncoding = &textEncoding;
}


void DOMWriter::setOptions(int options)
{
	_options = options;
}


void DOMWriter::setNewLine(const std::string& newLine)
{
	_newLine = newLine;
}


void DOMWriter::setIndent(const std::string& indent)
{
	_indent = indent;
}


void DOMWriter::writeNode(XMLByteOutputStream& ostr, const Node* pNode)
{
	poco_check_ptr (pNode);

	bool isFragment = pNode->nodeType() != Node::DOCUMENT_NODE;

	XMLWriter writer(ostr, _options, _encodingName, _pTextEncoding);
	writer.setNewLine(_newLine);
	writer.setIndent(_indent);
	
	DOMSerializer serializer;
	serializer.setContentHandler(&writer);
	serializer.setDTDHandler(&writer);
	serializer.setProperty(XMLReader::PROPERTY_LEXICAL_HANDLER, static_cast<LexicalHandler*>(&writer));
	if (isFragment) writer.startFragment();
	serializer.serialize(pNode);
	if (isFragment) writer.endFragment();
}


void DOMWriter::writeNode(const std::string& systemId, const Node* pNode)
{
	Lucid::FileOutputStream ostr(systemId);
	if (ostr.good())
		writeNode(ostr, pNode);
	else 
		throw Lucid::CreateFileException(systemId);
}


} } // namespace Lucid::XML

