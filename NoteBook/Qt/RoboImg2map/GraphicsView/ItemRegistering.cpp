
/******************************************************************************
   HotShots: Screenshot utility
   Copyright(C) 2011-2014  xbee@xbee.net

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *******************************************************************************/

#include "ItemRegistering.h"
#include "ItemFactory.h"

#include "EditorLineItem.h"
#include "EditorArrowItem.h"
#include "EditorTagItem.h"
#include "EditorEncircleItem.h"
#include "EditorPolylineItem.h"
#include "EditorPolygonItem.h"
#include "EditorEllipseItem.h"
#include "EditorRectangleItem.h"
#include "EditorBlurItem.h"
#include "EditorTextItem.h"
#include "EditorImageItem.h"
#include "EditorHighlighterItem.h"
#include "EditorMagnifierItem.h"
#include "EditorCurveItem.h"
#include "EditorCropItem.h"

int ItemRegistering::allRegister(ItemFactory *factoryInstance)
{
    factoryInstance->registerType("EditorLineItem", new ItemFactoryTemplate<EditorLineItem>);
    factoryInstance->registerType("EditorArrowItem",  new ItemFactoryTemplate<EditorArrowItem>);
    factoryInstance->registerType("EditorTagItem",  new ItemFactoryTemplate<EditorTagItem>);
    factoryInstance->registerType("EditorEncircleItem",  new ItemFactoryTemplate<EditorEncircleItem>);
    factoryInstance->registerType("EditorPolylineItem",  new ItemFactoryTemplate<EditorPolylineItem>);
    factoryInstance->registerType("EditorPolygonItem",  new ItemFactoryTemplate<EditorPolygonItem>);

    factoryInstance->registerType("EditorEllipseItem",  new ItemFactoryTemplate<EditorEllipseItem>);
    factoryInstance->registerType("EditorRectangleItem",  new ItemFactoryTemplate<EditorRectangleItem>);
    factoryInstance->registerType("EditorBlurItem",  new ItemFactoryTemplate<EditorBlurItem>);
    factoryInstance->registerType("EditorTextItem",  new ItemFactoryTemplate<EditorTextItem>);
    factoryInstance->registerType("EditorImageItem",  new ItemFactoryTemplate<EditorImageItem>);
    factoryInstance->registerType("EditorHighlighterItem",  new ItemFactoryTemplate<EditorHighlighterItem>);

    factoryInstance->registerType("EditorMagnifierItem",  new ItemFactoryTemplate<EditorMagnifierItem>);
    factoryInstance->registerType("EditorCurveItem",  new ItemFactoryTemplate<EditorCurveItem>);
    factoryInstance->registerType("EditorCropItem",  new ItemFactoryTemplate<EditorCropItem>);

    return factoryInstance->types().size();
}
