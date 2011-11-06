/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_dtrans.hxx"


#include <com/sun/star/datatransfer/dnd/DNDConstants.hpp>

#include "sourcecontext.hxx"
#include <rtl/unload.h>

using namespace com::sun::star::datatransfer::dnd;
using namespace com::sun::star::datatransfer::dnd::DNDConstants;
extern rtl_StandardModuleCount g_moduleCount;

SourceContext::SourceContext( DragSource* pSource,
							 const Reference<XDragSourceListener>& listener):
		WeakComponentImplHelper1<XDragSourceContext>(m_mutex),
		m_pDragSource( pSource),
		m_dragSource( static_cast<XDragSource*>( m_pDragSource) )
{
	g_moduleCount.modCnt.acquire( &g_moduleCount.modCnt );
#if OSL_DEBUG_LEVEL > 1
	if( listener.is())
#endif
	rBHelper.addListener( ::getCppuType( &listener ), listener );
}

SourceContext::~SourceContext()
{
	g_moduleCount.modCnt.release( &g_moduleCount.modCnt );
}

void SAL_CALL SourceContext::addDragSourceListener(
	const Reference<XDragSourceListener >& )
	throw( RuntimeException)
{
}

void SAL_CALL SourceContext::removeDragSourceListener(
	 const Reference<XDragSourceListener >& )
	throw( RuntimeException)
{
}

sal_Int32 SAL_CALL SourceContext::getCurrentCursor(  )
	throw( RuntimeException)
{
	return 0;
}

void SAL_CALL SourceContext::setCursor( sal_Int32 /*cursorId*/ )
	throw( RuntimeException)
{
}

void SAL_CALL SourceContext::setImage( sal_Int32 /*imageId*/ )
	throw( RuntimeException)
{
}

void SAL_CALL SourceContext::transferablesFlavorsChanged(  )
	throw( RuntimeException)
{
}


// non -interface functions
// Fires XDragSourceListener::dragDropEnd events.
void SourceContext::fire_dragDropEnd( sal_Bool success, sal_Int8 effect)
{

	DragSourceDropEvent e;

	if( success == sal_True)
	{
		e.DropAction=  effect;
		e.DropSuccess= sal_True;
	}
	else
	{
		e.DropAction= ACTION_NONE;
		e.DropSuccess= sal_False;
	}
	e.DragSource= m_dragSource;
	e.DragSourceContext= static_cast<XDragSourceContext*>( this);
	e.Source= Reference<XInterface>( static_cast<XDragSourceContext*>( this), UNO_QUERY);

	OInterfaceContainerHelper* pContainer= rBHelper.getContainer(
		getCppuType( (Reference<XDragSourceListener>* )0 ) );

	if( pContainer)
	{
		OInterfaceIteratorHelper iter( *pContainer);
		while( iter.hasMoreElements())
		{
			Reference<XDragSourceListener> listener(
				static_cast<XDragSourceListener*>( iter.next()));
			listener->dragDropEnd( e);
		}
	}
}


void SourceContext::fire_dropActionChanged( sal_Int8 dropAction, sal_Int8 userAction)
{
	if( m_currentAction != dropAction)
	{
		m_currentAction= dropAction;
		DragSourceDragEvent e;
		e.DropAction= dropAction;
		e.UserAction= userAction;
		e.DragSource= m_dragSource;
		e.DragSourceContext= static_cast<XDragSourceContext*>( this);
		e.Source= Reference<XInterface>( static_cast<XDragSourceContext*>( this), UNO_QUERY);

		OInterfaceContainerHelper* pContainer= rBHelper.getContainer(
			getCppuType( (Reference<XDragSourceListener>* )0 ) );

		if( pContainer)
		{
			OInterfaceIteratorHelper iter( *pContainer);
			while( iter.hasMoreElements())
			{
				Reference<XDragSourceListener> listener(
					static_cast<XDragSourceListener*>( iter.next()));
				listener->dropActionChanged( e);
			}
		}
	}
}
