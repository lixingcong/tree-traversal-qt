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

int main()
{
	typedef TreeStructure<int> MyTree;

	auto rootNode=new MyTree(new int(0));

	auto n1=new MyTree(new int(1),rootNode);
	auto n2=new MyTree(new int(2),rootNode);

	auto n11=new MyTree(new int(11),n1);
	auto n12=new MyTree(new int(12),n1);

	auto n111=new MyTree(new int(111),n11);
	auto n121=new MyTree(new int(121),n12);
	auto n122=new MyTree(new int(122),n12);

	auto n21=new MyTree(new int(21),n2);
	auto n22=new MyTree(new int(22),n2);

	auto hook=[](MyTree* instance, MyTree* node){
		qDebug("data=%d",*node->data());
		return true;
	};

	rootNode->bfs(hook);

	delete rootNode;
	return 0;
}
