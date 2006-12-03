/**************************************************************************** 
** 
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved. 
** 
** This file is part of "GT-8 FX FloorBoard". 
** 
** This program is free software; you can redistribute it and/or modify 
** it under the terms of the GNU General Public License as published by 
** the Free Software Foundation; either version 2 of the License, or 
** (at your option) any later version. 
** 
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of 
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
** GNU General Public License for more details. 
** 
** You should have received a copy of the GNU General Public License along 
** with this program; if not, write to the Free Software Foundation, Inc., 
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
** 
****************************************************************************/ 
 
#include "MidiTable.h"     
#include "MidiTableDestroyer.h" 
#include <QFile> 
 
MidiTable::MidiTable()  
{ 
    loadMidiMap(); 
}; 
 
MidiTable* MidiTable::_instance = 0;// initialize pointer 
MidiTableDestroyer MidiTable::_destroyer; 
 
MidiTable* MidiTable::Instance()  
{ 
    /* Multi-threading safe */ 
    if (!_instance /*_instance == 0*/)  // is it the first call? 
    {   
        _instance = new MidiTable; // create sole instance 
        _destroyer.SetMidiTable(_instance); 
    }; 
    return _instance; // address of sole instance 
 
    /* Single-threading */ 
    /* 
    static Preferences inst; 
    return &inst; 
    */ 
}; 
 
void MidiTable::loadMidiMap() 
{ 
    QDomDocument doc( "MIDI Transalation" ); 
    QFile file( "midi.xml" ); 
    doc.setContent( &file );                    // file is a QFile 
    file.close(); 
    QDomElement root = doc.documentElement();   // Points to <SysX> 
    this->root = root; 
 
    QVector<Midi> midiMap; 
     
    QDomNode node = root.firstChild(); 
    while ( !node.isNull() )  
    { 
        Midi midi; 
        midi.id = node.nodeName(); 
     
        QDomNode level1Node = node.firstChild(); 
        while ( !level1Node.isNull() )  
        { 
            Level1 level1; 
            level1.id = level1Node.nodeName(); 
            level1.name = level1Node.attributes().namedItem("name").nodeValue(); 
            level1.value = level1Node.attributes().namedItem("value").nodeValue();             
            level1.abbr = level1Node.attributes().namedItem("abbr").nodeValue(); 
            level1.desc = level1Node.attributes().namedItem("desc").nodeValue(); 
 
            QDomNode level2Node = level1Node.firstChild(); 
            level1.empty = level2Node.isNull(); 
            while ( !level2Node.isNull() )  
            { 
                Level2 level2; 
                level2.id = level2Node.nodeName(); 
                level2.name = level2Node.attributes().namedItem("name").nodeValue(); 
                level2.value = level2Node.attributes().namedItem("value").nodeValue();             
                level2.abbr = level2Node.attributes().namedItem("abbr").nodeValue(); 
                level2.desc = level2Node.attributes().namedItem("desc").nodeValue(); 
                level2.empty; 
             
                QDomNode level3Node = level2Node.firstChild(); 
                level2.empty = level3Node.isNull(); 
                while ( !level3Node.isNull() )  
                { 
                    Level3 level3; 
                    level3.id = level3Node.nodeName(); 
                    level3.name = level3Node.attributes().namedItem("name").nodeValue(); 
                    level3.value = level3Node.attributes().namedItem("value").nodeValue();             
                    level3.abbr = level3Node.attributes().namedItem("abbr").nodeValue(); 
                    level3.desc = level3Node.attributes().namedItem("desc").nodeValue(); 
                    level3.empty; 
 
                    QDomNode level4Node = level3Node.firstChild(); 
                    level3.empty = level4Node.isNull(); 
                    while ( !level4Node.isNull() )  
                    { 
                        Level4 level4; 
                        level4.id = level4Node.nodeName(); 
                        level4.name = level4Node.attributes().namedItem("name").nodeValue(); 
                        level4.value = level4Node.attributes().namedItem("value").nodeValue();             
                        level4.abbr = level4Node.attributes().namedItem("abbr").nodeValue(); 
                        level4.desc = level4Node.attributes().namedItem("desc").nodeValue(); 
                        level4.empty; 
 
                        QDomNode level5Node = level4Node.firstChild(); 
                        level4.empty = level5Node.isNull(); 
                        while ( !level5Node.isNull() )  
                        { 
                            Level5 level5; 
                            level5.id = level5Node.nodeName(); 
                            level5.name = level5Node.attributes().namedItem("name").nodeValue(); 
                            level5.value = level5Node.attributes().namedItem("value").nodeValue();             
                            level5.desc = level5Node.attributes().namedItem("desc").nodeValue(); 
 
                            level4.level5.append(level5); 
                            level5Node = level5Node.nextSibling(); 
                        }; 
 
                        level3.level4.append(level4); 
                        level4Node = level4Node.nextSibling(); 
                    }; 
 
                    level2.level3.append(level3); 
                    level3Node = level3Node.nextSibling(); 
                }; 
 
                level1.level2.append(level2); 
                level2Node = level2Node.nextSibling(); 
            }; 
 
            midi.level1.append(level1); 
            level1Node = level1Node.nextSibling(); 
        }; 
 
        midiMap.append(midi); 
        node = node.nextSibling(); 
    }; 
 
    this->midiMap = midiMap; 
}; 
 
QVector<Midi> MidiTable::getMidiMap() 
{ 
    return this->midiMap; 
}; 