#ifndef QWORD_H
#define QWORD_H

#define _WIN32_DCOM

#include <QMainWindow>
#include"qmainwindow.h"
#include <QString>
#include <QVariant>
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxWidget>
#include "objbase.h"
//#include "GlobalAppData.h"

class QWord : public QObject
{
	Q_OBJECT

public:
	QWord(QObject *parent = 0);
	~QWord();

public:
	QAxObject* getDocuments(){return m_documents;}
	QAxObject* getDocument(){return m_document;}
	QAxObject* getWordApp(){return m_word;}	
public:
	/**************************************************************************/
	/* 文件   操作                                                            */
	/**************************************************************************/
	void save();				//保存操作内容	
	void close();				//关闭 退出 析构时候也会自动调用一次
	void saveAs();				//新建word另存为
	bool createWord(QString reportname );		//创建一个新的word文档
	void openWord();			//打开一个现有的word文档
	bool createNewWord(const QString& filePath );		//创建一个新的word文档

	void setTableAutoFitBehavior(int flag);//表格自动拉伸列: 0 固定  1根据内容调整  2 根据窗口调整
	void setPageOrientation(int flag);	//设置页面0为纵向wdOrientPortrait  1为横向wdOrientLandscape 
	void setWordPageView(int flag);	//设置页面视图，是web视图wdWebView 6还是页面视图wdPrintView 3 或者阅读视图wdReadingView 7
	void setFontSize(int fontsize);						//设置字体大小
	void setFontName(QString& fontName);				//设置字体	比如 “宋体”
	void setFontBold(bool flag);						//字体加粗
	void setParagraphAlignment(int flag);				//设置选中位置文字居左 0 ，居中 1 ,居右 2
	void setRowAlignment(int tableIndex,int row,int flag);
	//同时设置列宽和列高可以固定图片的大小
	void setColumnWidth(int column, int width);			//设置列宽
	void setColumnHeight(int column, int height);		//设置列高
	void setRowHeight(int nTable,int Row, int height);
	void setCellString(int row, int column, const QString& text);
	void setCellFontBold(int row, int column, bool isBold);			//设置内容粗体  isBold控制是否粗体
	void setCellFontSize(int row, int column, int size);			//设置文字大小
	void setOptionCheckSpell(bool flags);		//设置审阅的拼写检查  true开启检查  false 取消检查 

	QString GetText();												//获取内容
	void getUsedRange(int *topLeftRow, int *topLeftColumn, int *bottomRightRow, int *bottomRightColumn);
	void setSelectionRange(int start,int end);	//"SetRange(1, 9)"第1个字符后开始，到第9个字符结束范围
	QVariant getCellValue(int row, int column);						//获取单元格内容 
	int getTableCount();											//获取word中表格总数	


	QString getStrErrorInfo(){return m_strError;}					//获取代码中出现的错误信息可以用QMessageBox::information打印 在cpp不用QMessageBox 是怕你们在线程中调导出报表  
	void deleteSelectColumn(int column);							//删除指定的列
	void moveForEnd();												//移动选定对象到文档末尾
	void insertCellPic(int row,int column,const QString& picPath);	//单元格插入图片
	void intsertTable(int row,int column);							//插入一个几行几列表格
	void insertMoveDown();											//插入回车
	void insertText(const QString& text);							//插入文字
	void insertTable(int tableIndex,int row,int column);

	void MergeCells(int tableIndex, int nStartRow,int nStartCol,int nEndRow,int nEndCol);//合并单元格	
	bool open(const QString& strFilePath,bool bVisable);
	bool openword(bool bVisable);
	bool isOpen();


	void setColumnWidth(int nTable,int column,int width);
	void setCellString(int nTable,int row,int column,const QString& text);
	void insertCellPic(int nTable,int row,int column,const QString& picPath);
	void setCellFontBold(int nTable,int row,int column,bool isBold);
	void setCellFontSize(int nTable,int row,int column,int size);
	//void addTableRow(int nTable ,int nRow,int rowCount);
	void addTableRow(int tableIndex ,int nRow,int rowCount);
	void setColumnHeight(int nTable,int column, int height);
	void intsertTable(int tableIndex, int row,int column);

public:
	void setVisible(bool isVisible);

private:
	QAxObject* m_word;
	QAxObject* m_documents;	
	QAxObject* m_document;	
	QString  m_fileName;
	QString  m_saveName;
	QString  m_strError;
	QString  m_strFilePath;
	bool m_bOpened;

};

#endif // QWORD_H
