/****************************************************************************
** Filename: XMLWriter.cpp
** Last updated [dd/mm/yyyy]: 19/08/2005
**
** Class for XML file generation [Requires Qt4 - QtCore module].
**
** Copyright (C) 2005 Angius Fabrizio. All rights reserved.
**
** This file is part of the OSDaB project (http://osdab.sourceforge.net/).
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See the file LICENSE.GPL that came with this software distribution or
** visit http://www.gnu.org/copyleft/gpl.html for GPL licensing information.
**
**********************************************************************/


#include "XMLWriter.h"

#include <QtCore/QRegExp>

/*
 * Builds a new XML writer for given IO device and text encoding.
 * A first line with xml encoding type is written if writeEncoding=true.
 * Default UTF-8 encoding is used if codec==0.
 * Indenting uses '\t'-tabs by default.
 * As for default, a CRLF is automatically added after each tag.
 */
XMLWriter::XMLWriter(QIODevice* device, QTextCodec* codec, bool writeEncoding)
{
	mOut = new QTextStream(device);
	if (codec == 0) mOut->setCodec("UTF-8");
	else mOut->setCodec(codec);

	mIndentString = "\t";
	mAutoNL = true;
	mNewLine = "\r\n";
	mIndent = 0;
	mPauseIndent = false;

	// <?xml version="1.0" encoding="SELECTED_ENCODING"?>
	if (writeEncoding)
		(*mOut) << "<?xml version=\"1.0\" encoding=\"" << escape(QString(mOut->codec()->name())) << "\"?>" << mNewLine /*<< mNewLine*/;
}



/*
 * Unsets the used device.
 */
XMLWriter::~XMLWriter()
{
	mOut->flush();

	delete mOut;
	mOut = 0;
}


/*
 * Writes out a <?xml version="1.0" encoding="XXX"?> string followed by two new lines.
 * XXX is the encoding specified to the constructor.
 */
void XMLWriter::writeEncoding()
{
	(*mOut) << "<?xml version=\"1.0\" encoding=\"" << escape(QString(mOut->codec()->name())) << "\"?>" << mNewLine << mNewLine;
}


/*
 * Sets the newline type to either CR, CR+LF or LF
 */
void XMLWriter::setNewLine(NewLineType type)
{
	switch (type)
	{
	case CR: mNewLine = "\r"; break;
	case CRLF: mNewLine = "\r\n"; break;
	case LF: mNewLine = "\n";
	}
}


/*
 * Writes a (raw) string.
 */
void XMLWriter::writeString(const QString& string	)
{
	(*mOut) << escape(string);
}


/*
 * Writes an opening tag with given name and attributes.
 * Example: <item id="X23">
 */
void XMLWriter::writeOpenTag(const QString& name, const QMap<QString,QString>* attrs)
{
	if (!mPauseIndent)
	for (int i=0; i<mIndent; ++i)
		(*mOut) << mIndentString;
	mIndent++;

	(*mOut) << openTag(name, attrs);
	if (mAutoNL)
		(*mOut) << mNewLine;
}


/*
 * Writes a closing tag with given name.
 * Example: </item>
 */
void XMLWriter::writeCloseTag(const QString& name)
{
	mIndent--;

	if (!mPauseIndent)
	for (int i=0; i<mIndent; ++i)
		(*mOut) << mIndentString;

	(*mOut) << "</" << escape(name) << ">";
	if (mAutoNL)
		(*mOut) << mNewLine;
}


/*
 * Writes an atom tag with given name and attributes
 * Example: <item id=X23/>
 */
void XMLWriter::writeAtomTag(const QString& name, const QMap<QString,QString>* attrs)
{
	if (!mPauseIndent)
    for (int i=0; i<mIndent; ++i)
		(*mOut) << mIndentString;

	QString atom = openTag(name, attrs);
	atom.truncate(atom.length() - 1);
	(*mOut) << atom << "/>";

	if (mAutoNL)
		(*mOut) << mNewLine;
}


/*
 * Writes an opening tag, a string and a closing tag.
 * Example: <item id="i_love_attributes">some text</item>
 */
void XMLWriter::writeTaggedString(const QString& name, const QString& string, const QMap<QString,QString>* attrs)
{
	if (!mPauseIndent)
	for (int i=0; i<mIndent; ++i)
		(*mOut) << mIndentString;

	(*mOut) << openTag(name, attrs) << escape(string) << "</" << escape(name) << ">";

	if (mAutoNL)
		(*mOut) << mNewLine;
}


/*
 * Writes a comment using "<!-- " and " -->" tags.
 */
void XMLWriter::writeComment(const QString& comment)
{
	if (!mPauseIndent)
	for (int i=0; i<mIndent; ++i)
		(*mOut) << mIndentString;

	// we do not want the comments to end before WE want it ;)
	QString com(comment);
	com.replace(QString("-->"), QString("==>"));

	(*mOut) << "<!-- " << com << " -->";

	if (mAutoNL)
		(*mOut) << mNewLine;
}


/*
 * Starts a comment with the "<!-- " tag. A newline is never added.
 */
void XMLWriter::startComment()
{
	if (!mPauseIndent)
	for (int i=0; i<mIndent; ++i)
		(*mOut) << mIndentString;

	(*mOut) << "<!-- ";
}


/*
 * Closes a comment by writing the " -->" tag. No newline or indent is written.
 * Same as calling writeString(" -->") - still don't know why the heck i added this call here ;)
 */
void XMLWriter::endComment()
{
	(*mOut) << " -->";
}


/*
 * Outputs a newline (default is CR+LF).
 */
void XMLWriter::newLine()
{
	(*mOut) << mNewLine;
}


/*
 * Writes out some indent (level is determined by previous tags).
 * Any value set with pauseIndent() is ignored.
 */
void XMLWriter::writeCurrentIndent()
{
	for (int i=0; i<mIndent; ++i)
		(*mOut) << mIndentString;
}


/*
 * Stops writing out indents.
 * Indentation level is still recorded as tags get opened or closed.
 */
void XMLWriter::pauseIndent(bool pause)
{
	mPauseIndent = pause;
}


/*
 * Replaces special chars with escape sequences.
 */
QString XMLWriter::escape(const QString& string) const
{
	QString s = string;
	s.replace("&", "&amp;");
	s.replace(">", "&gt;");
	s.replace("<", "&lt;");
	s.replace("\"", "&quot;");
	s.replace("\'", "&apos;");
	return s;
}


/*
 * Returns an opening tag with evtl attributes in the form attr="value"
 */
QString XMLWriter::openTag(const QString& tag, const QMap<QString,QString>* attributes)
{
	QString out = "<" + escape(tag);

	if (attributes != 0)
		for (QMap<QString,QString>::ConstIterator itr=attributes->constBegin(); itr!=attributes->constEnd(); ++itr)
			out += " " + escape(itr.key()) + "=\"" + escape(itr.value()) + "\"";

	out += ">";
	return out;
}


/*
 * Sets the number of spaces to use for indent. Real '\t'-tabs are used if spaces < 0.
 */
void XMLWriter::setIndentType(int spaces)
{
	if (spaces < 0)
		mIndentString = "\t";
	else
	{
		mIndentString = "";
		mIndentString.fill(' ', spaces);
	}
}
