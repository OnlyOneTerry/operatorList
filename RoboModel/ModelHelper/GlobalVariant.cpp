#include "GlobalVariant.h"


bool GlobalVariant::flag = false;
bool GlobalVariant::isRedo = false;
bool GlobalVariant::globalCursorFlag = false;//用于处理光标控制冲出
GlobalVariant* GlobalVariant::inst = nullptr;

GlobalVariant *GlobalVariant::getInst()
{
   if(inst==nullptr)
       inst = new GlobalVariant;

   return inst;
}

GlobalVariant::GlobalVariant()
{

}
