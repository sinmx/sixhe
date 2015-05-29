#include "StdAfx.h"
#include "UITree.h"
namespace DuiLib {

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CTreeNode::CTreeNode() :
	m_pParentNode( NULL ),
	m_pListElementUI( NULL ),
	m_pExpandControl( NULL ),
	m_pSelectControl( NULL ),
	m_bIsNodeExpand( false ),
	m_bIsNodeSelected( true ),
	m_nNodeLevel(0)
	{
	}

	CTreeNode::~CTreeNode()
	{
		for( int index = 0; index < GetChildrenNum(); ++index )
		{
			delete m_vecChildrenNode[index];
			m_vecChildrenNode[index] = NULL;
		}
	}

	void CTreeNode::SetNodeLevel( int nodeLevel )
	{
		m_nNodeLevel = nodeLevel;
	}

	int CTreeNode::GetNodeLevel()
	{
		return m_nNodeLevel;
	}

	void CTreeNode::SetNodeExpand( bool bNodeExpand )
	{
		m_bIsNodeExpand = bNodeExpand;
	}

	bool CTreeNode::IsNodeExpand()
	{
		return m_bIsNodeExpand;
	}

	void CTreeNode::SetNodeSelected( bool bNodeSelected )
	{
		m_bIsNodeSelected = bNodeSelected;
		if( NULL != m_pSelectControl )
			((COptionUI*)m_pSelectControl)->Selected( m_bIsNodeSelected );
	}

	bool CTreeNode::IsNodeSelected()
	{
		return m_bIsNodeSelected;
	}

	void CTreeNode::SetListElementUI( CControlUI* pListElementUI )
	{
		ASSERT( NULL != pListElementUI );
		m_pListElementUI = pListElementUI;
	}

	CControlUI* CTreeNode::GetListElementUI()
	{
		ASSERT( NULL != m_pListElementUI );
		return m_pListElementUI;
	}

	void CTreeNode::SetExpandControl( CControlUI* pExpandControl )
	{
		if( NULL != pExpandControl )
		{
			m_pExpandControl = pExpandControl;
			m_pExpandControl->SetVisible( false );
		}
	}

	CControlUI* CTreeNode::GetExpandControl()
	{
		return m_pExpandControl;
	}

	void CTreeNode::SetSelectControl( CControlUI* pSelectControl )
	{
		if( NULL != pSelectControl )
		{
			m_pSelectControl = pSelectControl;
			((COptionUI*)m_pSelectControl)->Selected( true );
		}
	}

	CControlUI* CTreeNode::GetSelectControl()
	{
		return m_pSelectControl;
	}

	int CTreeNode::GetChildrenNum()
	{
		return m_vecChildrenNode.size();
	}

	bool CTreeNode::IsHasChildren()
	{
		return m_vecChildrenNode.size() > 0;
	}

	CTreeNode* CTreeNode::GetChildrenAt( int index )
	{
		if( index < 0 || index > ( GetChildrenNum() - 1 ) || m_vecChildrenNode.size() == 0 )
			return NULL;
		return m_vecChildrenNode[index];
	}

	CTreeNode* CTreeNode::GetLastChildren()
	{
		if( IsHasChildren() &&  NULL != GetChildrenAt( GetChildrenNum() - 1 ) )
		{
			return GetChildrenAt( GetChildrenNum() - 1 )->GetLastChildren(); // recursive find last children
		}
		else
			return this;
	}

	void CTreeNode::AddChildren( CTreeNode* pTreeNode )
	{
		ASSERT( NULL != pTreeNode );
		pTreeNode->SetNodeParent( this );
		m_vecChildrenNode.push_back( pTreeNode );
		if( IsHasChildren() && NULL != m_pExpandControl  )
			m_pExpandControl->SetVisible( true );
	}

	bool CTreeNode::RemoveChildren( CTreeNode* pTreeNode )
	{
		ChildrenNodeVec::iterator iter = m_vecChildrenNode.begin();
		bool bRemove = false;
		for( ; iter != m_vecChildrenNode.end(); ++iter )
		{
			if( *iter == pTreeNode )
			{
				m_vecChildrenNode.erase( iter );
				bRemove = true;
				break;
			}
		}	
		return bRemove;
	}

