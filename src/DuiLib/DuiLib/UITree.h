#pragma once
#include <vector>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace DuiLib {

	class UILIB_API CTreeNode
	{
	public:
		CTreeNode();
		~CTreeNode();

	public:
		void SetNodeLevel( int nodeLevel );
		int GetNodeLevel();
		void SetNodeExpand( bool bNodeExpand );
		bool IsNodeExpand();
		void SetNodeSelected( bool bNodeSelected );
		bool IsNodeSelected();

		void SetListElementUI( CControlUI* pListElementUI );
		CControlUI* GetListElementUI();
		void SetExpandControl( CControlUI* pExpandControl );
		CControlUI* GetExpandControl();
		void SetSelectControl( CControlUI* pSelectControl );
		CControlUI* GetSelectControl();

		int GetChildrenNum();
		bool IsHasChildren();
		CTreeNode* GetChildrenAt( int index );
		CTreeNode* GetLastChildren();
		void AddChildren( CTreeNode* pXMLTreeNode );
		bool RemoveChildren( CTreeNode* pXMLTreeNode );

		CTreeNode* GetParent();

	protected:
		void SetNodeParent( CTreeNode* pXMLTreeNode );

		typedef std::vector<CTreeNode*> ChildrenNodeVec;
		ChildrenNodeVec m_vecChildrenNode;
		CTreeNode*  m_pParentNode;
		int  m_nNodeLevel;
		bool m_bIsNodeExpand;
		bool m_bIsNodeSelected;
		CControlUI* m_pListElementUI; // maybe is ListElementUI, ListLabelElementUI, ListTextElementUI or ListContainerElementUI
		CControlUI* m_pExpandControl;
		CControlUI* m_pSelectControl;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	class UILIB_API CTreeUI : public CListUI
	{
	public:
		CTreeUI();
		~CTreeUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

	public:
		bool PreCheckControl( CControlUI* pControl );
		bool Add( CControlUI* pControl );
		bool AddAt( CControlUI* pControl, int index );

		bool Remove( CControlUI* pControl );
		bool RemoveAt( int index );
		void RemoveAll();

		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);

		CTreeNode* AddNode( CTreeNode* pNodeRoot, CControlUI* pListElementUI, 
			COptionUI* pExpandOption, bool bIsExpand, COptionUI* pSelectedOption, bool bIsSelected, int vanum = 0, ... );
		void OnExpandNode( CTreeNode* pXMLTreeNode );
		void OnSelectedNode( CTreeNode* pXMLTreeNode, bool bSelected );
		void SelectedNode( CTreeNode* pXMLTreeNode, bool bSelected );
		bool RemoveNode( CTreeNode* pXMLTreeNode );

	protected:
		int GetListElementIndex( CTreeNode* pNode );
		bool OnCommonEvent4ExpandOption( void* pEvent );
		bool OnCommonEvent4SelectedOption( void* pEvent );

		CTreeNode* m_pRoot;
		CStdString m_sListElementUIClassName;
	};

} // namespace DuiLib

#pragma warning(pop)