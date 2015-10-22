//
//  ObjReader.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/16/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "ObjParser.h"
#include "Vertex.h"
using namespace std;

ObjParser::ObjParser() {
}

void ObjParser::parseFile(string path, Item* i) {
    item = i;
    ifstream objFile;
    string buffer;
    
    objFile.open (path);
    
    if (objFile.is_open())
    {
        while ( getline (objFile, buffer) )
        {
            if(buffer.length() < 2) // ignore all short lines
                continue;
            
            // Look for a vertex definition on this line
            if(buffer[0] == 'v' && buffer[1] == ' ')
                item->AddVertex(parseVertex(buffer));
            
            // Look for a texture coordinate on this line
            if(buffer[0] == 'v' && buffer[1] == 't')
                item->AddTextureCoordinate(parseTextureCoordinate(buffer));
            
            // Look for a face definition on this line
            if(buffer[0] == 'f' && buffer[1] == ' ')
                item->AddFace(parseFace(buffer));
        }
        objFile.close();
    }
    
    else cout << "Unable to open .obj file [" << path << "]\n";
    
    objFile.close();
}

Vertex ObjParser::parseVertex(string line) {
    float x, y, z;
    size_t start, end;
    
    start = line.find(" ");
    line[start++] = 'X';
    end = line.find(" ");
    
    x = stof(line.substr(start, end-start));
             
    start = end;
    line[start++] = 'X';
    end = line.find(" ");
             
    y = stof(line.substr(start, end-start));
             
    start = end;
    end = line.length()-1;
             
    z = stof(line.substr(start, end-start));
    
    //cout << "vertex [ " << x << "," << y << "," << z << "]" << endl;
    Vertex v = Vertex(x, y, z);
    return v;
}

TextureCoordinate ObjParser::parseTextureCoordinate(string line) {
    float x, y;
    size_t start, end;
    
    start = line.find(" ");
    line[start++] = 'X';
    end = line.find(" ");
    
    x = stof(line.substr(start, end-start));
    
    start = end;
    end = line.length()-1;
    
    y = stof(line.substr(start, end-start));
    
    
    //cout << "texCor [ " << x << "," << y << "]" << endl;
    
    TextureCoordinate t = TextureCoordinate(x, y);
    return t;
}

Face ObjParser::parseFace(string line) {
    
    Face f = Face();
    
    while (line.find("/") != string::npos) {
        int vert, texCoord;
        size_t start, end;
        
        // set vert and texCoord
        start = line.find(" ");
        line[start++] = 'X';
        end = line.find("/");
        
        vert = stof(line.substr(start, end-start));
        
        line[end++] = 'X';
        start = end;
        end = line.find("/");
        line[end] = 'X';
        
        texCoord = stof(line.substr(start, end-start));
        
        f.addVertex(item->vertices[vert-1]);
        f.addTextureCoordinate(item->textureCoordinates[texCoord-1]);
    }
    
    
    return f;
}
