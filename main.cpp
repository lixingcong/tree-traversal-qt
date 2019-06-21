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

	MyTree t(new int(20));

	auto hook=[](MyTree* instance, MyTree* node){
		qDebug("data=%d",*node->data());
		return true;
	};

	t.dfs(hook);

	return 0;
}
