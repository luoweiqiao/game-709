
#include "sangongdou_logic.h"
#include "svrlib.h"
#include "math/rand_table.h"

using namespace svrlib;

namespace game_sangongdou
{
//扑克数据
BYTE CSanGongDouLogic::m_cbCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//黑桃 A - K
};

BYTE CSanGongDouLogic::m_cbArCardColor[4]  = { 0x00,0x10,0x20,0x30 };
BYTE CSanGongDouLogic::m_cbArCardValue[13] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D };
BYTE CSanGongDouLogic::m_cbArGongPai[12] = { 0x0B,0x0C,0x0D,0x1B,0x1C,0x1D,0x2B,0x2C,0x2D,0x3B,0x3C,0x3D };

//////////////////////////////////////////////////////////////////////////
//构造函数
CSanGongDouLogic::CSanGongDouLogic()
{
}

//析构函数
CSanGongDouLogic::~CSanGongDouLogic()
{
}

//获取类型
BYTE CSanGongDouLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount==MAX_COUNT);
	//大小排序
	SortCardList(cbCardData, cbCardCount);

	if (cbCardCount == MAX_COUNT)
	{
		BYTE cbFirstValue = GetCardLogicValue(cbCardData[0]);
		BYTE cbSecondValue = GetCardLogicValue(cbCardData[1]);
		BYTE cbThreeValue = GetCardLogicValue(cbCardData[2]);

		//大小三公
		if (cbFirstValue == cbSecondValue && cbSecondValue == cbThreeValue)
		{
			if (cbFirstValue > 0xA)
				return CT_DASANGONG;
			else
				return CT_XIAOSANGONG;
		}

		//混三公
		if (cbFirstValue > 0xA && cbSecondValue > 0xA && cbThreeValue > 0xA)
		{
			return CT_HUNSANGONG;
		}

		//计算点数
		if (cbFirstValue > 0xA)		cbFirstValue = 0;
		if (cbSecondValue > 0xA)	cbSecondValue = 0;
		if (cbThreeValue > 0xA)		cbThreeValue = 0;

		BYTE sum_point = (cbFirstValue + cbSecondValue + cbThreeValue) % 10;
		return CT_ZERO_POINT + sum_point;
	}
	return CT_ZERO_POINT;
}

