/***************************************************************************
               starblockfactory.cpp  -  K Desktop Planetarium
                             -------------------
    begin                : 7 Jun 2008
    copyright            : (C) 2008 by Akarsh Simha
    email                : akarshsimha@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "starblockfactory.h"

// TODO: Remove later
#include <stdio.h>

// TODO: Implement a better way of deciding this
#define DEFAULT_NCACHE 12

StarBlockFactory::StarBlockFactory() {
    first = NULL;
    last = NULL;
    nBlocks = 0;
    drawID = 0;
    nCache = DEFAULT_NCACHE;
}


StarBlockFactory::StarBlockFactory( int nblocks ) {
    first = NULL;
    last = NULL;
    nBlocks = 0;
    drawID = 0;
    nCache = nblocks;
}

StarBlockFactory::~StarBlockFactory() {
    deleteBlocks( nBlocks );
}

StarBlock *StarBlockFactory::getBlock() {
    StarBlock *freeBlock = NULL;
    if( nBlocks < nCache ) {
        freeBlock = new StarBlock;
        if( freeBlock ) {
            ++nBlocks;
            return freeBlock;
        }
    }
    //    fprintf(stderr, "last record %s", (last ? "exists" : "does not exist"));
    if( last && ( last->drawID != drawID || last->drawID == 0 ) ) {
        freeBlock = last;
        last = last->prev;
        if( last )
            last->next = NULL;
        if( freeBlock == first )
            first = NULL;
        freeBlock->prev = NULL;
        freeBlock->next = NULL;
        return freeBlock;
    }
    freeBlock = new StarBlock;
    if( freeBlock )
        ++nBlocks;
    //    fprintf(stderr, "\"Illegal\" block allocation: %d blocks\n", nBlocks);
    return freeBlock;
}

bool StarBlockFactory::markFirst( StarBlock *block ) {

    if( !block )
        return false;

    //    fprintf(stderr, "markFirst()!\n");
    if( !first ) {
        //        fprintf(stderr, "\tLinking in first block!\n");
        last = first = block;
        first->prev = first->next = NULL;
        first->drawID = drawID;
        return true;
    }

    if( block == first ) { // Block is already in the front
        block->drawID = drawID;
        return true;
    }

    if( block == last )
        last = block -> prev;

    if( block->prev )
        block->prev->next = block->next;
    
    if( block->next )
        block->next->prev = block->prev;

    first->prev = block;
    block->next = first;
    block->prev = NULL;
    first = block;

    block->drawID = drawID;

    return true;
}

bool StarBlockFactory::markNext( StarBlock *after, StarBlock *block ) {

    //    fprintf(stderr, "markNext()!\n");
    if( !block || !after )
        return false;

    if( !first ) {
        return false;
    }

    if( block->prev == after ) { // Block is already after 'after'
        block->drawID = drawID;
        return true;
    }

    if( block == first )
        first = block->next;
    if( block == last )
        last = block->prev;

    if( block->prev )
        block->prev->next = block->next;
    if( block->next )
        block->next->prev = block->prev;

    block->next = after->next;
    if( block->next )
        block->next->prev = block;
    block->prev = after;
    after->next = block;

    block->drawID = drawID;

    return true;
}

bool StarBlockFactory::groupMove( StarBlock *start, const int nblocks ) {

    StarBlock *end;

    // Check for trivial cases
    if( !start || nblocks < 0 )
        return false;

    if( nblocks == 0 )
        return true;

    if( !first )
        return false;

    // Check for premature end
    end = start;
    for( int i = 1; i < nblocks; ++i ) {
        if( end  == NULL )
            return false;
        end = end->next;
    }
    if( end == NULL )
        return false;

    // Update drawIDs
    end = start;
    for( int i = 1; i < nblocks; ++i ) {
        end->drawID = drawID;
        end = end->next;
    }
    end->drawID = drawID;

    // Check if we are already in the front
    if( !start->prev )
        return true;

    start->prev->next = end->next;
    end->next->prev = start->prev;

    first->prev = end;
    end->next = first;
    start->prev = NULL;
    first = start;
}

int StarBlockFactory::deleteBlocks( int nblocks ) {
    int i;
    StarBlock *temp;

    fprintf(stderr, "%d StarBlocks freed\n", nblocks );
    return 1;
    i = 0;
    while( last != NULL && i != nblocks ) {
        temp = last->prev;
        delete last;
        last = temp;
        i++;
    }   
    if( last )
        last->next = NULL;
    else
        first = NULL;

    nBlocks -= i;
    return i;
}
