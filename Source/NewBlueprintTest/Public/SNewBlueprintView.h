#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


class SNewBlueprintView : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SNewBlueprintView)
        {}

    SLATE_END_ARGS()

public:
    SNewBlueprintView() {}
    ~SNewBlueprintView();

    void Construct(const FArguments& InArgs);

    // SWidget interface
    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
    // End of SWidget interface


    // 按钮“新建蓝图类”点击响应
    FReply NewBlueprintClicked();

    // 按钮“存储属性”点击响应。实例化一个Blueprint类型的实例TempObject，并将编辑框PropertyInput中的字符串反序列化到TempObject的属性中。
    FReply SaveToProperty();

    // 按钮“修改属性”点击响应。将原属性移除，添加两个新属性，并编译蓝图。
    FReply ModifyProperty();

    // 按钮“读取属性”点击响应。属性迭代器遍历TempObject
    FReply LoadProperty();

private:

    TSharedPtr<UBlueprint> Blueprint; // 动态创建的蓝图类

    //TSharedPtr<UBlueprintGeneratedClass> GeneratedClass;

    UObject* TempObject; // 动态创建的Blueprint类型的对象，用于存取Property

    TSharedPtr<SEditableTextBox> PropertyInput; // 属性输入

    TSharedPtr<SVerticalBox> PropertyOutputBox; // 属性输出框
};