//排列扑克
void CSanGongDouLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount,bool bAsc)
{
	//转换数值
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardLogicValue(cbCardData[i]);	

	//排序操作
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
            if(bAsc){
    			if((cbLogicValue[i]<cbLogicValue[i+1]) || ((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
    			{
    				//交换位置
    				cbTempData=cbCardData[i];
    				cbCardData[i]=cbCardData[i+1];
    				cbCardData[i+1]=cbTempData;
    				cbTempData=cbLogicValue[i];
    				cbLogicValue[i]=cbLogicValue[i+1];
    				cbLogicValue[i+1]=cbTempData;
    				bSorted=false;
    			}	
            }else{
                if((cbLogicValue[i]>cbLogicValue[i+1]) || ((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]>cbCardData[i+1])))
    			{
    				//交换位置
    				cbTempData=cbCardData[i];
    				cbCardData[i]=cbCardData[i+1];
    				cbCardData[i+1]=cbTempData;
    				cbTempData=cbLogicValue[i];
    				cbLogicValue[i]=cbLogicValue[i+1];
    				cbLogicValue[i+1]=cbTempData;
    				bSorted=false;
    			}                
            }	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//混乱扑克(所有牌)
void CSanGongDouLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	BYTE cbCardData[getArrayLen(m_cbCardListData)];
	memcpy(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(getArrayLen(m_cbCardListData)-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[getArrayLen(m_cbCardListData)-bRandCount];
	} while (bRandCount<cbBufferCount);
    
	return;
}

//混乱扑克(仅公牌)
void CSanGongDouLogic::RandGongCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	BYTE cbCardData[getArrayLen(m_cbArGongPai)];
	memcpy(cbCardData, m_cbArGongPai, sizeof(m_cbArGongPai));

	//混乱扑克
	BYTE bRandCount = 0, bPosition = 0;
	do
	{
		bPosition = rand() % (getArrayLen(m_cbArGongPai) - bRandCount);
		cbCardBuffer[bRandCount++] = cbCardData[bPosition];
		cbCardData[bPosition] = cbCardData[getArrayLen(m_cbArGongPai) - bRandCount];
	} while (bRandCount < cbBufferCount);

	return;
}

//逻辑数值
BYTE CSanGongDouLogic::GetCardLogicValue(BYTE cbCardData)
{
	//扑克属性
	BYTE bCardColor=GetCardColor(cbCardData);
	BYTE bCardValue=GetCardValue(cbCardData);
	// 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D
	//	 14	   2	3	 4	  5	   6    7    8	  9   10   11   12   13
	//转换数值
	return (bCardValue==1)?(bCardValue+13):bCardValue;
}

//对比扑克
BYTE CSanGongDouLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	//设置变量
	BYTE FirstData[MAX_COUNT],NextData[MAX_COUNT];
	memcpy(FirstData,cbFirstData,sizeof(FirstData));
	memcpy(NextData,cbNextData,sizeof(NextData));

	//大小排序
	SortCardList(FirstData,cbCardCount);
	SortCardList(NextData,cbCardCount);

	//获取类型
	BYTE cbNextType=GetCardType(NextData,cbCardCount);
	BYTE cbFirstType=GetCardType(FirstData,cbCardCount);

	//类型判断
	if (cbFirstType != cbNextType)
	{
		return (cbFirstType > cbNextType) ? 1 : 0;
	}
	else
	{
		//简单类型
		switch (cbFirstType)
		{
			case CT_DASANGONG:				//大三公
			case CT_XIAOSANGONG:			//小三公
			{
				//对比数值			
				BYTE cbNextValue = GetCardLogicValue(NextData[0]);
				BYTE cbFirstValue = GetCardLogicValue(FirstData[0]);
				return (cbFirstValue > cbNextValue) ? 1 : 0;			
			}
			case CT_HUNSANGONG:				//混三公
			{
				return DRAW;
			}
			case CT_NINE_POINT:			//9点
			case CT_EIGHT_POINT:		//8点
			case CT_SERVEN_POINT:		//7点
			case CT_SIX_POINT:			//6点
			case CT_FIVE_POINT:			//5点
			case CT_FOUR_POINT:			//4点
			case CT_THREE_POINT:		//3点
			case CT_TWO_POINT:			//2点
			case CT_ONE_POINT:			//1点
			case CT_ZERO_POINT:			//0点
			{
				//先比较公牌数
				BYTE cbFirstGongPiaNum = 0;
				BYTE cbNextGongPiaNum = 0;
				for (BYTE i = 0; i < cbCardCount; i++)
				{
					BYTE cbTmpValue = GetCardLogicValue(FirstData[i]);
					if (cbTmpValue > 0xA)
					{
						cbFirstGongPiaNum++;
					}					
				}
				for (BYTE i = 0; i < cbCardCount; i++)
				{
					BYTE cbTmpValue = GetCardLogicValue(NextData[i]);
					if (cbTmpValue > 0xA)
					{
						cbNextGongPiaNum++;
					}
				}

				if (cbFirstGongPiaNum != cbNextGongPiaNum)
				{
					return (cbFirstGongPiaNum > cbNextGongPiaNum) ? 1 : 0;
				}
				else
				{
					//再比较最大牌
					BYTE cbNextValue = GetCardLogicValue(NextData[0]);
					BYTE cbFirstValue = GetCardLogicValue(FirstData[0]);

					if (cbFirstValue == cbNextValue)
					{
						return DRAW;
					}
					else
					{
						return (cbFirstValue > cbNextValue) ? 1 : 0;
					}
				}			
			}
		}
	}	
	return 1;
}

bool CSanGongDouLogic::GetCardTypeData(int iArProCardType[], BYTE cbArCardData[][MAX_COUNT])
{
	int iArTempProCardType[SHOW_CARD_COUNT] = { 0 };
	for (int i = 0; i < SHOW_CARD_COUNT; i++)
	{
		iArTempProCardType[i] = iArProCardType[i];
	}

	BYTE cbArTempCardData[SHOW_CARD_COUNT][MAX_COUNT] = { 0 };
	memset(cbArTempCardData, 0, sizeof(cbArTempCardData));

	BYTE cbCardListData[3] = { 0 };
	
	for (int i = 0; i < SHOW_CARD_COUNT; i++)
	{
		bool bIsFlag = false;
		if (iArTempProCardType[i] == Pro_Index_DaSanGong)
		{
			bIsFlag = GetTypeDaSanGong(cbCardListData, i * 3, cbArTempCardData[i], MAX_COUNT);
		}
		else if (iArTempProCardType[i] == Pro_Index_XiaoSanGong)
		{
			bIsFlag = GetTypeXiaoSanGong(cbCardListData, i * 3, cbArTempCardData[i], MAX_COUNT);
		}
		else if (iArTempProCardType[i] == Pro_Index_HunSanGong)
		{
			bIsFlag = GetTypeHunSanGong(cbCardListData, i * 3, cbArTempCardData[i], MAX_COUNT);
		}
		else if (iArTempProCardType[i] == Pro_Index_Point)
		{
			bIsFlag = GetTypePoint(cbCardListData, i * 3, cbArTempCardData[i], MAX_COUNT);
		}
		memcpy(cbCardListData, cbArTempCardData[i], sizeof(cbCardListData));
	}

	for (int i = 0; i < SHOW_CARD_COUNT; i++)
	{
		for (int j = 0; j < MAX_COUNT; j++)
		{
			cbArCardData[i][j] = cbArTempCardData[i][j];
		}
	}
	return false;
}

bool CSanGongDouLogic::IsValidCard(BYTE cbCardData)
{
	for (int i = 0; i < FULL_COUNT; i++)
	{
		if (m_cbCardListData[i] == cbCardData)
		{
			return true;
		}
	}
	return false;
}

//获取大三公
bool CSanGongDouLogic::GetTypeDaSanGong(BYTE cbCardListData[], BYTE cbListCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//随机所有的公牌
	do
	{
		RandGongCardList(cbCardData, cbCardCount);
		BYTE cbCardType = GetCardType(cbCardData, MAX_COUNT);
		if (cbCardType == CT_DASANGONG)
		{
			if (cbListCount == 0)
			{
				break;
			}
			else
			{
				//判断是否在已经确定的牌中
				bool isExist = false;
				for (int i = 0; i < cbListCount; i++)
				{
					if (cbCardListData[i] == cbCardData[0] && cbCardListData[i] == cbCardData[1] && cbCardListData[i] == cbCardData[2])
					{
						isExist = true;
						break;
					}
				}
				if (!isExist)
				{
					break;
				}
			}
		}
	} while (1);	
	return true;
}

//获取小三公
bool CSanGongDouLogic::GetTypeXiaoSanGong(BYTE cbCardListData[], BYTE cbListCount, BYTE cbCardData[], BYTE cbCardCount)
{
	// 随机小三公的数值
	BYTE cbCardValue = 0;
	do 
	{
		cbCardValue = g_RandGen.RandRange(1, 10);
		bool isExist = false;
		for (int i = 0; i < cbListCount; i++)
		{
			if (GetCardLogicValue(cbCardListData[i]) == cbCardValue)
			{
				isExist = true;
				break;
			}
		}
		if (!isExist)
		{
			break;
		}
	} while (1);

	
	BYTE cbCardColor[3] = { 0 };
	BYTE cbCfgCardColor[4] = { 0 };
	memcpy(cbCfgCardColor, m_cbArCardColor, sizeof(m_cbArCardColor));

	// 选其中三个颜色
	BYTE cbRandCount = 0;
	do
	{
		BYTE cbPosition = g_RandGen.RandRange(0, MAX_CARD_COLOR - cbRandCount - 1);
		cbCardColor[cbRandCount] = cbCfgCardColor[cbPosition];
		cbCfgCardColor[cbPosition] = cbCfgCardColor[MAX_CARD_COLOR - cbRandCount - 1];
		cbRandCount++;
	} while (cbRandCount < cbCardCount);

	for (int i = 0; i < cbCardCount; i++)
	{
		cbCardData[i] = cbCardValue | cbCardColor[i];
	}
	return true;
}

//获取混三公
bool CSanGongDouLogic::GetTypeHunSanGong(BYTE cbCardListData[], BYTE cbListCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//随机所有的公牌
	do
	{
		RandGongCardList(cbCardData, cbCardCount);
		BYTE cbCardType = GetCardType(cbCardData, MAX_COUNT);
		if (cbCardType == CT_HUNSANGONG)
		{
			if (cbListCount == 0)
			{
				break;
			}
			else
			{
				//判断是否在已经确定的牌中
				bool isExist = false;
				for (int i = 0; i < cbListCount; i++)
				{
					if (cbCardListData[i] == cbCardData[0] && cbCardListData[i] == cbCardData[1] && cbCardListData[i] == cbCardData[2])
					{
						isExist = true;
						break;
					}
				}
				if (!isExist)
				{
					break;
				}
			}
		}
	} while (1);
	return true;
}

//获取点牌
bool CSanGongDouLogic::GetTypePoint(BYTE cbCardListData[], BYTE cbListCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//随机所有的公牌
	do
	{
		RandCardList(cbCardData, cbCardCount);
		BYTE cbCardType = GetCardType(cbCardData, MAX_COUNT);
		if (cbCardType < CT_HUNSANGONG)
		{
			if (cbListCount == 0)
			{
				break;
			}
			else
			{
				//判断是否在已经确定的牌中
				bool isExist = false;
				for (int i = 0; i < cbListCount; i++)
				{
					if (cbCardListData[i] == cbCardData[0] && cbCardListData[i] == cbCardData[1] && cbCardListData[i] == cbCardData[2])
					{
						isExist = true;
						break;
					}
				}
				if (!isExist)
				{
					break;
				}
			}
		}
	} while (1);

	return true;
}

bool CSanGongDouLogic::IsSameCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	SortCardList(cbFirstData, cbCardCount);
	SortCardList(cbNextData, cbCardCount);

	for (BYTE i = 0; i < cbCardCount; i++)
	{
		if (GetCardValue(cbFirstData[i]) != GetCardValue(cbNextData[i]))
		{
			return false;
		}
	}
	return true;
}

};

