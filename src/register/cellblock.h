/********************************************************************\
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, contact:                        *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 59 Temple Place - Suite 330        Fax:    +1-617-542-2652       *
 * Boston, MA  02111-1307,  USA       gnu@gnu.org                   *
 *                                                                  *
\********************************************************************/

/* 
 * FILE:
 * cellblock.h
 *
 * FUNCTION:
 * The CellBlock struct is a rectangular grid of cells that 
 * define an arrangement of cells.  It is typically used to
 * define a virtual cursor within a larger table of cells.
 *
 * The CellBlock also has utilities to define a tab group.
 * A tab group is an ordered group of cells that are traversed
 * when the user presses the tab key (and/or uses the arrow 
 * keys).
 *
 * The xaccNextRight() method can be used to declare the
 * traversal order from cell to cell.  If the indicated cell
 * has the current input focus, then the next cell that will
 * be traversed to will be the one indicated.  Traversing
 * to the rright is usally performed with the tab key.
 * Special traversal order to the left, up or down are not
 * currently implemented.  
 *
 * To traverse out of the table entirely,
 * the next_row&col should be set to negative values.  If
 * a traversal back into the table occurs, then the cell that 
 * will be entered will be the one with the negative values 
 * minus one.  Thus to traverse out of the table, then back
 * back to (m,n), set the next row-col to (-m-1,-n-1).
 *
 * MEMBERS:
 * The right_traverse array indicates which cell chould be
 * traversed to when the tab key is pressed.
 *
 * HISTORY:
 * Copyright (c) 1988 Linas Vepstas
 */

#ifndef __XACC_CELL_BLOCK_H__
#define __XACC_CELL_BLOCK_H__

#include "basiccell.h"

enum _Alignments {
  ALIGN_RIGHT,
  ALIGN_CENTER,
  ALIGN_LEFT,
  ALIGN_FILL,
};

#ifdef MOTIF
typedef unsigned char Alignments;
#else
typedef enum _Alignments Alignments;
#endif

struct _CellBlock {

  short numRows;
  short numCols;

  /* The array "cells" of pointers to cells has dimensions of numRows*numCols.
   * It is automatically created and managed by the routines below.
   * It contains pointers to the cell handlers that are a part of
   * this "block".
   */
  BasicCell ***cells;  /* row-col array */

  short **cell_types;  /* row-col array of the cell types in splitreg.h */
  
  /* The active_bg_color is the default color (in argb) for the cell
   * backgrounds when this cell block needs to be "highlighted" in 
   * some way (typically, when this cellblock represents the
   * the currently active cursor).
   *
   * The passive_bg_color is the default color for the cell background
   * (in argb format) of the first row when the cell block is not highlighted.
   *
   * The passive_bg_color2 is the default color for cell backgrounds
   * in other rows of the cellblock when it is not highlighted.
   */
  guint32 active_bg_color;
  guint32 passive_bg_color;
  guint32 passive_bg_color2;

  /* other attributes */
  short         *widths;        /* column widths */
  Alignments *alignments;    /* column text alignments */

  short     **right_traverse_r;
  short     **right_traverse_c;
  short     **left_traverse_r;
  short     **left_traverse_c;
  short     right_exit_r;
  short     right_exit_c;
  short     left_exit_r;
  short     left_exit_c;
  /* the above arrays have dimension of numRows*numCols.
   * the are automatically created and managed by the routines below.
   * The control the tab-traversal order through this cell block.
   * If the cell (i,j) has input-focus, then hitting the tab key
   * on the keyboard will take input-focus to cell (inext,jnext), where
   * inext = right_traverse_r[i][j] and jnext = right_traverse_c[i][j].
   *
   *  (exit_r, exit_c) is the last cell of this tab group.
   */

  /* the last-reenter row and column should contain the very last
   * cell when the cursor was traversed out of.  They determine 
   * the first cell that will be entered (since the first follows 
   * the last).
   */
  short last_reenter_traverse_row;
  short last_reenter_traverse_col;

  short last_left_reenter_traverse_row;
  short last_left_reenter_traverse_col;

  void * user_data;
  /* above is a pointer to anything the programmer-user of this struct
   * wants it to be.  Handy for stuff.
   */

};

typedef struct _CellBlock CellBlock;

CellBlock * xaccMallocCellBlock (int numrows, int numcols);
void        xaccInitCellBlock (CellBlock *, int numrows, int numcols);
void        xaccDestroyCellBlock (CellBlock *);

/* define next cell to traverse to */
void        xaccNextRight (CellBlock *, int row,      int col,
                                        int next_row, int next_col);

void        xaccNextLeft (CellBlock *arr, int row,      int col,
                                          int next_row, int next_col);

#endif /* __XACC_CELL_BLOCK_H__ */
