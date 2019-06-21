/*
 * %{Cpp:License:FileName}
 *
 * FIXME: 功能简要概述
 *
 * Created on: 2019年 6月 21日
 * Author: lixingcong
 */

#include <QtCore>
#include <QDebug>
#include "TreeStructure.h"

class TreeItem:public TreeStructure::AbstractTreeItem
{
public:
	TreeItem(int data):data(data){}
	TreeItem* cloneInstance()const override
	{
		return new TreeItem(this->data);
	}

	int data;
};

int main()
{
	auto oldTree = new TreeStructure(new TreeItem(0));

	auto n1 = new TreeStructure(new TreeItem(1), oldTree);
	auto n2 = new TreeStructure(new TreeItem(2), oldTree);

	auto n11 = new TreeStructure(new TreeItem(11), n1);
	auto n12 = new TreeStructure(new TreeItem(12), n1);

	auto n111 = new TreeStructure(new TreeItem(111), n11);
	auto n121 = new TreeStructure(new TreeItem(121), n12);
	auto n122 = new TreeStructure(new TreeItem(122), n12);

	auto n21 = new TreeStructure(new TreeItem(21), n2);
	auto n22 = new TreeStructure(new TreeItem(22), n2);

	auto hook = [](TreeStructure* instance, TreeStructure* node) {
		auto treeItem=static_cast<TreeItem*>(node->data());
		qDebug("data=%d", treeItem->data);
		return true;
	};

	qDebug("old tree:");
	oldTree->bfs(hook);

	auto newTree=oldTree->cloneTree();
	qDebug("new tree:");
	newTree->bfs(hook);

	delete oldTree;
	delete newTree;
	return 0;
}
