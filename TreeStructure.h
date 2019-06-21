#ifndef TREESTRUCTURE_H
#define TREESTRUCTURE_H

#include <QList>

#if defined(_MSC_VER) && (_MSC_VER > 1600)
#pragma execution_character_set("utf-8")
#endif

template<typename T>
class TreeStructure
{
public:
	typedef bool (*TraversalHook)(TreeStructure<T>* instance, TreeStructure<T>* nodeToVisit);

public:
	TreeStructure(T* nodeData, TreeStructure* parent = nullptr):
		m_nodeData(nodeData),
		m_parentNode(parent)
    {
		if(nullptr != parent)
			parent->addChild(this,-1,false); // append to parent's children
    }

	~TreeStructure()
    {
		delete m_nodeData;
		qDeleteAll(m_childNodes);
    }

	TreeStructure* child(int row) const
    {
		if(false == validateChildIndex(row))
			return nullptr; // invalid child!

		return m_childNodes.value(row);
    }

	TreeStructure* sibling(int siblingRow) const
	{
		if(parent() == nullptr)
			return nullptr; // no sibling

		return parent()->child(siblingRow);
	}

	TreeStructure* parent() const
    {
		return m_parentNode;
    }

	void setParent(TreeStructure* parent)
    {
		this->m_parentNode=parent;
    }

	bool addChild(TreeStructure* newChild, int row=-1, bool checkParentFirst=true)
    {
		if(checkParentFirst && nullptr != newChild->parent()){ // can't add child which already has parent, use moveChild instead
			return false;
		}

		if(0==row && 0==m_childNodes.size()){ // 往空白list中插入0下标
			m_childNodes.append(newChild);
		}else if(row<0 || m_childNodes.size()==row){ // 追加
			m_childNodes.append(newChild);
        }else{ // 插入
			if(false == validateChildIndex(row))
                return false;

			m_childNodes.insert(row,newChild);
        }

		newChild->setParent(this);
        return true;
    }

	bool removeChild(int row, bool isDelete=true)
    {
		if(false == validateChildIndex(row))
            return false;

		auto childToRemove=m_childNodes.takeAt(row);
		if(isDelete)
			delete childToRemove;

        return true;
    }

	bool moveChild(int oldRow, int newRow)
	{
		if(false == validateChildIndex(oldRow) || false == validateChildIndex(newRow))
			return false;

		m_childNodes.move(oldRow,newRow);
		return true;
	}

	bool moveTo(TreeStructure* newParent, int newRow=-1)
	{
		bool isOk=true;
		auto oldRow=row();
		auto oldParent=parent();

		if(oldParent == newParent){
			if(newRow<0)
				newRow=childCount()-1;

			isOk=oldParent->moveChild(oldRow,newRow);
		}else{ // different parent
			isOk=newParent->addChild(this,newRow,false);

			if(true==isOk && nullptr !=oldParent)
				oldParent->removeChild(oldRow,false);
		}

		return isOk;
	}

    int childCount() const
    {
		return m_childNodes.count();
    }

	T* data() const
    {
		return m_nodeData;
    }

    int row() const
    {
		if(m_parentNode)
			return m_parentNode->m_childNodes.indexOf(const_cast<TreeStructure*>(this));

        return 0;
    }

	inline bool validateChildIndex(int index) const
	{
		return !(index < 0 || index >= childCount());
	}

	void copyTo(TreeStructure* destTree) const
	{
		// BFS Traversal
		decltype(m_childNodes) srcNodes;
		decltype(m_childNodes) destNodes;

		srcNodes.push_front(const_cast<TreeStructure*>(this));
		destNodes.push_front(destTree);

		destTree->data()->copyFromAnotherObject(data()); // root node data

		while(false == srcNodes.isEmpty()){
			auto srcNode=srcNodes.takeFirst();
			auto destNode=destNodes.takeFirst();

			for(auto i=0;i<srcNode->childCount();++i){
				auto srcNodeChild=srcNode->child(i);
				srcNodes.append(srcNodeChild);

				// create new node
				auto newNodeData=srcNodeChild->data()->getInstance();
				newNodeData->copyFromAnotherObject(srcNodeChild->data());
				auto newNode=new TreeStructure(newNodeData, destNode);

				destNodes.append(newNode);
			}
		}
	}

	void dfs(TraversalHook hook) // 深度优先遍历
	{
		decltype(m_childNodes) nodesToVisit;

		nodesToVisit.push_front(this);

		while(false == nodesToVisit.isEmpty()){
			auto node=nodesToVisit.takeFirst();

			if(false == (*hook)(this,node))
				break;

			for(auto i=node->childCount()-1;i>=0;--i)
				nodesToVisit.prepend(node->child(i));
		}
	}

	void bfs(TraversalHook hook) // 广度优先遍历
	{
		decltype(m_childNodes) nodesToVisit;

		nodesToVisit.push_front(this);

		while(false == nodesToVisit.isEmpty()){
			auto node=nodesToVisit.takeFirst();

			if(false == (*hook)(this,node))
				break;

			for(auto i=0;i<node->childCount();++i)
				nodesToVisit.append(node->child(i));
		}
	}

protected: // members
	T* m_nodeData;
	TreeStructure<T>* m_parentNode;
	QList<decltype(m_parentNode)> m_childNodes;
};

#endif
