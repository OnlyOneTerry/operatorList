#ifndef GLOBALVARIANT_H
#define GLOBALVARIANT_H


class GlobalVariant
{
public:
    static GlobalVariant*  getInst();
    static bool flag;
    static bool isRedo;
    static bool globalCursorFlag;
private:
    GlobalVariant();
    static GlobalVariant* inst;
};

#endif // GLOBALVARIANT_H