	CTreeNode* CTreeNode::GetParent()
	{
		return m_pParentNode;
	}

	void CTreeNode::SetNodeParent( CTreeNode* pTreeNode )
	{
		ASSERT( NULL != pTreeNode );
		m_pParentNode = pTreeNode;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CTreeUI::CTreeUI()
	{	
		m_pRoot = new CTreeNode();
		if( NULL == m_pRoot )
		{
			m_pRoot->SetNodeLevel( - 1 );
			m_pRoot->SetNodeExpand( false );
			m_pRoot->SetNodeSelected( false );
			m_pRoot->SetListElementUI( NULL );
		}
	}	

	CTreeUI::~CTreeUI()
	{
		if( NULL != m_pRoot )
		{
			delete m_pRoot;
			m_pRoot = NULL;
		}
	}

	LPCTSTR CTreeUI::GetClass() const
	{
		return _T("TreeUI");
	}

	LPVOID CTreeUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("Tree")) == 0 ) return static_cast<CTreeUI*>(this);
		if( _tcscmp(pstrName, _T("IList")) == 0 ) return static_cast<IListUI*>(this);
		if( _tcscmp(pstrName, _T("IListOwner")) == 0 ) return static_cast<IListOwnerUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	bool CTreeUI::PreCheckControl( CControlUI* pControl )
	{
		if( NULL == pControl ) 
			return false;
		if( _tcscmp(m_sListElementUIClassName.GetData(), _T("ListElementUI")) != 0 &&
			_tcscmp(m_sListElementUIClassName.GetData(), _T("ListLabelElementUI")) != 0 &&
			_tcscmp(m_sListElementUIClassName.GetData(), _T("ListTextElementUI")) != 0 &&
			_tcscmp(m_sListElementUIClassName.GetData(), _T("ListContainerElementUI")) != 0 
			) 
			return false;
		return true;
	}

	bool CTreeUI::Add( CControlUI* pControl )
	{
		if( !PreCheckControl( pControl ) )
			return false;
		return CListUI::Add(pControl);
	}

	bool CTreeUI::AddAt( CControlUI* pControl, int index )
	{
		if( !PreCheckControl( pControl ) )
			return false;
		return CListUI::AddAt(pControl, index);
	}

	bool CTreeUI::Remove( CControlUI* pControl )
	{
		if( !PreCheckControl( pControl ) )
			return false;

		if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListElementUI") ) == 0 )
		{
			if (reinterpret_cast<CTreeNode*>(static_cast<CListElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()) == NULL)
				return CListUI::Remove(pControl);
			else
				return RemoveNode(reinterpret_cast<CTreeNode*>(static_cast<CListElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()));
		}
		else if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListLabelElementUI") ) == 0 )
		{
			if (reinterpret_cast<CTreeNode*>(static_cast<CListLabelElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()) == NULL)
				return CListUI::Remove(pControl);
			else
				return RemoveNode(reinterpret_cast<CTreeNode*>(static_cast<CListLabelElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()));
		}
		else if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListTextElementUI") ) == 0 )
		{
			if (reinterpret_cast<CTreeNode*>(static_cast<CListTextElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()) == NULL)
				return CListUI::Remove(pControl);
			else
				return RemoveNode(reinterpret_cast<CTreeNode*>(static_cast<CListTextElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()));
		}
		else if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListContainerElementUI") ) == 0 )
		{
			if (reinterpret_cast<CTreeNode*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()) == NULL)
				return CListUI::Remove(pControl);
			else
				return RemoveNode(reinterpret_cast<CTreeNode*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()));
		}

		return true;
	}

	bool CTreeUI::RemoveAt( int index )
	{
		CControlUI* pControl = GetItemAt(index);
		if( !PreCheckControl( pControl ) )
			return false;

		if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListElementUI") ) == 0 )
		{
			if (reinterpret_cast<CTreeNode*>(static_cast<CListElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()) == NULL)
				return CListUI::RemoveAt(index);
			else
				return RemoveNode(reinterpret_cast<CTreeNode*>(static_cast<CListElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()));
		}
		else if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListLabelElementUI") ) == 0 )
		{
			if (reinterpret_cast<CTreeNode*>(static_cast<CListLabelElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()) == NULL)
				return CListUI::RemoveAt(index);
			else
				return RemoveNode(reinterpret_cast<CTreeNode*>(static_cast<CListLabelElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()));
		}
		else if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListTextElementUI") ) == 0 )
		{
			if (reinterpret_cast<CTreeNode*>(static_cast<CListTextElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()) == NULL)
				return CListUI::RemoveAt(index);
			else
				return RemoveNode(reinterpret_cast<CTreeNode*>(static_cast<CListTextElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()));
		}
		else if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListContainerElementUI") ) == 0 )
		{
			if (reinterpret_cast<CTreeNode*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()) == NULL)
				return CListUI::RemoveAt(index);
			else
				return RemoveNode(reinterpret_cast<CTreeNode*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElementUI")))->GetTag()));
		}

		return true;
	}

	void CTreeUI::RemoveAll()
	{
		CListUI::RemoveAll();
		for( int index = 0; index < m_pRoot->GetChildrenNum(); ++index )
		{
			CTreeNode* pNode = m_pRoot->GetChildrenAt( index );
			RemoveNode( pNode );
		}
		delete m_pRoot;
		m_pRoot = NULL;
		m_pRoot = new CTreeNode();
		m_pRoot->SetNodeLevel( - 1 );
		m_pRoot->SetNodeExpand( false );
		m_pRoot->SetNodeSelected( false );		
	}

	void CTreeUI::SetVisible(bool bVisible )
	{
		if( m_bVisible == bVisible ) return;
		CControlUI::SetVisible(bVisible);
	}

	void CTreeUI::SetInternVisible(bool bVisible )
	{
		CControlUI::SetInternVisible(bVisible);
	}

	CTreeNode* CTreeUI::AddNode( CTreeNode* pNodeRoot, CControlUI* pListElementUI,
		COptionUI* pExpandOption, bool bIsExpand, COptionUI* pSelectedOption, bool bIsSelected, int vanum, ... )
	{
		ASSERT( NULL != pListElementUI );
		if( NULL == pNodeRoot )
			pNodeRoot = m_pRoot;

		m_sListElementUIClassName = pListElementUI->GetClass();
		CTreeNode* node = new CTreeNode();
		if( NULL == node )
			return NULL;
		node->SetNodeLevel( pNodeRoot->GetNodeLevel() + 1 );
		node->SetNodeExpand( bIsExpand );
		node->SetNodeSelected( bIsSelected );
		node->SetListElementUI( pListElementUI );
		node->SetExpandControl( pExpandOption );
		node->SetSelectControl( pSelectedOption );
		pListElementUI->SetTag( (UINT_PTR)node );
		if( NULL != pExpandOption )
		{
			pExpandOption->SetTag( (UINT_PTR)node );
			pExpandOption->OnEvent += MakeDelegate<CTreeUI, CTreeUI>( this, &CTreeUI::OnCommonEvent4ExpandOption );
		}
		if( NULL != pSelectedOption )
		{
			pSelectedOption->SetTag( (UINT_PTR)node );
			pSelectedOption->OnEvent += MakeDelegate<CTreeUI, CTreeUI>( this, &CTreeUI::OnCommonEvent4SelectedOption );
		}

		va_list controlList;
		va_start( controlList, vanum );
		while( --vanum >= 0 )
		{		
			CControlUI* pControl = NULL;
			pControl = va_arg( controlList, CControlUI* );
			if( NULL != pControl )		
				pControl->SetTag( (UINT_PTR)node );
		}
		va_end( controlList );

		if( pNodeRoot != m_pRoot )
		{
			if( !( pNodeRoot->IsNodeExpand() && pNodeRoot->GetListElementUI()->IsVisible() ) )
				pListElementUI->SetInternVisible( false );
		}

		int index = 0;
		CTreeNode* lastChildren = NULL;
		if( pNodeRoot->IsHasChildren() )
		{
			lastChildren = pNodeRoot->GetLastChildren();
		}
		else if( pNodeRoot != m_pRoot )
		{
			lastChildren = pNodeRoot;
		}

		if( NULL != lastChildren )
		{
			index = GetListElementIndex( lastChildren );
			if( -1 == index )
				return NULL;
			else
				index+=1;
		}

		if( !AddAt( pListElementUI, index ))
		{
			delete pListElementUI;
			delete node;
			pListElementUI = NULL;
			node = NULL;
		}
		pNodeRoot->AddChildren( node );
		return node;
	}

	void CTreeUI::OnExpandNode( CTreeNode* pTreeNode )
	{
		if( ( NULL == pTreeNode || pTreeNode == m_pRoot ) && 
			!pTreeNode->IsHasChildren() )
			return;

		pTreeNode->SetNodeExpand( !pTreeNode->IsNodeExpand() );
		CTreeNode* pBeginNode = pTreeNode->GetChildrenAt( 0 );
		CTreeNode* pEndNode = pTreeNode->GetLastChildren();
		int beginIndex = GetListElementIndex( pBeginNode );
		int endIndex = GetListElementIndex( pEndNode );
		if( -1 == beginIndex || -1 == endIndex )
			return;

		for( int index = beginIndex; index <= endIndex; ++index )
		{
			CControlUI* pControl = GetItemAt( index );
			if( NULL != pControl )
			{
				if( _tcscmp( pControl->GetClass(), m_sListElementUIClassName.GetData() ) == 0 )
				{
					if( NULL != pControl->GetTag() )
					{
						CTreeNode* pNodeRoot = ( ( CTreeNode*)pControl->GetTag() )->GetParent();
						if( NULL != pNodeRoot )
							pControl->SetInternVisible( pNodeRoot->IsNodeExpand() && pNodeRoot->GetListElementUI()->IsVisible() );
					}
				}
			}
		}
		NeedUpdate();
	}

	void CTreeUI::OnSelectedNode( CTreeNode* pTreeNode, bool bSelected )
	{
		if( NULL== pTreeNode )
			return;
		SelectedNode( pTreeNode, bSelected );
		CTreeNode* pBeginNode;
		CTreeNode* pEndNode;
		if( pTreeNode->IsHasChildren() )
		{
			pBeginNode = pTreeNode->GetChildrenAt(0); 
			pEndNode = pTreeNode->GetLastChildren();
			int beginIndex = GetListElementIndex( pBeginNode );
			int endIndex = GetListElementIndex( pEndNode );
			if( -1 == beginIndex || -1 == endIndex )
				return;

			for( int index = beginIndex; index <= endIndex; ++index ) 
			{
				CControlUI* control = GetItemAt(index);
				CTreeNode* pChildrenNode = (CTreeNode*)control->GetTag();
				SelectedNode( pChildrenNode, bSelected );
			}	
		}

		CTreeNode* pParentNode = NULL;
		pParentNode = pTreeNode->GetParent();
		if( NULL == pParentNode )
			return;

		if( bSelected )
		{
			do
			{
				pBeginNode = pParentNode->GetChildrenAt(0);
				pEndNode = pParentNode->GetLastChildren();
				int index;
				int beginIndex = GetListElementIndex( pBeginNode );
				int endIndex = GetListElementIndex( pEndNode );
				if( -1 == beginIndex || -1 == endIndex )
					return;
				for( index = beginIndex; index <= endIndex; ++index ) 
				{
					CControlUI* control = GetItemAt(index);
					CTreeNode* pChildrenNode = (CTreeNode*)control->GetTag();
					if( pChildrenNode->IsNodeSelected() != bSelected )
						break;
				}
				if( index > endIndex )
					SelectedNode( pParentNode, bSelected );

				pParentNode = pParentNode->GetParent();
				if( !pParentNode )
					break;

			}while( true );
		}
		else
		{
			do
			{
				SelectedNode( pParentNode, bSelected );
				pParentNode = pParentNode->GetParent();
				if( !pParentNode )
					break;
			}while( true );
		}
	}

	void CTreeUI::SelectedNode( CTreeNode* pTreeNode, bool bSelected )
	{
		if( NULL == pTreeNode || 
			pTreeNode == m_pRoot ||
			pTreeNode->IsNodeSelected() == bSelected
			)
			return;

		pTreeNode->SetNodeSelected( bSelected );

		if( !pTreeNode->GetListElementUI()->IsVisible() ||
			!pTreeNode->IsHasChildren() 
			) 
			return;

		NeedUpdate();
	}

	bool CTreeUI::RemoveNode( CTreeNode* pTreeNode )
	{
		if( NULL == pTreeNode || pTreeNode == m_pRoot )
			return false;
		for( int index = 0; index < pTreeNode->GetChildrenNum(); ++index )
		{
			CTreeNode* pChildren = pTreeNode->GetChildrenAt(index);
			RemoveNode( pChildren );
		}

		CListUI::Remove( pTreeNode->GetListElementUI() );
		pTreeNode->GetParent()->RemoveChildren( pTreeNode );
		delete pTreeNode;
		pTreeNode = NULL;
		return true;
	}

	int CTreeUI::GetListElementIndex( CTreeNode* pNode )
	{
		int index = -1;
		if( NULL != pNode )
		{
			if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListElementUI") ) == 0 )
			{
				index = ((CListElementUI*)pNode->GetListElementUI())->GetIndex();
			}
			else if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListLabelElementUI") ) == 0 )
			{
				index = ((CListLabelElementUI*)pNode->GetListElementUI())->GetIndex();
			}
			else if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListTextElementUI") ) == 0 )
			{
				index = ((CListTextElementUI*)pNode->GetListElementUI())->GetIndex();
			}
			else if( _tcscmp( m_sListElementUIClassName.GetData(), _T("ListContainerElementUI") ) == 0 )
			{
				index = ((CListContainerElementUI*)pNode->GetListElementUI())->GetIndex();
			}
		}		
		return index;
	}

	bool CTreeUI::OnCommonEvent4ExpandOption( void* pEvent )
	{
		if( ((TEventUI*)pEvent)->Type == UIEVENT_BUTTONDOWN ||
			((TEventUI*)pEvent)->Type == UIEVENT_DBLCLICK ) 
		{
			CControlUI* pExpandOption = ((TEventUI*)pEvent)->pSender;
			if( NULL != pExpandOption )
			{
				CTreeNode* pCurrentNode = (CTreeNode*)((TEventUI*)pEvent)->pSender->GetTag();  
				if( NULL != pCurrentNode )
				{
					OnExpandNode( pCurrentNode );
				}
			}
		}
		else if( ((TEventUI*)pEvent)->Type == UIEVENT_SETCURSOR ) 
		{
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		}
		return true;
	}

	bool CTreeUI::OnCommonEvent4SelectedOption( void* pEvent )
	{
		if( ((TEventUI*)pEvent)->Type == UIEVENT_BUTTONDOWN ||
			((TEventUI*)pEvent)->Type == UIEVENT_DBLCLICK ) 
		{
			CControlUI* pCheckOption = ((TEventUI*)pEvent)->pSender;
			if( NULL != pCheckOption )
			{
				CTreeNode* pCurrentNode = (CTreeNode*)((TEventUI*)pEvent)->pSender->GetTag();  
				if( NULL != pCurrentNode )
				{
					OnSelectedNode( pCurrentNode, !( pCurrentNode->IsNodeSelected() ) );
					((COptionUI*)pCurrentNode->GetSelectControl())->Selected( !( pCurrentNode->IsNodeSelected() ) );
				}
			}
		}
		else if( ((TEventUI*)pEvent)->Type == UIEVENT_SETCURSOR ) 
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		}
		return true;
	}

} // namespace DuiLib