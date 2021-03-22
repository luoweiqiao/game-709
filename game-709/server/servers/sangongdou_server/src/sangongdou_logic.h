

#ifndef SERVER_SANGONGDOU_LOGIC_H
#define SERVER_SANGONGDOU_LOGIC_H

#include "poker/poker_logic.h"

//区域索引
#define JETTON_INDEX_DRAGON			0				//龙
#define JETTON_INDEX_PHOENIX		1				//凤
#define JETTON_INDEX_DRAW			2				//和

#define JETTON_INDEX_COUNT			3				//下注区域数目(龙，凤，和)
#define SHOW_CARD_COUNT				2				//开牌区域数目(龙，凤)
#define MAX_CARD_COUNT				3

namespace game_sangongdou
{
//宏定义

static const int GAME_PLAYER	    =			5;									//游戏人数
//结束原因
static const int  GER_NO_PLAYER		=			0x10;								//没有玩家
static const int  GER_COMPARECARD	=			0x20;								//比牌结束
static const int  GER_OPENCARD		=			0x30;								//开牌结束

//数目定义
static const int FULL_COUNT		=			    52;									//全牌数目
static const int MAX_COUNT		=				3;									//最大数目
static const int DRAW           =				2;									//和局类型
static const int MAX_CARD_VALUE =				14;									//最大牌值
static const int MAX_CARD_COLOR	=				4;									//最大颜色

//扑克类型
enum emSANGONGDOU_CARD_TYPE
{
    //扑克类型
    CT_ZERO_POINT			= 0,		//0点类型
	CT_ONE_POINT			= 1,		//1点类型
	CT_TWO_POINT			= 2,		//2点类型
	CT_THREE_POINT			= 3,		//3点类型
	CT_FOUR_POINT			= 4,		//4点类型
	CT_FIVE_POINT			= 5,		//5点类型
	CT_SIX_POINT			= 6,		//6点类型
	CT_SERVEN_POINT			= 7,		//7点类型
	CT_EIGHT_POINT			= 8,		//8点类型
    CT_NINE_POINT			= 9,		//9点类型									
    CT_HUNSANGONG			= 10,		//混三公类型		
	CT_XIAOSANGONG			= 11,		//小三公类型
	CT_DASANGONG			= 12,		//大三公类型
    CT_SPECIAL				= 13		//特殊类型
};

//发牌概率索引
enum emSANGONGDOU_CARD_TYPE_PRO_INDEX
{
	Pro_Index_DaSanGong = 0,	
	Pro_Index_XiaoSanGong,
	Pro_Index_HunSanGong,
	Pro_Index_Point,	
	Pro_Index_MAX,
};

//游戏逻辑类
class CSanGongDouLogic : public CPokerLogic
{
	//变量定义
private:
	static BYTE						m_cbCardListData[52];			//扑克定义
	static BYTE						m_cbArCardColor[4];				//扑克定义
	static BYTE						m_cbArCardValue[13];			//扑克定义
	static BYTE						m_cbArGongPai[12];				//扑克定义

	//函数定义
public:
	//构造函数
	CSanGongDouLogic();
	//析构函数
	virtual ~CSanGongDouLogic();

	//类型函数
public:
	
	//获取类型
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount,bool bAsc = true);
	//混乱扑克(所有牌)
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//混乱扑克(仅公牌)
	void RandGongCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
    
	//功能函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
	//获取牌型扑克
	bool GetCardTypeData(int iArProCardType[], BYTE cbArCardData[][MAX_COUNT]);
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//获取大三公
	bool GetTypeDaSanGong(BYTE cbCardListData[], BYTE cbListCount,BYTE cbCardData[], BYTE cbCardCount);
	//获取小三公
	bool GetTypeXiaoSanGong(BYTE cbCardListData[], BYTE cbListCount, BYTE cbCardData[], BYTE cbCardCount);
	//获取混三公
	bool GetTypeHunSanGong(BYTE cbCardListData[], BYTE cbListCount, BYTE cbCardData[], BYTE cbCardCount);
	//获取点牌
	bool GetTypePoint(BYTE cbCardListData[], BYTE cbListCount, BYTE cbCardData[], BYTE cbCardCount);	
	//判断是否相等
	bool IsSameCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);

};
};

#endif //SERVER_SANGONGDOU_LOGIC_H
